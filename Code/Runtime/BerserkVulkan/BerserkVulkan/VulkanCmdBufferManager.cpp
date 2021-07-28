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

#include <BerserkVulkan/VulkanCmdBufferManager.hpp>
#include <BerserkVulkan/VulkanCmdBufferPool.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanSurface.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>

namespace Berserk {
    namespace RHI {

        VulkanCmdBufferManager::VulkanCmdBufferManager(VulkanDevice &device)
                : mDevice(device), mPool(*device.GetCmdBufferPool()) {
            mFramesToWait.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
            mUsedSemaphores.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
        }

        VulkanCmdBufferManager::~VulkanCmdBufferManager() {
            auto& utils = *mDevice.GetUtils();

            for (uint64 i = 0; i < Limits::MAX_FRAMES_IN_FLIGHT; i++)  {
                for (auto fence: mFramesToWait[i])
                    utils.DestroyFence(fence);
                for (auto semaphore: mUsedSemaphores[i])
                    utils.DestroySemaphore(semaphore);
            }
        }

        void VulkanCmdBufferManager::BeginFrame(uint32 frameId) {
            // Advance frame, get index of the frame to draw/use cached data
            mCurrentFrame = frameId;
            mPrevFrameIndex = mFrameIndex;
            mFrameIndex = (mCurrentFrame) % Limits::MAX_FRAMES_IN_FLIGHT;

            auto& utils = *mDevice.GetUtils();

            // Release operations synchronization semaphores
            for (auto semaphore: mUsedSemaphores[mFrameIndex])
                utils.DestroySemaphore(semaphore);

            mUsedSemaphores[mFrameIndex].Clear();

            if (mUpload) {
                auto& queues = *mDevice.GetQueues();

                auto signal = GetSemaphore();
                auto stage = VK_PIPELINE_STAGE_TRANSFER_BIT;

                Submit(queues.FetchNextGraphicsQueue(), mUpload, VK_NULL_HANDLE, signal, 0, GetFence());

                mWait.Add(signal);
                mWaitMask.Add(stage);
                mUpload = nullptr;
            }
        }

        void VulkanCmdBufferManager::BeginScene() {
            // No special actions
            assert(mUpload == nullptr);
        }

        void VulkanCmdBufferManager::EndScene(class VulkanSurface &surface) {
            // Transition layout to presentation
            surface.TransitionLayoutBeforePresentation(mGraphics);

            // Signal this semaphore when graphics is ready
            VkSemaphore signal = GetSemaphore();
            EndScene(signal);

            auto& queues = *mDevice.GetQueues();

            auto swapchain = surface.GetSwapchain();
            auto imageIndex = surface.GetImageIndexToDraw();

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &signal;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr;

            auto result = vkQueuePresentKHR(queues.FetchNextPresentQueue(), &presentInfo);

            // Notify, that we presented image into surface of currently bound window
            surface.NotifyPresented();

            // Handle resize/recreation of the swap chain
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
                surface.Recreate();
            } else if (result != VK_SUCCESS) {
                BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to present surface {0}"), surface.GetName());
            }
        }

        void VulkanCmdBufferManager::EndScene(VkSemaphore signal) {
            assert(mUpload == nullptr);

            WaitForPrevFrame();

            auto& queues = *mDevice.GetQueues();

            // Final transition after presentation
            auto waitCount = static_cast<uint32>(mWait.GetSize());
            VkSemaphore* wait = mWait.GetData();
            VkPipelineStageFlags* waitMask = mWaitMask.GetData();
            Submit(queues.FetchNextGraphicsQueue(), mGraphics, waitCount, wait, waitMask, signal, GetFence());

            mGraphics = nullptr;
            mWait.Clear();
            mWaitMask.Clear();
        }

        void VulkanCmdBufferManager::EndFrame() {
            WaitForPrevFrame();

            auto& queues = *mDevice.GetQueues();

            if (mGraphics) {
                Submit(queues.FetchNextGraphicsQueue(), mGraphics, GetFence());
                mGraphics = nullptr;
            }

            if (mUpload) {
                Submit(queues.FetchNextGraphicsQueue(), mUpload, GetFence());
                mUpload = nullptr;
            }
        }

