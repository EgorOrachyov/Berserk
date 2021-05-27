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

#include <BerserkVulkan/VulkanContext.hpp>
#include <BerserkVulkan/VulkanVertexBuffer.hpp>
#include <BerserkVulkan/VulkanIndexBuffer.hpp>
#include <BerserkVulkan/VulkanUniformBuffer.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanSurfaceManager.hpp>
#include <BerserkVulkan/VulkanCmdBufferManager.hpp>

namespace Berserk {
    namespace RHI {


        VulkanContext::VulkanContext(struct VulkanDevice &device)
                : mDevice(device),
                  mQueues(*device.GetQueues()),
                  mSurfaceManager(*device.GetSurfaceManager()),
                  mCmdBufferManager(*device.GetCmdBufferManager()) {

            mFramesToWait.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
            mFramesSync.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
            mPipelineCache = SharedPtr<VulkanPipelineCache>::Make(mDevice);
            mFboCache = SharedPtr<VulkanFramebufferCache>::Make(mDevice);
        }

        VulkanContext::~VulkanContext() {
            mDevice.WaitDeviceIdle();

            auto& utils = *mDevice.GetUtils();

            for (size_t i = 0; i < Limits::MAX_FRAMES_IN_FLIGHT; i++)  {
                for (auto fence: mFramesToWait[i])
                    utils.DestroyFence(fence);
                for (auto semaphore: mFramesSync[i])
                    utils.DestroySemaphore(semaphore);
            }

            mFboCache = nullptr;
            mPipelineCache = nullptr;
        }

        void VulkanContext::BeginFrame() {
            // Advance frame, get index of the frame to draw/use cached data
            mCurrentFrame += 1;
            mFetchIndex = mCurrentFrame % Limits::MAX_FRAMES_IN_FLIGHT;

            // Wait for frame before invalidate any data
            WaitAndReleaseFences(mFramesToWait[mFetchIndex]);

            // Release operations synchronization semaphores
            ReleaseSemaphores(mFramesSync[mFetchIndex]);

            mDevice.NextFrame(mCurrentFrame);
        }

        void VulkanContext::EndFrame() {
            mPipelineCache->GC();
            mFboCache->GC();
        }

        void VulkanContext::BeginScene() {
            // todo something useful
        }

        void VulkanContext::BeginParallel() {

        }

        void VulkanContext::EndParallel() {

        }

        void VulkanContext::BeginSequence() {

        }

        void VulkanContext::EndSequence() {

        }

        void VulkanContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            auto native = (VulkanVertexBuffer*) buffer.Get();
            assert(native);

            // todo: remove in the future, only for test
            auto tmp = mCmdBufferManager.StartTransferCmd();
            native->Update(tmp, byteOffset, byteSize, memory);
            mCmdBufferManager.Submit(mQueues.FetchNextTransferQueue(), tmp);
            mDevice.WaitDeviceIdle();
        }

        void VulkanContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            auto native = (VulkanIndexBuffer*) buffer.Get();
            assert(native);

