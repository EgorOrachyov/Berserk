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

#ifndef BERSERK_VULKANSTAGEPOOL_HPP
#define BERSERK_VULKANSTAGEPOOL_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <vk_mem_alloc.h>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Staging pool
         *
         * Pool of CPU memory blocks and vulkan buffers, used to transfer data from CPU to GPU.
         * Pool manages set of buffers for each potential frame in flight, so when frame is finished
         * all staging buffers are reused and marked as free at once;
         */
        class VulkanStagePool {
        public:
            /** When allocate more buffers, size is defined as prev * factor */
            static const size_t ALLOCATION_FACTOR = 2;
            /** When first bucket created this value defines size of its staging buffers in bytes */
            static const size_t INITIAL_POOL_SIZE = 256;

            struct Allocation {
                VkBuffer buffer;                        // Temporary staging buffer
                VmaAllocation allocation;               // Corresponding memory allocation
            };

            explicit VulkanStagePool(class VulkanDevice& device);
            VulkanStagePool(const VulkanStagePool&) = delete;
            VulkanStagePool(VulkanStagePool&&) noexcept = delete;
            ~VulkanStagePool();

            /**
             * Must be called each frame to advance pools and release finished.
             * @warning It is up to the user to ensure, that earliest frame in the queue is fully finished.
             */
            void NextFrame(uint32 frameIndex);

            /**
             * Recreates current pool for this frame.
             * This function can be called to release all cached staging buffer in pool fo current frame
             * inorder to free potentially no more used buffers, which are kept by allocator.
             *
             * It is quite handy to decrease memory footprint as well as recreate entire pool to
             * reduce fragmentation.
             *
             * @note Call this function as soon as application memory profile is changed
             * @warning This function can be called only after NextFrame() before any staging allocation
             */
            void RecreateCurrentPool();

            /**
             * Allocates staging buffer of specified size
             *
             * @param size Size in bytes of the buffer
             * @return Allocation info (buffer and its vma alloc info)
             */
            Allocation AllocateBuffer(VkDeviceSize size);

        private:

            struct StageBucket {
                size_t nextToAllocate;
                Array<Allocation> allocations;
            };

            void CreatePool(size_t index);
            void ReleasePool(size_t index);
            void CreatePools();
            void ReleasePools();
            size_t FindSuitable(size_t requested) const;
            Allocation Allocate(size_t size, VmaPool& pool, StageBucket& bucket);

        private:
            using PoolType = HashMap<size_t, StageBucket>;

            // For each frame we have a mapping of stage size to its bucket with actual memory
            // Since stages are not released, when frame is finished, mark all buffers as empty
            ArrayFixed<PoolType, Limits::MAX_FRAMES_IN_FLIGHT> mBuckets;
            ArrayFixed<VmaPool, Limits::MAX_FRAMES_IN_FLIGHT> mMemoryPools;

            size_t mAllocFactor = ALLOCATION_FACTOR;
            size_t mInitialPoolSize = INITIAL_POOL_SIZE;
            size_t mTotalAllocated = 0;

            uint32 mCurrentFrameIndex = 0;
            uint32 mFetchIndex = 0;             // mCurrentFrameIndex % framesInFlight

            class VulkanDevice& mDevice;
            class VulkanMemoryManager& mMemMan;
        };

    }
}

#endif //BERSERK_VULKANSTAGEPOOL_HPP