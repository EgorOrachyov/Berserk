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
            assert(mGraphSync.IsEmpty());

            mDevice.WaitDeviceIdle();

            auto& utils = *mDevice.GetUtils();

            for (size_t i = 0; i < Limits::MAX_FRAMES_IN_FLIGHT; i++)  {
                for (auto fence: mFramesToWait[i])
                    utils.DestroyFence(fence);
                for (auto semaphore: mFramesSync[i])
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
            mFrameIndex = mCurrentFrame % Limits::MAX_FRAMES_IN_FLIGHT;

            // Wait for frame before invalidate any data
            WaitAndReleaseFences(mFramesToWait[mFrameIndex]);

            // Release operations synchronization semaphores
            ReleaseSemaphores(mFramesSync[mFrameIndex]);

            mDevice.NextFrame(mCurrentFrame);
        }

        void VulkanContext::EndFrame() {
            // Preset all surfaces, referenced in this frame
            if (mPendingSwapBuffers.IsNotEmpty()) {
                mPresentationResults.Clear();
                mPresentationResults.Resize(mPendingSwapBuffers.GetSize(), VK_SUCCESS);

                auto presentationTransition = mCmdBufferManager.StartGraphicsCmd();

                // Transition layout to presentation
                for (auto& surface: mPendingSwapBuffers) {
                    surface->TransitionLayoutBeforePresentation(presentationTransition);
                }

                // Final transition after presentation
                uint32 waitCount = mWaitBeforeSwap.GetSize();
                VkSemaphore* wait = mWaitBeforeSwap.GetData();
                VkPipelineStageFlags* waitMask = mWaitMaskBeforeSwap.GetData();
                VkSemaphore signal = GetSemaphore();
                mCmdBufferManager.Submit(mQueues.FetchNextGraphicsQueue(), presentationTransition, waitCount, wait, waitMask, signal, GetFence());

                VkPresentInfoKHR presentInfo{};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = &signal;
                presentInfo.swapchainCount = mPendingSwapchains.GetSize();
                presentInfo.pSwapchains = mPendingSwapchains.GetData();
                presentInfo.pImageIndices = mImageIndices.GetData();
                presentInfo.pResults = mPresentationResults.GetData();

                vkQueuePresentKHR(mQueues.FetchNextPresentQueue(), &presentInfo);

                for (size_t i = 0; i < mPendingSwapBuffers.GetSize(); i++) {
                    auto& surface = mPendingSwapBuffers[i];
                    auto result = mPresentationResults[i];

                    surface->NotifyPresented();

                    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
                        surface->Recreate();
                    } else if (result != VK_SUCCESS) {
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to present surface {0}"), surface->GetName());
                    }
                }

                mPendingSwapBuffers.Clear();
                mPendingSwapchains.Clear();
                mWaitMaskBeforeSwap.Clear();
                mWaitBeforeSwap.Clear();
                mImageIndices.Clear();
            }

            // Collect garbage and release caches for unused objects
            mPipelineCache->GC();
            mFboCache->GC();
        }

        void VulkanContext::BeginScene() {
            assert(!mInSceneRendering);
            assert(mGraphSync.IsEmpty());

            // Push dummy root sequence scope
            // And manually start sequence since by default we in sequential mode
            mGraphSync.Push(GetNode(NodeType::Sequence));

            mInSceneRendering = true;
        }

        void VulkanContext::BeginParallel() {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            // If some buffer was created, submit it
            CommitCommands();

            NodeSync* current;
            mGraphSync.Peek(current);

            NodeSync* parallel = GetNode(NodeType::Parallel);
            parallel->waitBeforeStart = current->waitBeforeStart;
            parallel->waitMask = current->waitMask;
            mGraphSync.Push(parallel);
        }

        void VulkanContext::EndParallel() {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            NodeSync* parallel;
            mGraphSync.Pop(parallel);
            assert(parallel->type == NodeType::Parallel);

            NodeSync* current;
            mGraphSync.Peek(current);

            // No commands allowed in parallel
            assert(mGraphics == nullptr);

            if (parallel->hasSubmissions) {
                // Inherit submissions info
                current->hasSubmissions = true;

                if (current->type == NodeType::Parallel) {
                    // Outer parallel block will finish as soon as all enclosed
                    // blocks finish
                    current->signalFinished.Add(parallel->signalFinished);
                } else {
                    // Update wait stages of the sequence buffer
                    // if we did not submit something, then we can't remove initial deps
                    if (parallel->hasSubmissions) {
                        current->waitBeforeStart.Clear();
                        current->waitMask.Clear();
                        current->waitBeforeStart.Add(parallel->signalFinished);
                        current->waitMask.Resize(parallel->signalFinished.GetSize(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
                    }
                }
            }

            ReleaseNode(parallel);
        }

        void VulkanContext::BeginSequence() {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            // If some buffer was created, submit it
            CommitCommands();

            NodeSync* current;
            mGraphSync.Peek(current);

            // Inherit dependencies
            NodeSync* sequence = GetNode(NodeType::Sequence);
            sequence->waitBeforeStart = current->waitBeforeStart;
            sequence->waitMask = current->waitMask;
            mGraphSync.Push(sequence);
        }

        void VulkanContext::EndSequence() {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            // If some buffer was created, submit it
            CommitCommands();

            NodeSync* sequence;
            mGraphSync.Pop(sequence);
            assert(sequence->type == NodeType::Sequence);

            NodeSync* current;
            mGraphSync.Peek(current);

            // If this sequence was not empty (has submissions in this or enclosed parallel blocks and etc.)
            if (sequence->hasSubmissions) {
                // Inherit submissions info
                current->hasSubmissions = true;

                if (current->type == NodeType::Parallel) {
                    // We submitted buffer, add parallel stage signal deps
                    // this deps are required, to end whole parallel block before any sequence ops
                    current->signalFinished.Add(sequence->waitBeforeStart);
                } else {
                    // We submitted buffer in sequence,
                    // And current is sequence, so update its wait deps
                    current->waitMask.Clear();
                    current->waitBeforeStart.Clear();
                    current->waitBeforeStart.Add(sequence->waitBeforeStart);
                    current->waitMask.Add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
                }
            }

            ReleaseNode(sequence);
        }

        void VulkanContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            auto native = (VulkanVertexBuffer*) buffer.Get();
            assert(native);

            auto& utils = *mDevice.GetUtils();
            auto cmd = GetBufferForTransfer();

            native->Update(cmd, byteOffset, byteSize, memory);

            // Insert barrier, since it is synchronized update operation
            utils.BarrierBuffer(cmd,
                                native->GetBuffer(), byteOffset, byteSize,
                                VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
        }

        void VulkanContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            auto native = (VulkanIndexBuffer*) buffer.Get();
            assert(native);

            auto& utils = *mDevice.GetUtils();
            auto cmd = GetBufferForTransfer();

            native->Update(cmd, byteOffset, byteSize, memory);

            // Insert barrier, since it is synchronized update operation
            utils.BarrierBuffer(cmd,
                                native->GetBuffer(), byteOffset, byteSize,
                                VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_INDEX_READ_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);
        }

        void VulkanContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            auto native = (VulkanUniformBuffer*) buffer.Get();
            assert(native);

            auto& utils = *mDevice.GetUtils();
            auto cmd = GetBufferForTransfer();

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

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass, const SharedPtr<Window> &renderTarget) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            assert(renderPass.colorAttachments.GetSize() == 1);
            auto& attachment = renderPass.colorAttachments[0];

            // Commit commands if has
            CommitCommands();

            auto surface = mSurfaceManager.GetOrCreateSurface(renderTarget);

            // Begin new buffer if its is not started yet
            if (mGraphics == nullptr)
                mGraphics = mCmdBufferManager.StartGraphicsCmd();

            // Acquire index to draw
            if (!surface->IsIndexRequested()) {
                // Semaphore to be notified when image is available
                auto imageAvailable = GetSemaphore();
                surface->AcquireNextImage(imageAvailable);

                // Add surface into queue to swap after frame
                mPendingSwapBuffers.Add(surface);
                mImageIndices.Add(surface->GetImageIndexToDraw());
                mPendingSwapchains.Add(surface->GetSwapchain());
            }

            auto top = mGraphSync.PeekTop();
            auto imageAvailable = surface->GetImageAvailableSemaphore();

            // Add sync for this scope dependency
            if (!top->waitBeforeStart.Contains(imageAvailable)) {
                top->waitBeforeStart.Add(surface->GetImageAvailableSemaphore());
                top->waitMask.Add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
            }

            // Prepare image: transition layout if it is required
            if (!VulkanDefs::DiscardsOnStart(attachment.option) && surface->GetCurrentLayout() != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
                surface->TransitionLayoutBeforeDraw(mGraphics);
            }

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
            vkCmdBeginRenderPass(mGraphics, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Setup viewport
            auto viewport = renderPass.viewport;
            VkViewport vkViewport;
            vkViewport.x = viewport.left;
            vkViewport.y = viewport.bottom;
            vkViewport.width = viewport.width;
            vkViewport.height = viewport.height;
            vkViewport.minDepth = 0.0f;
            vkViewport.maxDepth = 1.0f;
            vkCmdSetViewport(mGraphics, 0, 1, &vkViewport);

            // Set scissors (dummy, will be supported in the future)
            VkRect2D scissor;
            scissor.offset.x = viewport.left;
            scissor.offset.y = viewport.bottom;
            scissor.extent.width = viewport.width;
            scissor.extent.height = viewport.height;
            vkCmdSetScissor(mGraphics, 0, 1, &scissor);

            mInRenderPass = true;
        }

        void VulkanContext::BindPipelineState(const PipelineState &pipelineState) {
            assert(mInRenderPass);

            mPipelineDescriptor.pipelineState = pipelineState;
            mPipelineDescriptor.renderPass = mRenderPassObjects.renderPass;

            mPipeline = mPipelineCache->GetOrCreatePipeline(mPipelineDescriptor);
            vkCmdBindPipeline(mGraphics, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);

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
                vkBuffers.Add(vkBuffer->GetBuffer());
            }

            vkCmdBindVertexBuffers(mGraphics, first, count, vkBuffers.GetData(), vkOffsets.GetData());
        }

        void VulkanContext::BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) {
            assert(mPipelineBound);

            auto vkBuffer = (VulkanIndexBuffer*) buffer.Get();
            mIndexType = VulkanDefs::GetIndexType(indexType);

            vkCmdBindIndexBuffer(mGraphics, vkBuffer->GetBuffer(), 0, mIndexType);
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

        }

        void VulkanContext::DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) {
            assert(mPipelineBound);

            vkCmdDrawIndexed(mGraphics, indexCount, instanceCount, baseIndex, baseVertex, 0);
        }

        void VulkanContext::EndRenderPass() {
            assert(mInRenderPass);

            vkCmdEndRenderPass(mGraphics);

            mInRenderPass = false;
            mPipelineBound = false;
        }

        void VulkanContext::EndScene() {
            assert(mInSceneRendering);
            assert(mGraphSync.GetSize() == 1);

            // Commit last commands
            CommitCommands();

            // Finish sequence and manually pop dummy root
            NodeSync* root;
            mGraphSync.Pop(root);

            // Root is sequence scope, so next is started as soon as all wait stages signaled
            mWaitBeforeSwap.Add(root->waitBeforeStart);
            mWaitMaskBeforeSwap.Add(root->waitMask);

            ReleaseNode(root);

            mInSceneRendering = false;
        }

        bool VulkanContext::IsInSeparateThreadMode() const {
            return true;
        }

        void VulkanContext::CommitCommands() {
            ArrayFixed<VkSemaphore, 2> signaled;
            ArrayFixed<VkPipelineStageFlags, 2> stages;

            bool commitGraphics = false;
            bool commitTransfer = false;

            if (mGraphics || mTransfer) {
                NodeSync* current;
                mGraphSync.Peek(current);

                uint32 waitCount = current->waitBeforeStart.GetSize();
                VkSemaphore* wait = current->waitBeforeStart.GetData();
                VkPipelineStageFlags* flags = current->waitMask.GetData();

                if (mGraphics) {
                    assert(current->type == NodeType::Sequence);

                    VkSemaphore signal = GetSemaphore();
                    VkQueue queue = mQueues.FetchNextGraphicsQueue();

                    mCmdBufferManager.Submit(queue, mGraphics, waitCount, wait, flags, signal, GetFence());

                    VkPipelineStageFlags stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

                    // If some transfer commands were captured, add
                    // transfer dependency
                    if (mTransferCommandsInGraphicsBuffer) {
                        stage |= VK_PIPELINE_STAGE_TRANSFER_BIT;
                    }

                    // We submitted buffer in sequence, so now we need to update
                    // wait stages, since they will be handled by `signal`
                    signaled.Add(signal);
                    stages.Add(stage);

                    commitGraphics = true;
                    mGraphics = nullptr;
                }

                if (mTransfer) {
                    VkSemaphore signal = GetSemaphore();
                    VkQueue queue = mQueues.FetchNextTransferQueue();

                    mCmdBufferManager.Submit(queue, mTransfer, waitCount, wait, flags, signal, GetFence());

                    // Transfer op must finish before the next stage
                    // So when transfer is finished, this is signalled
                    signaled.Add(signal);
                    stages.Add(VK_PIPELINE_STAGE_TRANSFER_BIT);

                    commitTransfer = true;
                    mTransfer = nullptr;
                }

                current->hasSubmissions = true;

                if (commitGraphics || commitTransfer) {
                    current->waitBeforeStart.Clear();
                    current->waitMask.Clear();
                }

                assert(signaled.GetSize() == stages.GetSize());

                current->waitBeforeStart.Add(signaled.GetData(), signaled.GetSize());
                current->waitMask.Add(stages.GetData(), stages.GetSize());
            }
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
            node->hasSubmissions = false;
            node->waitBeforeStart.Clear();
            node->signalFinished.Clear();
            node->waitMask.Clear();
            mCachedSyncNodes.Add(node);
        }

        VkCommandBuffer VulkanContext::GetBufferForTransfer() {
            if (mGraphics)
                return mGraphics;
            else
                return mGraphics = mCmdBufferManager.StartGraphicsCmd();
        }

        VkFence VulkanContext::GetFence() {
            auto fence = mDevice.GetUtils()->CreateFence(false);
            mFramesToWait[mFrameIndex].Add(fence);
            return fence;
        }

        VkSemaphore VulkanContext::GetSemaphore() {
            auto semaphore =  mDevice.GetUtils()->CreateSemaphore();
            mFramesSync[mFrameIndex].Add(semaphore);
            return semaphore;
        }

        VulkanContext::NodeSync *VulkanContext::GetNode(VulkanContext::NodeType type) {
            auto node = mCachedSyncNodes.IsNotEmpty()? mCachedSyncNodes.PopLast(): Memory::Make<NodeSync>();
            node->type = type;
            return node;
        }
    }
}