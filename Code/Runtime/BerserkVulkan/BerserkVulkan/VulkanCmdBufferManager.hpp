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

        /** Manages set of commands for different operations for current frame*/
        class VulkanCmdBufferManager {
        public:
            explicit VulkanCmdBufferManager(class VulkanDevice& device);
            VulkanCmdBufferManager(const VulkanCmdBufferManager&) = delete;
            VulkanCmdBufferManager(VulkanCmdBufferManager&&) noexcept = delete;
            ~VulkanCmdBufferManager();

            void BeginFrame(uint32 frameId);
            void BeginScene();
            void EndScene(class VulkanSurface &surface);
            void EndScene(VkSemaphore signal = VK_NULL_HANDLE);
            void EndFrame();
            void AcquireImage(class VulkanSurface& surface);

            VkCommandBuffer GetGraphicsCmdBuffer();
            VkCommandBuffer GetUploadCmdBuffer();
            VkCommandBuffer GetAsyncTransferCmdBuffer();

        private:
            static void Submit(VkQueue queue, VkCommandBuffer buffer, VkSemaphore wait, VkSemaphore signal, VkPipelineStageFlags waitMask, VkFence fence);
            static void Submit(VkQueue queue, VkCommandBuffer buffer, uint32 waitCount, VkSemaphore *wait, const VkPipelineStageFlags *waitMask, VkSemaphore signal, VkFence fence);
            static void Submit(VkQueue queue, VkCommandBuffer buffer);
            static void Submit(VkQueue queue, VkCommandBuffer buffer, VkFence fence);

            VkSemaphore GetSemaphore();
            VkFence GetFence();
            void WaitForPrevFrame();

        private:
            ArrayFixed<Array<VkFence>, Limits::MAX_FRAMES_IN_FLIGHT> mFramesToWait;
            ArrayFixed<Array<VkSemaphore>, Limits::MAX_FRAMES_IN_FLIGHT> mUsedSemaphores;
            Array<VkSemaphore> mWait;
            Array<VkPipelineStageFlags> mWaitMask;

            VkCommandBuffer mGraphics = nullptr;
            VkCommandBuffer mUpload = nullptr;
            VkCommandBuffer mAsyncTransfer = nullptr;

            uint32 mCurrentFrame = 1;
            uint32 mFrameIndex = 1;
            uint32 mPrevFrameIndex = 0;

            class VulkanDevice& mDevice;
            class VulkanCmdBufferPool& mPool;
        };

    }
}




#endif //BERSERK_VULKANCMDBUFFERMANAGER_HPP
