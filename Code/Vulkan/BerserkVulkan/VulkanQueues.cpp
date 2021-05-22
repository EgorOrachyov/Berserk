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

#include <BerserkVulkan/VulkanQueues.hpp>

namespace Berserk {
    namespace RHI {

        VulkanQueues::VulkanQueues(VkPhysicalDevice device, VkSurfaceKHR surface) {
            uint32 queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            mQueueFamilyProperties.Resize(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, mQueueFamilyProperties.GetData());

            uint32 graphicsQueuesCount = 0;
            uint32 transferQueuesCount = 0;

            for (size_t i = 0; i < queueFamilyCount; i++) {
                auto& family = mQueueFamilyProperties[i];

                VkBool32 presentationSupported;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupported);

                if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 &&
                    (family.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0 &&
                    family.queueCount > graphicsQueuesCount &&
                    presentationSupported) {
                    mGraphicsQueueFamilyIndex = i;
                    graphicsQueuesCount = family.queueCount;
                }

                if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 &&
                    (family.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0 &&
                    family.queueCount > transferQueuesCount) {
                    mTransferQueueFamilyIndex = i;
                    transferQueuesCount = family.queueCount;
                }

                if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 &&
                    (family.queueFlags & VK_QUEUE_TRANSFER_BIT) == 0 &&
                    presentationSupported) {
                    mPresentQueueFamilyIndex = i;
                }
            }

            assert(mGraphicsQueueFamilyIndex != INVALID_QUEUE_INDEX);

            /** Fallback */

            if (mTransferQueueFamilyIndex == INVALID_QUEUE_INDEX) {
                mTransferQueueFamilyIndex = mGraphicsQueueFamilyIndex;
            }

            if (mPresentQueueFamilyIndex == INVALID_QUEUE_INDEX) {
                mPresentQueueFamilyIndex = mGraphicsQueueFamilyIndex;
            }
        }

        void VulkanQueues::SetupQueuesFromDevice(VkDevice device) {
            mGraphicsQueues.Resize(mQueueFamilyProperties[mGraphicsQueueFamilyIndex].queueCount);
            mTransferQueues.Resize(mQueueFamilyProperties[mTransferQueueFamilyIndex].queueCount);
            mPresentQueues.Resize(mQueueFamilyProperties[mPresentQueueFamilyIndex].queueCount);

            VkQueue* queuesArrays[] = {
                mGraphicsQueues.GetData(),
                mTransferQueues.GetData(),
                mPresentQueues.GetData()
            };

            uint32 toGet[] = {
                mQueueFamilyProperties[mGraphicsQueueFamilyIndex].queueCount,
                mQueueFamilyProperties[mTransferQueueFamilyIndex].queueCount,
                mQueueFamilyProperties[mPresentQueueFamilyIndex].queueCount
            };

            uint32 familyIndices[] = {
                mGraphicsQueueFamilyIndex,
                mTransferQueueFamilyIndex,
                mPresentQueueFamilyIndex
            };

            size_t count = sizeof(familyIndices) / sizeof(familyIndices[0]);

            for (size_t i = 0; i < count; i++) {
                auto family = familyIndices[i];
                auto array = queuesArrays[i];

                for (size_t j = 0; j < toGet[i]; j++) {
                    VkQueue queue;

                    vkGetDeviceQueue(device, family, j, &queue);
                    array[j] = queue;
                }
            }
        }

        void VulkanQueues::GetQueuesCreateInfos(Array<VkDeviceQueueCreateInfo> &creatInfos, Array<float> &graphicsQueuesPriority, Array<float> &transferQueuesPriority, Array<float> &presentQueuesPriority) const {
            graphicsQueuesPriority.Resize(mQueueFamilyProperties[mGraphicsQueueFamilyIndex].queueCount, mDefaultPriority);

            VkDeviceQueueCreateInfo graphicsQueue{};
            graphicsQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            graphicsQueue.queueFamilyIndex = mGraphicsQueueFamilyIndex;
            graphicsQueue.queueCount = graphicsQueuesPriority.GetSize();
            graphicsQueue.pQueuePriorities = graphicsQueuesPriority.GetData();

            creatInfos.Add(graphicsQueue);

            if (mTransferQueueFamilyIndex != mGraphicsQueueFamilyIndex) {
                transferQueuesPriority.Resize(mQueueFamilyProperties[mTransferQueueFamilyIndex].queueCount, mDefaultPriority);

                VkDeviceQueueCreateInfo transferQueue{};
                transferQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                transferQueue.queueFamilyIndex = mTransferQueueFamilyIndex;
                transferQueue.queueCount = transferQueuesPriority.GetSize();
                transferQueue.pQueuePriorities = transferQueuesPriority.GetData();

                creatInfos.Add(transferQueue);
            }

            if (mPresentQueueFamilyIndex != mTransferQueueFamilyIndex && mPresentQueueFamilyIndex != mGraphicsQueueFamilyIndex) {
                presentQueuesPriority.Resize(mQueueFamilyProperties[mPresentQueueFamilyIndex].queueCount, mDefaultPriority);

                VkDeviceQueueCreateInfo presentQueue{};
                presentQueue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                presentQueue.queueFamilyIndex = mPresentQueueFamilyIndex;
                presentQueue.queueCount = presentQueuesPriority.GetSize();
                presentQueue.pQueuePriorities =  presentQueuesPriority.GetData();

                creatInfos.Add(presentQueue);
            }
        }

        bool VulkanQueues::IsComplete() const {
            return mGraphicsQueueFamilyIndex != INVALID_QUEUE_INDEX &&
                   mTransferQueueFamilyIndex != INVALID_QUEUE_INDEX &&
                   mPresentQueueFamilyIndex != INVALID_QUEUE_INDEX;
        }

        void VulkanQueues::GetUniqueFamilies(uint32 *queues, uint32& count) const {
            count = 0;

            queues[count] = mGraphicsQueueFamilyIndex;
            count += 1;

            if (mTransferQueueFamilyIndex != mGraphicsQueueFamilyIndex) {
                queues[count] = mTransferQueueFamilyIndex;
                count += 1;
            }

            if (mPresentQueueFamilyIndex != mGraphicsQueueFamilyIndex &&
                mPresentQueueFamilyIndex != mTransferQueueFamilyIndex) {
                queues[count] = mPresentQueueFamilyIndex;
                count += 1;
            }
        }

        VkQueue VulkanQueues::FetchNextGraphicsQueue() {
            auto queue = mGraphicsQueues[mCurrentGraphicsQueue];
            mCurrentGraphicsQueue = (mCurrentGraphicsQueue + 1) % mGraphicsQueues.GetSize();
            return queue;
        }

        VkQueue VulkanQueues::FetchNextTransferQueue() {
            auto queue = mTransferQueues[mCurrentTransferQueue];
            mCurrentTransferQueue = (mCurrentTransferQueue + 1) % mTransferQueues.GetSize();
            return queue;
        }

        VkQueue VulkanQueues::FetchNextPresentQueue() {
            auto queue = mPresentQueues[mCurrentPresentQueue];
            mCurrentPresentQueue = (mCurrentPresentQueue + 1) % mPresentQueues.GetSize();
            return queue;
        }

    }
}
