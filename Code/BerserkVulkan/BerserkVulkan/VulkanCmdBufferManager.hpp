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

#ifndef BERSERK_VULKANCMDBUFFERMANAGER_HPP
#define BERSERK_VULKANCMDBUFFERMANAGER_HPP

#include <BerserkVulkan/VulkanDefs.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Command buffers manager
         *
         * Manages command buffers allocations for different queue families,
         * for each frame in flight allocates buffers independently, so
         * if frames overlap no corruption between buffers.
         *
         * @note Currently max frames in flight is defined by Limits::MAX_FRAMES_IN_FLIGHT.
         *       Maybe it will be removed for dynamic configuration at runtime.
         */
        class VulkanCmdBufferManager {
        public:
            /** When allocate more buffers, size is defined as prev * factor */
            static const size_t ALLOCATION_FACTOR = 2;
            /** When pool is first time allocated, fetch 16 command buffers */
            static const size_t INITIAL_POOL_SIZE = 16;

            explicit VulkanCmdBufferManager(class VulkanDevice& device, size_t allocFactor = ALLOCATION_FACTOR);
            VulkanCmdBufferManager(const VulkanCmdBufferManager&) = delete;
            VulkanCmdBufferManager(VulkanCmdBufferManager&&) noexcept = delete;
            ~VulkanCmdBufferManager();

            /**
             * Must be called each frame to advance pools and release finished.
             * @warning It is up to the user to ensure, that earliest frame in the queue is fully finished.
             */
            void NextFrame(uint32 frameIndex);

            /** @return Starts new graphics commands buffer */
            VkCommandBuffer StartGraphicsCmd();

            /** @return Starts new transfer commands buffer */
            VkCommandBuffer StartTransferCmd();

            /** @return Starts new presentation commands buffer */
            VkCommandBuffer StartPresentCmd();

            /**
             * Submit cmd buffer to the queue.
             * @warning It is up to the user to check, that queue matches buffer queue family.
             */
            void Submit(VkQueue queue, VkCommandBuffer buffer, VkSemaphore wait, VkSemaphore signal,
                        VkPipelineStageFlags waitMask, VkFence fence);
            void Submit(VkQueue queue, VkCommandBuffer buffer);
            void Submit(VkQueue queue, VkCommandBuffer buffer, VkFence fence);

        private:
            struct Pool {
                Array<VkCommandBuffer> cached;  // All commands, which are allocated from pool
                VkCommandPool pool;             // Pool for commands allocation
                uint32 nextToAllocate = 0;      // Shows which cmb buffer to fetch on next allocation
            };

            void ExpandPool(Pool& pool);
            VkCommandBuffer StartBuffer(Pool& pool);

        private:
            ArrayFixed<Pool, Limits::MAX_FRAMES_IN_FLIGHT> mGraphics;
            ArrayFixed<Pool, Limits::MAX_FRAMES_IN_FLIGHT> mTransfer;
            ArrayFixed<Pool, Limits::MAX_FRAMES_IN_FLIGHT> mPresent;

            size_t mAllocFactor;
            size_t mTotalAllocated = 0;

            uint32 mCurrentFrameIndex = 0;
            uint32 mFetchIndex = 0;             // mCurrentFrameIndex % framesInFlight

            class VulkanDevice& mDevice;
            class VulkanQueues& mQueues;
        };

    }
}

#endif //BERSERK_VULKANCMDBUFFERMANAGER_HPP