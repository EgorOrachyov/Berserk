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

#ifndef BERSERK_VULKANMEMORYMANAGER_HPP
#define BERSERK_VULKANMEMORYMANAGER_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <vk_mem_alloc.h>

namespace Berserk {
    namespace RHI {

        /** Wrapper for VMA library with additional functionality for vulkan memory management */
        class VulkanMemoryManager {
        public:

            struct BufferAllocation {
                VkBuffer buffer;
                VmaAllocation allocation;
            };

            struct ImageAllocation {
                VkImage image;
                VmaAllocation allocation;
            };

            explicit VulkanMemoryManager(class VulkanDevice& device);
            VulkanMemoryManager(const VulkanMemoryManager&) = delete;
            VulkanMemoryManager(VulkanMemoryManager&&) noexcept = delete;
            ~VulkanMemoryManager();

            /**
             * Must be called each frame to advance manager and release now unused resources.
             * @warning It is up to the user to ensure, that earliest frame in the queue is fully finished.
             */
            void NextFrame(uint32 frameIndex);

            /**
             * Allocate buffer with specified size and usage settings
             *
             * @param size Size in bytes of the buffer
             * @param usageFlags Buffer intended usage (vertex, index, uniform and etc.)
             * @param memoryPropertyFlags Properties of the memory for this buffer
             *
             * @return Allocation information (required to later release buffer)
             */
            BufferAllocation AllocateBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags);

            /**
             * Allocate image with specified settings
             *
             * @param imageInfo Info of the image to create
             * @param memoryPropertyFlags Properties of the memory for this image
             *
             * @return Allocation information (required to later release image)
             */
            ImageAllocation AllocateImage(VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags memoryPropertyFlags);

            /** Release allocated buffer */
            void DeallocateBuffer(BufferAllocation allocation);

            /** Release allocated image */
            void DeallocateImage(ImageAllocation allocation);

            /** @return Global vma allocator instance */
            VmaAllocator GetVmaAllocator() const { return mVmaAllocator; }

        private:
            // Buffer that must be released. Store here for some time here before actual release,
            // since this buffers may be used in the rendering of previous frames
            ArrayFixed<Array<BufferAllocation>, Limits::MAX_FRAMES_IN_FLIGHT> mPendingReleaseBfs;
            ArrayFixed<Array<ImageAllocation>, Limits::MAX_FRAMES_IN_FLIGHT> mPendingReleaseImg;

            VmaAllocator mVmaAllocator = nullptr;

            uint32 mCurrentFrameIndex = 0;
            uint32 mFetchIndex = 0;             // mCurrentFrameIndex % framesInFlight

            size_t mAllocCalls = 0;
            size_t mDeallocCalls = 0;

            class VulkanDevice& mDevice;
            class VulkanQueues& mQueues;
            class VulkanPhysicalDevice& mPhysicalDevice;
        };

    }
}

#endif //BERSERK_VULKANMEMORYMANAGER_HPP