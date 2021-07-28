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

#include <BerserkVulkan/VulkanStagePool.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanMemoryManager.hpp>

namespace Berserk {
    namespace RHI {

        VulkanStagePool::VulkanStagePool(VulkanDevice &device)
                : mDevice(device), mMemMan(*device.GetMemoryManager()) {

            // Prepare structures, their size won't be changed
            auto frames = Limits::MAX_FRAMES_IN_FLIGHT;

            mBuckets.Resize(frames);
            mMemoryPools.Resize(frames);

            // Create pools and buckets maps
            CreatePools();
        }

        VulkanStagePool::~VulkanStagePool() {
            ReleasePools();
        }

        void VulkanStagePool::NextFrame() {
            mCurrentFrameIndex += 1;
            mFetchIndex = mCurrentFrameIndex % Limits::MAX_FRAMES_IN_FLIGHT;

            for (auto& entry: mBuckets[mFetchIndex]) {
                // Reset used buffers
                entry.GetSecond().nextToAllocate = 0;
            }
        }

        void VulkanStagePool::RecreateCurrentPool() {
            ReleasePool(mFetchIndex);
            CreatePool(mFetchIndex);
        }

        VulkanStagePool::Allocation VulkanStagePool::AllocateBuffer(VkDeviceSize size) {
            auto aligned = FindSuitable(size);
            auto index = mFetchIndex;

            auto bucketPtr = mBuckets[index].GetPtr(aligned);

            if (bucketPtr == nullptr) {
                // We need to emplace bucket for this size of memory
                StageBucket bucket;
                bucket.nextToAllocate = 0;

                bucketPtr = &mBuckets[index].Move(aligned, bucket);
            }

            return Allocate(aligned, mMemoryPools[index], *bucketPtr);
        }

        void VulkanStagePool::CreatePool(uint64 index) {
            // Staging buffers used as transfer source only
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = mInitialPoolSize;
            bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

            // Staging buffers are host resident (memory for PCIe transfer CPU -> GPU)
            VmaAllocationCreateInfo allocCreateInfo{};
            allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

            uint32 memTypeIndex;
            vmaFindMemoryTypeIndexForBufferInfo(mMemMan.GetVmaAllocator(), &bufferInfo, &allocCreateInfo, &memTypeIndex);

            VmaPoolCreateInfo poolCreateInfo{};
            poolCreateInfo.memoryTypeIndex = memTypeIndex;
            poolCreateInfo.flags = VMA_POOL_CREATE_LINEAR_ALGORITHM_BIT; // We will never deallocate allocated blocks
            poolCreateInfo.blockSize = 0;
            poolCreateInfo.minBlockCount = 0;
            poolCreateInfo.maxBlockCount = 0;
            poolCreateInfo.frameInUseCount = 0;

            BERSERK_VK_CHECK(vmaCreatePool(mMemMan.GetVmaAllocator(), &poolCreateInfo, &mMemoryPools[index]));
        }

        void VulkanStagePool::ReleasePool(uint64 index) {
            auto& pool = mBuckets[index];
            auto vmaAllocator = mMemMan.GetVmaAllocator();

            for (auto& entry: pool) {
                auto& bucket = entry.GetSecond();

                for (auto& allocation: bucket.allocations) {
                    auto buffer = allocation.buffer;
                    auto alloc = allocation.allocation;

                    vmaDestroyBuffer(vmaAllocator, buffer, alloc);
                }

                bucket.allocations.Clear();
                bucket.nextToAllocate = 0;
            }

            vmaDestroyPool(vmaAllocator, mMemoryPools[index]);
        }

        void VulkanStagePool::CreatePools() {
            auto frames = Limits::MAX_FRAMES_IN_FLIGHT;

            for (uint64 i = 0; i < frames; i++)
                CreatePool(i);
        }

        void VulkanStagePool::ReleasePools() {
            auto frames = Limits::MAX_FRAMES_IN_FLIGHT;

            for (uint64 i = 0; i < frames; i++)
                ReleasePool(i);
        }

        uint64 VulkanStagePool::FindSuitable(uint64 requested) const {
            auto size = mInitialPoolSize;

            while (size < requested)
                size *= mAllocFactor;

            return size;
        }

        VulkanStagePool::Allocation VulkanStagePool::Allocate(uint64 size, VmaPool &pool, VulkanStagePool::StageBucket &bucket) {
            if (bucket.nextToAllocate < bucket.allocations.GetSize()) {
                auto alloc = bucket.allocations[bucket.nextToAllocate];
                bucket.nextToAllocate += 1;

                return alloc;
            }

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.pool = pool;
            allocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

            VkBuffer buffer;
            VmaAllocation allocation;

            vmaCreateBuffer(mMemMan.GetVmaAllocator(), &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);

            Allocation object{};
            object.buffer = buffer;
            object.allocation = allocation;

            bucket.allocations.Add(object);
            bucket.nextToAllocate += 1;

            mTotalAllocated += 1;

            return object;
        }
    }
}