        void VulkanCmdBufferManager::AcquireImage(VulkanSurface &surface) {
            auto signal = GetSemaphore();
            surface.AcquireNextImage(signal);

            // Add sync for this scope dependency
            mWait.Add(signal);
            mWaitMask.Add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        }

        VkCommandBuffer VulkanCmdBufferManager::GetGraphicsCmdBuffer() {
            if (!mGraphics)
                mGraphics = mPool.StartGraphicsCmd();

            return mGraphics;
        }

        VkCommandBuffer VulkanCmdBufferManager::GetUploadCmdBuffer() {
            if (!mUpload)
                mUpload = mPool.StartGraphicsCmd();

            return mUpload;
        }

        VkCommandBuffer VulkanCmdBufferManager::GetAsyncTransferCmdBuffer() {
            if (!mAsyncTransfer)
                mAsyncTransfer = mPool.StartTransferCmd();

            return mAsyncTransfer;
        }

        void VulkanCmdBufferManager::Submit(VkQueue queue, VkCommandBuffer buffer, VkSemaphore wait, VkSemaphore signal, VkPipelineStageFlags waitMask, VkFence fence) {
            ArrayFixed<VkSemaphore, 1> waitSemaphores;
            ArrayFixed<VkSemaphore, 1> signalSemaphores;

            if (wait)
                waitSemaphores.Add(wait);

            if (signal)
                signalSemaphores.Add(signal);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = static_cast<uint32>(waitSemaphores.GetSize());
            submitInfo.pWaitSemaphores = waitSemaphores.GetData();
            submitInfo.pWaitDstStageMask = &waitMask;
            submitInfo.signalSemaphoreCount = static_cast<uint32>(signalSemaphores.GetSize());
            submitInfo.pSignalSemaphores = signalSemaphores.GetData();
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &buffer;

            BERSERK_VK_CHECK(vkEndCommandBuffer(buffer));
            BERSERK_VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, fence));
        }

        void VulkanCmdBufferManager::Submit(VkQueue queue, VkCommandBuffer buffer, uint32 waitCount, VkSemaphore *wait,
                                         const VkPipelineStageFlags *waitMask, VkSemaphore signal, VkFence fence) {

            ArrayFixed<VkSemaphore, 1> signalSemaphores;

            if (signal)
                signalSemaphores.Add(signal);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.waitSemaphoreCount = waitCount;
            submitInfo.pWaitSemaphores = wait;
            submitInfo.pWaitDstStageMask = waitMask;
            submitInfo.signalSemaphoreCount = static_cast<uint32>(signalSemaphores.GetSize());
            submitInfo.pSignalSemaphores = signalSemaphores.GetData();
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &buffer;

            BERSERK_VK_CHECK(vkEndCommandBuffer(buffer));
            BERSERK_VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, fence));
        }

        void VulkanCmdBufferManager::Submit(VkQueue queue, VkCommandBuffer buffer) {
            Submit(queue, buffer, VK_NULL_HANDLE, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);
        }

        void VulkanCmdBufferManager::Submit(VkQueue queue, VkCommandBuffer buffer, VkFence fence) {
            Submit(queue, buffer, VK_NULL_HANDLE, VK_NULL_HANDLE, 0, fence);
        }

        VkSemaphore VulkanCmdBufferManager::GetSemaphore() {
            auto semaphore =  mDevice.GetUtils()->CreateSemaphore();
            mUsedSemaphores[mFrameIndex].Add(semaphore);
            return semaphore;
        }

        VkFence VulkanCmdBufferManager::GetFence() {
            auto fence = mDevice.GetUtils()->CreateFence(false);
            mFramesToWait[mFrameIndex].Add(fence);
            return fence;
        }

        void VulkanCmdBufferManager::WaitForPrevFrame() {
            auto& sync = mFramesToWait[mPrevFrameIndex];

            // Wait for previse frame before new submission
            if (sync.GetSize() > 0) {
                auto& utils = *mDevice.GetUtils();

                for (auto fence: sync) {
                    utils.WaitFence(fence);
                    utils.DestroyFence(fence);
                }

                sync.Clear();
            }
        }

    }
}