            // todo: remove in the future, only for test
            auto tmp = mCmdBufferManager.StartTransferCmd();
            native->Update(tmp, byteOffset, byteSize, memory);
            mCmdBufferManager.Submit(mQueues.FetchNextTransferQueue(), tmp);
            mDevice.WaitDeviceIdle();
        }

        void VulkanContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            auto native = (VulkanUniformBuffer*) buffer.Get();
            assert(native);

            // todo: remove in the future, only for test
            auto tmp = mCmdBufferManager.StartTransferCmd();
            native->Update(tmp, byteOffset, byteSize, memory);
            mCmdBufferManager.Submit(mQueues.FetchNextTransferQueue(), tmp);
            mDevice.WaitDeviceIdle();
        }

        void VulkanContext::UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) {
            assert(false);
        }

        void VulkanContext::UpdateTexture2DArray(const RefCounted<Texture> &texture, uint32 arrayIndex, uint32 mipLevel,
                                                 const Math::Rect2u &region, const PixelData &memory) {
            assert(false);
        }

        void VulkanContext::UpdateTextureCube(const RefCounted<Texture> &texture, TextureCubemapFace face, uint32 mipLevel,
                                         const Math::Rect2u &region, const PixelData &memory) {
            assert(false);
        }

        void VulkanContext::GenerateMipMaps(const RefCounted<Texture> &texture) {
            assert(false);
        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass, const RefCounted<Framebuffer> &renderTarget) {
            assert(false);
        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass, const SharedPtr<Window> &renderTarget) {
            mCurrentSurface = mSurfaceManager.GetOrCreateSurface(renderTarget);

            // Create render pass
            mRenderPassDescriptor.renderPass = renderPass;
            mRenderPassDescriptor.surface = mCurrentSurface;
            mRenderPassDescriptor.frameIndex = mCurrentSurface->GetImageToDrawIndex();
            mRenderPassDescriptor.framebuffer = nullptr;
            mRenderPassObjects = mFboCache->GetOrCreateRenderPass(mRenderPassDescriptor);

            // Begin new buffer
            commandBuffer = mCmdBufferManager.StartGraphicsCmd();

            // Fill clear values
            ArrayFixed<VkClearValue, Limits::MAX_COLOR_ATTACHMENTS + 1> clearValues;

            // Color attachment
            auto& colorAttachment = renderPass.colorAttachments[0];
            VkClearValue vkColorClearValue;
            vkColorClearValue.color.float32[0] = colorAttachment.clearColor.R();
            vkColorClearValue.color.float32[1] = colorAttachment.clearColor.G();
            vkColorClearValue.color.float32[2] = colorAttachment.clearColor.B();
            vkColorClearValue.color.float32[3] = colorAttachment.clearColor.A();
            clearValues.Add(vkColorClearValue);

            // Depth stencil attachment
            auto& depthStencil = renderPass.depthStencilAttachment;
            VkClearValue vkDepthStencilClearValue;
            vkDepthStencilClearValue.depthStencil.depth = depthStencil.depthClear;
            vkDepthStencilClearValue.depthStencil.stencil = depthStencil.stencilClear;
            clearValues.Add(vkDepthStencilClearValue);

            // Begin render pass
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = mRenderPassObjects.renderPass;
            renderPassInfo.framebuffer = mRenderPassObjects.framebuffer;
            renderPassInfo.renderArea.offset.x = 0;
            renderPassInfo.renderArea.offset.y = 0;
            renderPassInfo.renderArea.extent.width = mCurrentSurface->GetWidth();
            renderPassInfo.renderArea.extent.height = mCurrentSurface->GetHeight();
            renderPassInfo.clearValueCount = clearValues.GetSize();
            renderPassInfo.pClearValues = clearValues.GetData();
            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Setup viewport
            auto viewport = renderPass.viewport;
            VkViewport vkViewport;
            vkViewport.x = viewport.left;
            vkViewport.y = viewport.bottom;
            vkViewport.width = viewport.width;
            vkViewport.height = viewport.height;
            vkViewport.minDepth = 0.0f;
            vkViewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);

            // Set scissors (dummy, will be supported in the future)
            VkRect2D scissor;
            scissor.offset.x = viewport.left;
            scissor.offset.y = viewport.bottom;
            scissor.extent.width = viewport.width;
            scissor.extent.height = viewport.height;
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        }

        void VulkanContext::BindPipelineState(const PipelineState &pipelineState) {
            mPipelineDescriptor.pipelineState = pipelineState;
            mPipelineDescriptor.renderPass = mRenderPassObjects.renderPass;

            mPipeline = mPipelineCache->GetOrCreatePipeline(mPipelineDescriptor);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
        }

        void VulkanContext::BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
            assert(buffers.IsNotEmpty());

            ArrayFixed<VkBuffer, Limits::MAX_VERTEX_BUFFERS> vkBuffers;
            ArrayFixed<VkDeviceSize, Limits::MAX_VERTEX_BUFFERS> vkOffsets;
            uint32 first = 0;
            uint32 count = buffers.GetSize();

            vkOffsets.Resize(count, 0);

            for (auto& buffer: buffers) {
                auto vkBuffer = (VulkanVertexBuffer*) buffer.Get();
                vkBuffers.Add(vkBuffer->GetBuffer());
            }

            vkCmdBindVertexBuffers(commandBuffer, first, count, vkBuffers.GetData(), vkOffsets.GetData());
        }

        void VulkanContext::BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) {
            auto vkBuffer = (VulkanIndexBuffer*) buffer.Get();
            mIndexType = VulkanDefs::GetIndexType(indexType);

            vkCmdBindIndexBuffer(commandBuffer, vkBuffer->GetBuffer(), 0, mIndexType);
        }

        void VulkanContext::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {

        }

        void VulkanContext::BindTexture(const RefCounted<Texture> &texture, uint32 location) {

        }

        void VulkanContext::BindSampler(const RefCounted<Sampler> &sampler, uint32 location) {

        }

        void VulkanContext::Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {


        }

        void VulkanContext::DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) {
            vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, baseIndex, baseVertex, 0);
        }

        void VulkanContext::EndRenderPass() {
            vkCmdEndRenderPass(commandBuffer);

            auto s = GetSemaphore();
            mFramesSync[mFetchIndex].Add(s);

            mCmdBufferManager.Submit(mQueues.FetchNextGraphicsQueue(), commandBuffer, nullptr, s, 0, nullptr);

            if (mRenderPassDescriptor.renderPass.presentation) {
                assert(mCurrentSurface);

                auto swapchain = mCurrentSurface->GetSwapchain();
                auto index = mCurrentSurface->GetImageToDrawIndex();

                VkPresentInfoKHR presentInfo = {};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = &s;
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = &swapchain;
                presentInfo.pImageIndices = &index;
                presentInfo.pResults = nullptr; // Optional

                auto result =  vkQueuePresentKHR(mQueues.FetchNextPresentQueue(), &presentInfo);

                if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
                    // We need to recreate surface
                    mCurrentSurface->Recreate();
                } else if (result != VK_SUCCESS) {
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to present image for {0}"), mCurrentSurface->GetName());
                }

                commandBuffer = mCmdBufferManager.StartGraphicsCmd();
                mCurrentSurface->TransitionLayoutAfterPresentation(commandBuffer);

                auto fence = GetFence();
                mCurrentSurface->AcquireNextImage(nullptr, fence);
                mDevice.GetUtils()->WaitFence(fence);
                mDevice.GetUtils()->DestroyFence(fence);
            }
        }

        void VulkanContext::EndScene() {
            auto fence = GetFence();

            mCmdBufferManager.Submit(mQueues.FetchNextGraphicsQueue(), commandBuffer, fence);
            mFramesToWait[mFetchIndex].Add(fence);
            mDevice.WaitDeviceIdle();
        }

        bool VulkanContext::IsInSeparateThreadMode() const {
            return true;
        }

        void VulkanContext::WaitAndReleaseFences(Array<VkFence> &fences) {
            auto& utils = *mDevice.GetUtils();

            for (auto fence: fences) {
                utils.WaitFence(fence);
                utils.DestroyFence(fence);
            }

            fences.Clear();
        }

        void VulkanContext::ReleaseSemaphores(Array<VkSemaphore> &semaphores) {
            auto& utils = *mDevice.GetUtils();

            for (auto semaphore: semaphores) {
                utils.DestroySemaphore(semaphore);
            }

            semaphores.Clear();
        }

        VkFence VulkanContext::GetFence() {
            return mDevice.GetUtils()->CreateFence(false);
        }

        VkSemaphore VulkanContext::GetSemaphore() {
            return mDevice.GetUtils()->CreateSemaphore();
        }


    }
}