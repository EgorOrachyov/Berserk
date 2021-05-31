/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <BerserkVulkan/VulkanDescriptorSetManager.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanUniformBuffer.hpp>
#include <BerserkVulkan/VulkanSampler.hpp>
#include <BerserkVulkan/VulkanTexture.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {
    namespace RHI {

        VulkanDescriptorSetManager::VulkanDescriptorSetManager(struct VulkanDevice &device, uint32 releaseFrequency, uint32 timeToKeep)
                : VulkanCache(device, releaseFrequency, timeToKeep) {

        }

        VulkanDescriptorSetManager::~VulkanDescriptorSetManager() {
            for (auto& e: mDescriptorPools) {
                ReleasePool(e.GetSecond());
            }
        }

        void VulkanDescriptorSetManager::BindLayout(VkDescriptorSetLayout layout, const RefCounted<ProgramMeta>& meta) {
            assert(layout);
            assert(meta);

            // Invalidate prev mappings, layouts, and offsets
            InvalidateSet();

            auto poolPtr = mDescriptorPools.GetPtr(layout);

            mLayout = layout;

            if (poolPtr == nullptr) {
                // Computes set properties, to resize write infos arrays
                uint32 maxBuffers = meta->paramBlocks.GetSize();
                uint32 maxImages = 0;

                for (const auto& sampler: meta->samplers) {
                    auto& info = sampler.GetSecond();
                    maxImages += info.arraySize;

                    assert(info.arraySize > 0);
                }

                Pool pool;
                pool.maxBuffers = maxBuffers;
                pool.maxImages = maxImages;
                pool.buckets.Resize(Limits::MAX_FRAMES_IN_FLIGHT);

                poolPtr = &mDescriptorPools.Move(layout, pool);

                BERSERK_VK_LOG_INFO(BERSERK_TEXT("Cache new DescriptorPool: {0} for program {1}"), poolPtr, meta->name);
            }

            // This pool will be used for sets allocations
            mMeta = meta;
            mPool = poolPtr;
            mPool->frameUsed = mCurrentFrame;

            // Resize write image/buffer and desc arrays
            // For each bound object we have one write info entry
            mBufferWriteInfo.Clear();
            mBufferWriteInfo.Resize(mPool->maxBuffers);
            mImageWriteInfo.Clear();
            mImageWriteInfo.Resize(mPool->maxImages);
            mWriteInfo.Clear();
            mWriteInfo.Resize(mPool->maxBuffers + mPool->maxImages);

            // Mark offsets for each potential binding
            // Setup write infos for each binding. This is done once,
            // since when we create new descSet `dstSet` is only changed
            uint32 currentBufferWriteInfo = 0;
            uint32 currentImageWriteInfo = 0;
            uint32 currentWriteInfo = 0;

            for (const auto& entry: mMeta->paramBlocks) {
                auto& buffer = entry.GetSecond();
                auto& binding = mBoundUniformBuffers[buffer.slot];
                binding.objectInfo = currentBufferWriteInfo;
                binding.writeInfo = currentWriteInfo;

                auto& writeInfo = mWriteInfo[currentWriteInfo];
                writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeInfo.pNext = nullptr;
                writeInfo.dstSet = nullptr;
                writeInfo.dstArrayElement = 0;
                writeInfo.dstBinding = buffer.slot;
                writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                writeInfo.descriptorCount = 1;
                writeInfo.pBufferInfo = &mBufferWriteInfo[currentBufferWriteInfo];

                currentBufferWriteInfo += 1;
                currentWriteInfo += 1;
            }

            for (const auto& entry: mMeta->samplers) {
                auto& sampler = entry.GetSecond();
                auto& binding = mBoundSamplersBuffers[sampler.location];
                binding.objectInfo = currentImageWriteInfo;
                binding.writeInfo = currentWriteInfo;

                for (uint16 dstArrayElement = 0; dstArrayElement < sampler.arraySize; dstArrayElement++) {
                    auto& writeInfo = mWriteInfo[currentWriteInfo];
                    writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    writeInfo.pNext = nullptr;
                    writeInfo.dstSet = nullptr;
                    writeInfo.dstArrayElement = dstArrayElement;
                    writeInfo.dstBinding = sampler.location;
                    writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    writeInfo.descriptorCount = 1;
                    writeInfo.pImageInfo = &mImageWriteInfo[currentImageWriteInfo];

                    currentImageWriteInfo += 1;
                    currentWriteInfo += 1;
                }
            }
        }

        void VulkanDescriptorSetManager::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
            assert(buffer);
            assert(mLayout);

            AllocateSet();

            auto& binding = mBoundUniformBuffers[index];
            auto vkBuffer = (VulkanUniformBuffer*) buffer.Get();

            auto& bufferInfo = mBufferWriteInfo[binding.objectInfo];
            bufferInfo.buffer = vkBuffer->GetBuffer();
            bufferInfo.offset = byteOffset;
            bufferInfo.range = byteSize;
        }

        void VulkanDescriptorSetManager::BindTexture(const RefCounted<Texture> &texture, uint32 location, uint32 arrayIndex) {
            assert(texture);
            assert(mLayout);

            AllocateSet();

            auto& binding = mBoundSamplersBuffers[location];
            auto vkTexture = (VulkanTexture*) texture.Get();

            auto& imageInfo = mImageWriteInfo[binding.objectInfo];
            imageInfo.imageView = vkTexture->GetView();
            imageInfo.imageLayout = vkTexture->GetPrimaryLayout();
        }

        void VulkanDescriptorSetManager::BindSampler(const RefCounted<Sampler> &sampler, uint32 location, uint32 arrayIndex) {
            assert(sampler);
            assert(mLayout);

            AllocateSet();

            auto& binding = mBoundSamplersBuffers[location];
            auto vkSampler = (VulkanSampler*) sampler.Get();

            auto& imageInfo = mImageWriteInfo[binding.objectInfo];
            imageInfo.sampler = vkSampler->GetHandle();
        }

        VkDescriptorSet VulkanDescriptorSetManager::GetOrCreateSet() {
            assert(mLayout);

            // Case when pipeline has no bindings
            // it's ok
            if (mCurrentSet == nullptr)
                return mCurrentSet;

            // If descriptor set not written, we need to write it
            // otherwise do nothing, since nothing is changed in bindings
            if (!mWritten) {
                mWritten = true;
                vkUpdateDescriptorSets(mDevice.GetDevice(), mWriteInfo.GetSize(), mWriteInfo.GetData(), 0, nullptr);
            }

            return mCurrentSet;
        }

        void VulkanDescriptorSetManager::NextFrame() {
            mFrameIndex = mCurrentFrame % Limits::MAX_FRAMES_IN_FLIGHT;
            InvalidateSet();

            // For current frame mark all sets as free.
            // Also where it is required - reallocate pools
            for (auto& e: mDescriptorPools) {
                auto& pool = e.GetSecond();

                if (pool.buckets[mFrameIndex].IsNotEmpty())
                    ReallocateAndMarkFreeBuckets(pool.buckets[mFrameIndex], pool.maxBuffers, pool.maxImages);
            }
        }

        void VulkanDescriptorSetManager::GC() {
            if (mCurrentFrame >= mLastFrameRelease + mReleaseFrequency) {
                mLastFrameRelease = mCurrentFrame;

                auto current = mDescriptorPools.begin();
                while (current != mDescriptorPools.end()) {
                    auto& e = *current;

                    if (e.GetSecond().frameUsed + mTimeToKeep <= mCurrentFrame) {
                        ReleasePool(e.GetSecond());
                        current = mDescriptorPools.Remove(current);
                    }
                    else
                        ++current;
                }
            }

            mCurrentFrame += 1;
        }

        void VulkanDescriptorSetManager::AllocateBucket(Bucket &bucket, size_t size, uint32 maxBuffers, uint32 maxImages) {
            ArrayFixed<VkDescriptorPoolSize, 2> sizes;

            if (maxBuffers > 0) {
                auto& ps = sizes.Emplace();
                ps.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ps.descriptorCount = size * maxBuffers;
            }

            if (maxImages > 0) {
                auto& ps = sizes.Emplace();
                ps.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;;
                ps.descriptorCount = size * maxImages;
            }

            assert(sizes.IsNotEmpty());
            assert(size > 0);

            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = sizes.GetSize();
            poolInfo.pPoolSizes = sizes.GetData();
            poolInfo.maxSets = size;

            VkDescriptorPool descriptorPool;

            BERSERK_VK_CHECK(vkCreateDescriptorPool(mDevice.GetDevice(), &poolInfo, nullptr, &descriptorPool));
            BERSERK_VK_NAME(mDevice.GetDevice(), descriptorPool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, "Pool-" + mMeta->name);

            bucket.pool = descriptorPool;
            bucket.size = size;
            bucket.allocated = 0;
        }

        void VulkanDescriptorSetManager::AllocateBucketBack(Array<Bucket> &source, uint32 maxBuffers, uint32 maxImages) {
            size_t size = source.IsEmpty()? POOL_ALLOC_BASE: POOL_ALLOC_FACTOR * source.Last().size;
            AllocateBucket(source.Emplace(), size, maxBuffers, maxImages);
        }

        void VulkanDescriptorSetManager::ReallocateAndMarkFreeBuckets(Array<Bucket> &source, uint32 maxBuffers, uint32 maxImages) {
            // If has more than 1 pool, than
            // we need to release all of them and allocate new
            if (source.GetSize() > 1) {
                size_t size = source.Last().size * POOL_ALLOC_FACTOR;
                ReleaseBuckets(source);
                AllocateBucket(source.Emplace(), size, maxBuffers, maxImages);
                return;
            }

            // In case when we have 1 bucket and do not reallocate, reset entire pool
            auto& bucket = source.Last();
            bucket.allocated = 0;
            BERSERK_VK_CHECK(vkResetDescriptorPool(mDevice.GetDevice(), bucket.pool, 0));
        }

        void VulkanDescriptorSetManager::InvalidateSet() {
            mCurrentSet = nullptr;
            mWritten = false;
            mBoundUniformBuffers.Clear();
            mBoundSamplersBuffers.Clear();
            mLayout = nullptr;
            mPool = nullptr;
        }

        void VulkanDescriptorSetManager::AllocateSet() {
            if (mCurrentSet == nullptr || mWritten) {
                mWritten = false;

                // Allocate index and buckets for this frame
                auto& buckets = mPool->buckets[mFrameIndex];

                // Bucket index out of pools count
                // Allocate another pool
                if (buckets.IsEmpty()) {
                    AllocateBucketBack(buckets, mPool->maxBuffers, mPool->maxImages);
                }

                auto& last = buckets.Last();

                // Check that current bucket has empty descriptor,
                // otherwise expand buckets list
                if (last.allocated >= last.size) {
                    AllocateBucketBack(buckets, mPool->maxBuffers, mPool->maxImages);
                }

                auto& bucket = buckets.Last();

                // Now we have index of bucket which is 100% suitable
                VkDescriptorSetAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = bucket.pool;
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts = &mLayout;

                BERSERK_VK_CHECK(vkAllocateDescriptorSets(mDevice.GetDevice(), &allocInfo, &mCurrentSet));

                bucket.allocated += 1;
                assert(bucket.allocated <= bucket.size);

                // After allocation we need to reassign dst set for each write info
                for (auto& writeInfo: mWriteInfo) {
                    writeInfo.dstSet = mCurrentSet;
                }
            }
        }

        void VulkanDescriptorSetManager::ReleasePool(VulkanDescriptorSetManager::Pool &pool) {
            for (auto& buckets: pool.buckets) {
                ReleaseBuckets(buckets);
            }

            BERSERK_VK_LOG_INFO(BERSERK_TEXT("Release DescriptorPool: {0}"), &pool);
        }

        void VulkanDescriptorSetManager::ReleaseBuckets(Array<Bucket> &source) {
            auto device = mDevice.GetDevice();

            for (auto& bucket: source) {
                vkDestroyDescriptorPool(device, bucket.pool, nullptr);
            }

            source.Clear();
        }

    }
}