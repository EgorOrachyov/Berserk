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

#ifndef BERSERK_VULKANUTILS_HPP
#define BERSERK_VULKANUTILS_HPP

#include <BerserkVulkan/VulkanDefs.hpp>

namespace Berserk {
    namespace RHI {

        class VulkanUtils {
        public:
            explicit VulkanUtils(class VulkanDevice& device);
            VulkanUtils(const VulkanUtils&) = delete;
            VulkanUtils(VulkanUtils&&) noexcept = delete;
            ~VulkanUtils();

            void BarrierImage(VkCommandBuffer cmd,
                              VkImage image,
                              VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                              VkImageLayout oldLayout, VkImageLayout newLayout,
                              VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                              const VkImageSubresourceRange& range);

            void BarrierImage2d(VkCommandBuffer cmd,
                                VkImage image, VkImageAspectFlags aspect,
                                VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                                VkImageLayout oldLayout, VkImageLayout newLayout,
                                VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask);

            VkFence CreateFence(bool signaled = false);
            void ResetFence(VkFence fence);
            void WaitFence(VkFence fence);
            void DestroyFence(VkFence fence);

            VkSemaphore CreateSemaphore();
            void DestroySemaphore(VkSemaphore semaphore);

        private:
            Array<VkFence> mCachedFences;
            Array<VkSemaphore> mCachedSemaphores;
            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANUTILS_HPP