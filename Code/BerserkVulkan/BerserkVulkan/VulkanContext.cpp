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
            mUsedSemaphores.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
            mPipelineCache = SharedPtr<VulkanPipelineCache>::Make(mDevice);
            mFboCache = SharedPtr<VulkanFramebufferCache>::Make(mDevice);
        }

        VulkanContext::~VulkanContext() {
            assert(mGraphSync.IsEmpty());

            mDevice.WaitDeviceIdle();

            auto& utils = *mDevice.GetUtils();

            for (size_t i = 0; i < Limits::MAX_FRAMES_IN_FLIGHT; i++)  {
                for (auto fence: mFramesToWait[i])
                    utils.DestroyFence(fence);
                for (auto semaphore: mUsedSemaphores[i])
                    utils.DestroySemaphore(semaphore);
            }

            for (auto node: mCachedSyncNodes)
                Memory::Release(node);

            mFboCache = nullptr;
            mPipelineCache = nullptr;
        }

        void VulkanContext::BeginFrame() {
            // Advance frame, get index of the frame to draw/use cached data
            mCurrentFrame += 1;
            mCurrentScene = 0;
            mPrevFrameIndex = mFrameIndex;
            mFrameIndex = mCurrentFrame % Limits::MAX_FRAMES_IN_FLIGHT;

            // Wait for frame before invalidate any data
            WaitAndReleaseFences(mFramesToWait[mFrameIndex]);

            // Release operations synchronization semaphores
            ReleaseSemaphores(mUsedSemaphores[mFrameIndex]);

            mDevice.NextFrame(mCurrentFrame);
        }

        void VulkanContext::EndFrame() {
            if (mGraphicsCmd) {
                mCmdBufferManager.Submit(mQueues.FetchNextGraphicsQueue(), mGraphicsCmd, GetFence());
                mGraphicsCmd = nullptr;
            }

            // Reset set of used windows for this frame
            mUsedWindows.Clear();

            // Collect garbage and release caches for unused objects
            mPipelineCache->GC();
            mFboCache->GC();
        }

        void VulkanContext::BeginScene(const SharedPtr<Window> &window) {
            assert(!mInSceneRendering);
            assert(mGraphSync.IsEmpty());

            // Push dummy root sequence scope
            // And manually start sequence since by default we in sequential mode
            auto root = GetNode(NodeType::Sequence);

            // Cache window to release unsafe usages later and to
            // Get access to the surface of the window
            mWindow = window;
            // Start new command buffer or continue already started, if user has
            // some update commands out of the scene scope (it allows to preserve order).
            mGraphicsCmd = GetOrCreateCmdBuffer();

            if (window) {
                // Check that window was not used in this frame before
                assert(!mUsedWindows.Contains(window));

                mUsedWindows.Add(window);

                // Find surface and acquire new index to draw
                auto surface = mSurfaceManager.GetOrCreateSurface(window);
                auto signal = GetSemaphore();

                surface->AcquireNextImage(signal);

                // Add sync for this scope dependency
                mWait.Add(signal);
                mWaitMask.Add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
            }

            mGraphSync.Push(root);
            mInSceneRendering = true;
        }

        void VulkanContext::BeginParallel() {
            assert(0);

            assert(mInSceneRendering);
            assert(!mInRenderPass);

            NodeSync* parallel = GetNode(NodeType::Parallel);
            mGraphSync.Push(parallel);
        }

        void VulkanContext::EndParallel() {
            assert(0);

            assert(mInSceneRendering);
            assert(!mInRenderPass);

            NodeSync* parallel;
            mGraphSync.Pop(parallel);
            assert(parallel->type == NodeType::Parallel);

            ReleaseNode(parallel);
        }

        void VulkanContext::BeginSequence() {
            assert(0);

            assert(mInSceneRendering);
            assert(!mInRenderPass);

            // If some buffer was created, submit it
            CommitCommands();

            // Inherit dependencies
            NodeSync* sequence = GetNode(NodeType::Sequence);
            mGraphSync.Push(sequence);
        }

        void VulkanContext::EndSequence() {
            assert(0);

            assert(mInSceneRendering);
            assert(!mInRenderPass);

            // If some buffer was created, submit it
            CommitCommands();

            NodeSync* sequence;
            mGraphSync.Pop(sequence);
            assert(sequence->type == NodeType::Sequence);

            ReleaseNode(sequence);
        }

        void VulkanContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            assert(!mInRenderPass);

            auto native = (VulkanVertexBuffer*) buffer.Get();
            assert(native);

            auto& utils = *mDevice.GetUtils();
            auto cmd = GetOrCreateCmdBuffer();

            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->Update(cmd, byteOffset, byteSize, memory);

            // Insert barrier, since it is synchronized update operation
            utils.BarrierBuffer(cmd,
                                native->GetBuffer(), byteOffset, byteSize,
                                VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
        }

        void VulkanContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            assert(!mInRenderPass);

            auto native = (VulkanIndexBuffer*) buffer.Get();
            assert(native);

            auto& utils = *mDevice.GetUtils();
            auto cmd = GetOrCreateCmdBuffer();

            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->Update(cmd, byteOffset, byteSize, memory);

            // Insert barrier, since it is synchronized update operation
            utils.BarrierBuffer(cmd,
                                native->GetBuffer(), byteOffset, byteSize,
                                VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
        }

        void VulkanContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            assert(!mInRenderPass);

            auto native = (VulkanUniformBuffer*) buffer.Get();
            assert(native);

            auto& utils = *mDevice.GetUtils();
            auto cmd = GetOrCreateCmdBuffer();

            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->Update(cmd, byteOffset, byteSize, memory);

            // Insert barrier, since it is synchronized update operation
            utils.BarrierBuffer(cmd,
                                native->GetBuffer(), byteOffset, byteSize,
                                VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_UNIFORM_READ_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
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
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            mInRenderPass = true;
        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);
            assert(mWindow);

            assert(renderPass.colorAttachments.GetSize() == 1);

            auto& attachment = renderPass.colorAttachments[0];

            // Commit commands if has
            CommitCommands();

            auto surface = mSurfaceManager.GetOrCreateSurface(mWindow);

            // Prepare image: transition layout if it is required
            if (!VulkanDefs::DiscardsOnStart(attachment.option) && surface->GetCurrentLayout() != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
                surface->TransitionLayoutBeforeDraw(mGraphicsCmd);
            }

            // Signal, that surface in render pass
            surface->NotifyUsedInRenderPass();

            // Create render pass
            mRenderPassDescriptor.renderPass = renderPass;
            mRenderPassDescriptor.surface = surface;
            mRenderPassDescriptor.frameIndex = surface->GetImageIndexToDraw();
            mRenderPassDescriptor.framebuffer = nullptr;
            mRenderPassObjects = mFboCache->GetOrCreateRenderPass(mRenderPassDescriptor);

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
            renderPassInfo.renderArea.extent.width = surface->GetWidth();
            renderPassInfo.renderArea.extent.height = surface->GetHeight();
            renderPassInfo.clearValueCount = clearValues.GetSize();
            renderPassInfo.pClearValues = clearValues.GetData();
            vkCmdBeginRenderPass(mGraphicsCmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Setup viewport
            auto viewport = renderPass.viewport;
            VkViewport vkViewport;
            vkViewport.x = viewport.left;
            vkViewport.y = viewport.bottom;
            vkViewport.width = viewport.width;
            vkViewport.height = viewport.height;
            vkViewport.minDepth = 0.0f;
            vkViewport.maxDepth = 1.0f;
            vkCmdSetViewport(mGraphicsCmd, 0, 1, &vkViewport);

            // Set scissors (dummy, will be supported in the future)
            VkRect2D scissor;
            scissor.offset.x = viewport.left;
            scissor.offset.y = viewport.bottom;
            scissor.extent.width = viewport.width;
            scissor.extent.height = viewport.height;
            vkCmdSetScissor(mGraphicsCmd, 0, 1, &scissor);

            mInRenderPass = true;
        }

        void VulkanContext::BindPipelineState(const PipelineState &pipelineState) {
            assert(mInRenderPass);

            // Release bindings
            // ......

            // Update pipeline descriptor, invalidate prev state
            mPipelineDescriptor.pipelineState = pipelineState;
            mPipelineDescriptor.renderPass = mRenderPassObjects.renderPass;

            // Get pipeline for rendering
            mPipeline = mPipelineCache->GetOrCreatePipeline(mPipelineDescriptor);
            vkCmdBindPipeline(mGraphicsCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);

            mPipelineBound = true;
        }

        void VulkanContext::BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
            assert(mPipelineBound);
            assert(buffers.IsNotEmpty());

            ArrayFixed<VkBuffer, Limits::MAX_VERTEX_BUFFERS> vkBuffers;
            ArrayFixed<VkDeviceSize, Limits::MAX_VERTEX_BUFFERS> vkOffsets;
            uint32 first = 0;
            uint32 count = buffers.GetSize();

            vkOffsets.Resize(count, 0);

            for (auto& buffer: buffers) {
                auto vkBuffer = (VulkanVertexBuffer*) buffer.Get();
                vkBuffer->NotifyRead(mCurrentFrame, mCurrentScene);
                vkBuffers.Add(vkBuffer->GetBuffer());
            }

            vkCmdBindVertexBuffers(mGraphicsCmd, first, count, vkBuffers.GetData(), vkOffsets.GetData());
        }

        void VulkanContext::BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) {
            assert(mPipelineBound);

            auto vkBuffer = (VulkanIndexBuffer*) buffer.Get();
            vkBuffer->NotifyRead(mCurrentFrame, mCurrentScene);
            mIndexType = VulkanDefs::GetIndexType(indexType);

            vkCmdBindIndexBuffer(mGraphicsCmd, vkBuffer->GetBuffer(), 0, mIndexType);
        }

        void VulkanContext::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
            assert(mPipelineBound);

        }

        void VulkanContext::BindTexture(const RefCounted<Texture> &texture, uint32 location) {
            assert(mPipelineBound);

        }

        void VulkanContext::BindSampler(const RefCounted<Sampler> &sampler, uint32 location) {
            assert(mPipelineBound);

        }

        void VulkanContext::Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {
            assert(mPipelineBound);

            vkCmdDraw(mGraphicsCmd, verticesCount, instancesCount, baseVertex, 0);
        }

        void VulkanContext::DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) {
            assert(mPipelineBound);

            vkCmdDrawIndexed(mGraphicsCmd, indexCount, instanceCount, baseIndex, baseVertex, 0);
        }

        void VulkanContext::EndRenderPass() {
            assert(mInRenderPass);

            // Finish render pass
            vkCmdEndRenderPass(mGraphicsCmd);

            // Reset state when pass is ended
            mInRenderPass = false;
            mPipelineBound = false;
            mRenderPassDescriptor = VulkanFramebufferCache::RenderPassDescriptor();
            mRenderPassObjects = VulkanFramebufferCache::RenderPassObjects();
            mPipelineDescriptor = VulkanPipelineCache::PipelineDescriptor();
            mPipeline = nullptr;

            // Also release bound objects
            // ....
        }

        void VulkanContext::EndScene() {
            assert(mInSceneRendering);
            assert(mGraphSync.GetSize() == 1);

            // Commit last commands
            CommitCommands();

            // Finish sequence and manually pop dummy root
            NodeSync* root;
            mGraphSync.Pop(root);

            // Preset image if required
            if (mWindow) {
                auto surface = mSurfaceManager.GetOrCreateSurface(mWindow);

                // Transition layout to presentation
                surface->TransitionLayoutBeforePresentation(mGraphicsCmd);

                // Final transition after presentation
                uint32 waitCount = mWait.GetSize();
                VkSemaphore* wait = mWait.GetData();
                VkPipelineStageFlags* waitMask = mWaitMask.GetData();
                VkSemaphore signal = GetSemaphore();
                mCmdBufferManager.Submit(mQueues.FetchNextGraphicsQueue(), mGraphicsCmd, waitCount, wait, waitMask, signal, GetFence());

                auto swapchain = surface->GetSwapchain();
                auto imageIndex = surface->GetImageIndexToDraw();

                VkPresentInfoKHR presentInfo{};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = &signal;
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = &swapchain;
                presentInfo.pImageIndices = &imageIndex;
                presentInfo.pResults = nullptr;

                auto result = vkQueuePresentKHR(mQueues.FetchNextPresentQueue(), &presentInfo);

                // Notify, that we presented image into surface of currently bound window
                surface->NotifyPresented();

                // Handle resize/recreation of the swap chain
                if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
                    surface->Recreate();
                } else if (result != VK_SUCCESS) {
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to present surface {0}"), surface->GetName());
                }
            }

            // Window no more used, can decrement unsafe usage
            if (mWindow) {
                mWindow->ReleaseUnsafeUsage();
                mWindow = nullptr;
            }

            ReleaseNode(root);

            mInSceneRendering = false;
            mCurrentScene += 1;
            mGraphicsCmd = nullptr;
            mWait.Clear();
            mWaitMask.Clear();
        }

        bool VulkanContext::IsInSeparateThreadMode() const {
            return true;
        }

        void VulkanContext::CommitCommands() {

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

        void VulkanContext::ReleaseNode(VulkanContext::NodeSync *node) {
            mCachedSyncNodes.Add(node);
        }

        VkCommandBuffer VulkanContext::GetOrCreateCmdBuffer() {
            if (mGraphicsCmd)
                return mGraphicsCmd;
            else
                return mGraphicsCmd = mCmdBufferManager.StartGraphicsCmd();
        }

        VkFence VulkanContext::GetFence() {
            auto fence = mDevice.GetUtils()->CreateFence(false);
            mFramesToWait[mFrameIndex].Add(fence);
            return fence;
        }

        VkSemaphore VulkanContext::GetSemaphore() {
            auto semaphore =  mDevice.GetUtils()->CreateSemaphore();
            mUsedSemaphores[mFrameIndex].Add(semaphore);
            return semaphore;
        }

        VulkanContext::NodeSync *VulkanContext::GetNode(VulkanContext::NodeType type) {
            auto node = mCachedSyncNodes.IsNotEmpty()? mCachedSyncNodes.PopLast(): Memory::Make<NodeSync>();
            node->type = type;
            return node;
        }
    }
}