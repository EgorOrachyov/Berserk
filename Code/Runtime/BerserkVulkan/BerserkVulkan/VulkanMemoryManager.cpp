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

#define VMA_IMPLEMENTATION
#include <BerserkVulkan/VulkanMemoryManager.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanPhysicalDevice.hpp>

namespace Berserk {
    namespace RHI {

        VulkanMemoryManager::VulkanMemoryManager(VulkanDevice &device)
                : mDevice(device), mQueues(*device.GetQueues()), mPhysicalDevice(*device.GetPhysicalDevice()) {

            VmaAllocatorCreateInfo allocatorInfo{};
            allocatorInfo.flags =
                    VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT | // Will be accessed only from single thread
                    VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
            allocatorInfo.instance = device.GetInstance();
            allocatorInfo.device = device.GetDevice();
            allocatorInfo.physicalDevice = mPhysicalDevice.Get();
            allocatorInfo.vulkanApiVersion = VULKAN_VERSION;
            allocatorInfo.frameInUseCount = 0;

            BERSERK_VK_CHECK(vmaCreateAllocator(&allocatorInfo, &mVmaAllocator));

            // Get space for pending release buffers
            mPendingReleaseBfs.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
            mPendingReleaseImg.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
        }

        VulkanMemoryManager::~VulkanMemoryManager() {
            assert(mAllocCalls == mDeallocCalls);

            if (mVmaAllocator) {
                auto frames = Limits::MAX_FRAMES_IN_FLIGHT;

                for (uint64 i = 0; i < frames; i++) {
                    for (auto & allocation: mPendingReleaseBfs[i]) {
                        vmaDestroyBuffer(mVmaAllocator, allocation.buffer, allocation.allocation);
                    }

                    for (auto & allocation: mPendingReleaseImg[i]) {
                        vmaDestroyImage(mVmaAllocator, allocation.image, allocation.allocation);
                    }
                }

                vmaDestroyAllocator(mVmaAllocator);
                mVmaAllocator = nullptr;
            }
        }

        void VulkanMemoryManager::NextFrame() {
            mCurrentFrameIndex += 1;
            mFetchIndex = mCurrentFrameIndex % Limits::MAX_FRAMES_IN_FLIGHT;

            // Release all pending buffers/images
            // Now this slot is used to enqueue release in this frame

            for (auto& allocation: mPendingReleaseBfs[mFetchIndex]) {
                vmaDestroyBuffer(mVmaAllocator, allocation.buffer, allocation.allocation);
            }
            for (auto& allocation: mPendingReleaseImg[mFetchIndex]) {
                vmaDestroyImage(mVmaAllocator, allocation.image, allocation.allocation);
            }

            mPendingReleaseBfs[mFetchIndex].Clear();
            mPendingReleaseImg[mFetchIndex].Clear();
        }

        VulkanMemoryManager::BufferAllocation VulkanMemoryManager::AllocateBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
            auto& queueFamilyIndices = mQueues.GetUniqueFamilies();

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usageFlags;
            bufferInfo.sharingMode = mQueues.GetResourcesSharingMode();
            bufferInfo.queueFamilyIndexCount = static_cast<uint32>(queueFamilyIndices.GetSize());
            bufferInfo.pQueueFamilyIndices = queueFamilyIndices.GetData();

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.requiredFlags = memoryPropertyFlags;
            allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

            VkBuffer buffer;
            VmaAllocation allocation;

            BERSERK_VK_CHECK(vmaCreateBuffer(mVmaAllocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr));

            BufferAllocation info{};
            info.buffer = buffer;
            info.allocation = allocation;

            mAllocCalls += 1;

            return info;
        }

        VulkanMemoryManager::ImageAllocation VulkanMemoryManager::AllocateImage(VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags memoryPropertyFlags) {
            VmaAllocationCreateInfo allocInfo{};
            allocInfo.requiredFlags = memoryPropertyFlags;
            allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

            VkImage image;
            VmaAllocation allocation;

            BERSERK_VK_CHECK(vmaCreateImage(mVmaAllocator, &imageInfo, &allocInfo, &image, &allocation, nullptr));

            ImageAllocation info{};
            info.image = image;
            info.allocation = allocation;

            mAllocCalls += 1;

            return info;
        }

        void VulkanMemoryManager::DeallocateBuffer(VulkanMemoryManager::BufferAllocation allocation) {
            mPendingReleaseBfs[mFetchIndex].Add(allocation);
            mDeallocCalls += 1;
        }

        void VulkanMemoryManager::DeallocateImage(VulkanMemoryManager::ImageAllocation allocation) {
            mPendingReleaseImg[mFetchIndex].Add(allocation);
            mDeallocCalls += 1;
        }


    }
}