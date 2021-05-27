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

#include <BerserkVulkan/VulkanUtils.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <limits>

namespace Berserk {
    namespace RHI {

        VulkanUtils::VulkanUtils(struct VulkanDevice &device) : mDevice(device) {

        }

        VulkanUtils::~VulkanUtils() {
            auto device = mDevice.GetDevice();

            for (auto fence: mCachedFences)
                vkDestroyFence(device, fence, nullptr);
            for (auto semaphore: mCachedSemaphores)
                vkDestroySemaphore(device, semaphore, nullptr);
        }

        void VulkanUtils::BarrierImage(VkCommandBuffer cmd, VkImage image,
                                       VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                                       VkImageLayout oldLayout, VkImageLayout newLayout,
                                       VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                                       const VkImageSubresourceRange& range) {

            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;
            barrier.subresourceRange = range;
            barrier.srcAccessMask = srcAccessMask;
            barrier.dstAccessMask = dstAccessMask;

            vkCmdPipelineBarrier(cmd,
                                 srcStageMask, dstStageMask, 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &barrier);
        }

        void VulkanUtils::BarrierImage2d(VkCommandBuffer cmd,
                                         VkImage image, VkImageAspectFlags aspect,
                                         VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                                         VkImageLayout oldLayout, VkImageLayout newLayout,
                                         VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {
            VkImageSubresourceRange range{};
            range.aspectMask = aspect;
            range.baseArrayLayer = 0;
            range.layerCount = 1;
            range.baseMipLevel = 0;
            range.levelCount = 1;

            BarrierImage(cmd, image, srcAccessMask, dstAccessMask, oldLayout, newLayout, srcStageMask, dstStageMask, range);
        }

        VkFence VulkanUtils::CreateFence(bool signaled) {
            if (mCachedFences.IsNotEmpty())
                return mCachedFences.PopLast();

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = signaled? VK_FENCE_CREATE_SIGNALED_BIT: 0;

            VkFence fence;

            BERSERK_VK_CHECK(vkCreateFence(mDevice.GetDevice(), &fenceInfo, nullptr, &fence));

            return fence;
        }

        void VulkanUtils::ResetFence(VkFence fence) {
            BERSERK_VK_CHECK(vkResetFences(mDevice.GetDevice(), 1, &fence));
        }

        void VulkanUtils::WaitFence(VkFence fence) {
            auto timeout = std::numeric_limits<uint64>::max();
            BERSERK_VK_CHECK(vkWaitForFences(mDevice.GetDevice(), 1, &fence, VK_TRUE, timeout));
        }

        void VulkanUtils::DestroyFence(VkFence fence) {
            ResetFence(fence);
            mCachedFences.Add(fence);
        }

        VkSemaphore VulkanUtils::CreateSemaphore() {
            if (mCachedSemaphores.IsNotEmpty())
                return mCachedSemaphores.PopLast();

            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            semaphoreInfo.flags = 0;

            VkSemaphore semaphore;

            BERSERK_VK_CHECK(vkCreateSemaphore(mDevice.GetDevice(), &semaphoreInfo, nullptr, &semaphore));

            printf(" semaphore %p\n", semaphore);

            return semaphore;
        }

        void VulkanUtils::DestroySemaphore(VkSemaphore semaphore) {
            mCachedSemaphores.Add(semaphore);
        }


    }
}
