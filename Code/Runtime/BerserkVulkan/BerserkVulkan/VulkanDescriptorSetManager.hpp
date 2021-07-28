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

#ifndef BERSERK_VULKANDESCRIPTORSETMANAGER_HPP
#define BERSERK_VULKANDESCRIPTORSETMANAGER_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkRHI/RHISampler.hpp>
#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <BerserkCore/Templates/HashMap.hpp>
#include <BerserkCore/Templates/HashTable.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Descriptor set manager
         *
         * Allocates descriptor sets, updates it accordingly to the currently
         * bound shader params and pipeline layout, stores them until frame is finished
         * and then returns all descriptors set back to the cache.
         *
         * Provides convenient interface for binding textures, samplers and uniform buffers.
         */
        class VulkanDescriptorSetManager: public VulkanCache {
        public:
            /** Used to increase size of the pool with descriptor sets */
            static const uint32 POOL_ALLOC_BASE = 8;
            static const uint32 POOL_ALLOC_FACTOR = 2;

            explicit VulkanDescriptorSetManager(class VulkanDevice& device, uint32 releaseFrequency = RELEASE_FREQUENCY, uint32 timeToKeep = TIME_TO_KEEP);
            VulkanDescriptorSetManager(const VulkanDescriptorSetManager&) = delete;
            VulkanDescriptorSetManager(VulkanDescriptorSetManager&&) noexcept = delete;
            ~VulkanDescriptorSetManager();

            /** Reset all bound resources and starts new resources binding (must be called when pipeline is changed) */
            void BindLayout(VkDescriptorSetLayout layout, const RefCounted<ProgramMeta>& meta);
            void BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize);
            void BindTexture(const RefCounted<Texture> &texture, uint32 location, uint32 arrayIndex);
            void BindSampler(const RefCounted<Sampler> &sampler, uint32 location, uint32 arrayIndex);

            /** @return Creates new descriptor set for currently bound resources (or returns existing one) */
            VkDescriptorSet GetOrCreateSet();

            /** Notify managers that frame is changed, so used descriptors in prev frames can be reused */
            void NextFrame();

            /** Releases cache accordingly to the settings (must be called every frame) */
            void GC();

        private:
            static const uint32 INVALID_INDEX = 0xffffffff;

            struct Bucket {
                VkDescriptorPool pool;
                uint64 size;
                uint64 allocated;
            };

            struct Pool {
                ArrayFixed<Array<Bucket>, Limits::MAX_FRAMES_IN_FLIGHT> buckets;
                uint32 frameUsed;
                uint32 maxBuffers;
                uint32 maxImages;
            };

            struct BoundObject {
                uint32 objectInfo = INVALID_INDEX;
                uint32 writeInfo = INVALID_INDEX;
            };

            void AllocateBucket(Bucket& bucket, uint64 size, uint32 maxBuffers, uint32 maxImages);
            void AllocateBucketBack(Array<Bucket>& source, uint32 maxBuffers, uint32 maxImages);
            void ReallocateAndMarkFreeBuckets(Array<Bucket> &source, uint32 maxBuffers, uint32 maxImages);
            void InvalidateSet();
            void AllocateSet();
            void ReleasePool(Pool& pool);
            void ReleaseBuckets(Array<Bucket> &source);

        private:
            // Cache of allocated descriptor sets
            HashTable<VkDescriptorSetLayout, Pool> mDescriptorPools;

            // Mapping of locations to indices of write info.
            // For arrays of textures, index of image write info with location L and array index I
            // can be evaluated as (mBoundSamplersBuffers[L].writeInfoBase + I)
            HashMap<uint32, BoundObject> mBoundUniformBuffers;
            HashMap<uint32, BoundObject> mBoundSamplersBuffers;

            // Write infos resized before bind accordingly to the meta of the layout
            Array<VkDescriptorBufferInfo> mBufferWriteInfo;
            Array<VkDescriptorImageInfo> mImageWriteInfo;
            Array<VkWriteDescriptorSet> mWriteInfo;

            // If binding mutated, forces descriptor recreation
            VkDescriptorSet mCurrentSet = VK_NULL_HANDLE;
            VkDescriptorSetLayout mLayout = VK_NULL_HANDLE;
            RefCounted<ProgramMeta> mMeta;
            Pool* mPool = nullptr;
            bool mWritten = false;

            uint32 mFrameIndex = 0;
        };

    }
}

#endif //BERSERK_VULKANDESCRIPTORSETMANAGER_HPP