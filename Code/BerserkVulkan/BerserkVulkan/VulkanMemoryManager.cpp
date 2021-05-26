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

        VulkanMemoryManager::VulkanMemoryManager(struct VulkanDevice &device)
                : mDevice(device), mQueues(*device.GetQueues()), mPhysicalDevice(*device.GetPhysicalDevice()) {

            VmaAllocatorCreateInfo allocatorInfo{};
            allocatorInfo.flags =
                    VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT | // Will be accessed only from single thread
                    VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
            allocatorInfo.instance = device.GetInstance();
            allocatorInfo.device = device.GetDevice();
            allocatorInfo.physicalDevice = mPhysicalDevice.Get();
            allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
            allocatorInfo.frameInUseCount = 0;

            BERSERK_VK_CHECK(vmaCreateAllocator(&allocatorInfo, &mVmaAllocator));

            // Get space for pending release buffers
            mPendingRelease.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
        }

        VulkanMemoryManager::~VulkanMemoryManager() {
            assert(mAllocCalls == mDeallocCalls);

            if (mVmaAllocator) {
                auto frames = Limits::MAX_FRAMES_IN_FLIGHT;

                for (size_t i = 0; i < frames; i++) {
                    for (auto & allocation: mPendingRelease[i]) {
                        vmaDestroyBuffer(mVmaAllocator, allocation.buffer, allocation.allocation);
                    }
                }

                vmaDestroyAllocator(mVmaAllocator);
                mVmaAllocator = nullptr;
            }
        }

        void VulkanMemoryManager::NextFrame(uint32 frameIndex) {
            mCurrentFrameIndex = frameIndex;
            mFetchIndex = mCurrentFrameIndex % Limits::MAX_FRAMES_IN_FLIGHT;

            // Release all pending buffers
            // Now this slot is used to enqueue release in this frame
            for (auto& allocation: mPendingRelease[mFetchIndex]) {
                vmaDestroyBuffer(mVmaAllocator, allocation.buffer, allocation.allocation);
            }

            mPendingRelease[mFetchIndex].Clear();
        }

        VulkanMemoryManager::Allocation VulkanMemoryManager::AllocateBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags) {
            auto& queueFamilyIndices = mQueues.GetUniqueFamilies();

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usageFlags;
            bufferInfo.sharingMode = mQueues.GetResourcesSharingMode();
            bufferInfo.queueFamilyIndexCount = queueFamilyIndices.GetSize();
            bufferInfo.pQueueFamilyIndices = queueFamilyIndices.GetData();

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.requiredFlags = memoryPropertyFlags;
            allocInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

            VkBuffer buffer;
            VmaAllocation allocation;

            BERSERK_VK_CHECK(vmaCreateBuffer(mVmaAllocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr));

            Allocation info{};
            info.buffer = buffer;
            info.allocation = allocation;

            mAllocCalls += 1;

            return info;
        }

        void VulkanMemoryManager::DeallocateBuffer(VulkanMemoryManager::Allocation allocation) {
            mPendingRelease[mFetchIndex].Add(allocation);
            mDeallocCalls += 1;
        }

    }
}