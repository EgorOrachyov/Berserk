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
#include <BerserkVulkan/VulkanTexture.hpp>
#include <BerserkVulkan/VulkanVertexBuffer.hpp>
#include <BerserkVulkan/VulkanIndexBuffer.hpp>
#include <BerserkVulkan/VulkanUniformBuffer.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanSurfaceManager.hpp>
#include <BerserkVulkan/VulkanCmdBufferManager.hpp>
#include <BerserkVulkan/VulkanDescriptorSetManager.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {
    namespace RHI {

        template<typename Buffer>
        static void UpdateBuffer(Buffer* buffer, uint32 byteOffset, uint32 byteSize, const void *memory,
                                VkAccessFlags dstAccessFlags, VkPipelineStageFlags dstStageFlags,
                                uint32 frame, uint32 scene, VkCommandBuffer cmd, VulkanUtils& utils) {
            assert(buffer);

            buffer->NotifyWrite(frame, scene);
            buffer->Update(cmd, byteOffset, byteSize, memory);

            // Insert barrier, since it is synchronized update operation
            utils.BarrierBuffer(cmd,
                                buffer->GetBuffer(), byteOffset, byteSize,
                                VK_ACCESS_TRANSFER_WRITE_BIT, dstAccessFlags,
                                VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageFlags);
        }

        VulkanContext::VulkanContext(VulkanDevice &device)
                : mDevice(device),
                  mQueues(*device.GetQueues()),
                  mSurfaceManager(*device.GetSurfaceManager()),
                  mCmdBufferManager(*device.GetCmdBufferManager()) {

            mResources.Resize(Limits::MAX_FRAMES_IN_FLIGHT);
            mFboCache = SharedPtr<VulkanFramebufferCache>::Make(mDevice);
            mPipelineCache = SharedPtr<VulkanPipelineCache>::Make(mDevice);
            mDescSetMan = SharedPtr<VulkanDescriptorSetManager>::Make(mDevice);
        }

        VulkanContext::~VulkanContext() {
            assert(mGraphSync.IsEmpty());

            for (auto node: mCachedSyncNodes)
                Memory::Release(node);

            mDevice.WaitDeviceIdle();
            mDescSetMan = nullptr;
            mPipelineCache = nullptr;
            mFboCache = nullptr;
        }

        void VulkanContext::BeginFrame() {
            // Advance frame, get index of the frame to draw/use cached data
            mCurrentFrame += 1;
            mCurrentScene = 0;
            mResources[mCurrentFrame % Limits::MAX_FRAMES_IN_FLIGHT].Clear();

            mDevice.NextFrame();
            mDescSetMan->NextFrame();
            mCmdBufferManager.BeginFrame(mCurrentFrame);

            // Start buffer for safety, in case if have some updates before scene begin/end calls
            mGraphicsCmd = mCmdBufferManager.GetGraphicsCmdBuffer();
        }

        void VulkanContext::EndFrame() {
            // Commit all pending buffers
            mCmdBufferManager.EndFrame();

            // Reset set of used windows for this frame
            mUsedWindows.Clear();

            // Collect garbage and release caches for unused objects
            // Remember: desc gc first, fbo - last, since it has dependencies
            // mDescSetMan -> layout -> mPipelineCache -> render pass -> mFboCache
            mDescSetMan->GC();
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
            mGraphicsCmd = mCmdBufferManager.GetGraphicsCmdBuffer();

            BERSERK_VK_BEGIN_LABEL(mGraphicsCmd, "Scene rendering " + String::From(mCurrentScene))

            if (window) {
                // Check that window was not used in this frame before
                assert(!mUsedWindows.Contains<Equals<SharedPtr<Window>>>(window));
                mUsedWindows.Add(window);

                // Find surface and acquire new index to draw
                auto surface = mSurfaceManager.GetOrCreateSurface(window);
                mCmdBufferManager.AcquireImage(*surface);
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

            // Inherit dependencies
            NodeSync* sequence = GetNode(NodeType::Sequence);
            mGraphSync.Push(sequence);
        }

        void VulkanContext::EndSequence() {
            assert(0);

            assert(mInSceneRendering);
            assert(!mInRenderPass);

            NodeSync* sequence;
            mGraphSync.Pop(sequence);
            assert(sequence->type == NodeType::Sequence);

            ReleaseNode(sequence);
        }

        void VulkanContext::UpdateVertexBuffer(const RcPtr<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            assert(!mInRenderPass);

            auto& utils = *mDevice.GetUtils();
            auto native = (VulkanVertexBuffer*) buffer.Get();
            UpdateBuffer(native, byteOffset, byteSize, memory,
                         VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                         mCurrentFrame,  mCurrentScene, mGraphicsCmd, utils);

            UseResource(buffer);
        }

        void VulkanContext::UpdateIndexBuffer(const RcPtr<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            assert(!mInRenderPass);

            auto& utils = *mDevice.GetUtils();
            auto native = (VulkanIndexBuffer*) buffer.Get();
            UpdateBuffer(native, byteOffset, byteSize, memory,
                         VK_ACCESS_INDEX_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                         mCurrentFrame,  mCurrentScene, mGraphicsCmd, utils);

            UseResource(buffer);
        }

        void VulkanContext::UpdateUniformBuffer(const RcPtr<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) {
            assert(!mInRenderPass);

            auto& utils = *mDevice.GetUtils();
            auto native = (VulkanUniformBuffer*) buffer.Get();
            UpdateBuffer(native, byteOffset, byteSize, memory,
                         VK_ACCESS_UNIFORM_READ_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
                         mCurrentFrame,  mCurrentScene, mGraphicsCmd, utils);

            UseResource(buffer);
        }

        void VulkanContext::UpdateTexture2D(const RcPtr<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) {
            assert(!mInRenderPass);

            auto native = (VulkanTexture*) texture.Get();
            assert(native);
            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->UpdateTexture2D(mGraphicsCmd, mipLevel, region, memory);

            UseResource(texture);
        }

        void VulkanContext::UpdateTexture2DArray(const RcPtr<Texture> &texture, uint32 arrayIndex, uint32 mipLevel,
                                                 const Math::Rect2u &region, const PixelData &memory) {
            assert(!mInRenderPass);

            auto native = (VulkanTexture*) texture.Get();
            assert(native);
            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->UpdateTexture2DArray(mGraphicsCmd, arrayIndex, mipLevel, region, memory);

            UseResource(texture);
        }

        void VulkanContext::UpdateTextureCube(const RcPtr<Texture> &texture, TextureCubemapFace face, uint32 mipLevel,
                                         const Math::Rect2u &region, const PixelData &memory) {
            auto native = (VulkanTexture*) texture.Get();
            assert(native);
            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->UpdateTextureCube(mGraphicsCmd, face, mipLevel, region, memory);

            UseResource(texture);
        }

        void VulkanContext::GenerateMipMaps(const RcPtr<Texture> &texture) {
            auto native = (VulkanTexture*) texture.Get();
            assert(native);
            native->NotifyWrite(mCurrentFrame, mCurrentScene);
            native->GenerateMipmaps(mGraphicsCmd);

            UseResource(texture);
        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass, const RcPtr<Framebuffer> &renderTarget) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);
            assert(renderTarget);

            assert(renderPass.colorAttachments.GetSize() == renderTarget->GetColorAttachmentsCount());

            auto vkFramebuffer = (VulkanFramebuffer*) renderTarget.Get();
            vkFramebuffer->NotifyWrite(mCurrentFrame, mCurrentScene);

            UseResource(renderTarget);

            // Create render pass
            mRenderPassDescriptor.renderPass = renderPass;
            mRenderPassDescriptor.surface = nullptr;
            mRenderPassDescriptor.frameIndex = 0;
            mRenderPassDescriptor.framebuffer = renderTarget;
            mRenderPassObjects = mFboCache->GetOrCreateRenderPass(mRenderPassDescriptor);

            // Define are
            VkRect2D renderArea{};
            renderArea.offset.x = 0;
            renderArea.offset.y = 0;
            renderArea.extent.width = renderTarget->GetWidth();
            renderArea.extent.height = renderTarget->GetHeight();

            // Internal init
            BeginRenderPassInternal(renderPass, renderArea);
        }

        void VulkanContext::BeginRenderPass(const RenderPass &renderPass) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);
            assert(mWindow);

            assert(renderPass.colorAttachments.GetSize() == 1);

            auto& attachment = renderPass.colorAttachments[0];
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

            // Define are
            VkRect2D renderArea{};
            renderArea.offset.x = 0;
            renderArea.offset.y = 0;
            renderArea.extent.width = surface->GetWidth();
            renderArea.extent.height = surface->GetHeight();

            // Internal init
            BeginRenderPassInternal(renderPass, renderArea);
        }

        void VulkanContext::BindPipelineState(const PipelineState &pipelineState) {
            assert(mInRenderPass);

            // Update pipeline descriptor, invalidate prev state
            mPipelineDescriptor.pipelineState = pipelineState;
            mPipelineDescriptor.renderPass = mRenderPassObjects.renderPass;

            // Get pipeline for rendering
            mPipeline = mPipelineCache->GetOrCreatePipeline(mPipelineDescriptor);
            vkCmdBindPipeline(mGraphicsCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline.pipeline);

            // Setup bindings layout and meta info
            mDescSetMan->BindLayout(mPipeline.bindingInfo.descriptorSetLayout, mPipeline.bindingInfo.meta);
            mDescriptorSet = VK_NULL_HANDLE;
            mIndexType = VK_INDEX_TYPE_MAX_ENUM;

            mPipelineBound = true;
        }

        void VulkanContext::BindVertexBuffers(const ArrayFixed<RcPtr<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
            assert(mPipelineBound);
            assert(buffers.IsNotEmpty());

            ArrayFixed<VkBuffer, Limits::MAX_VERTEX_BUFFERS> vkBuffers;
            ArrayFixed<VkDeviceSize, Limits::MAX_VERTEX_BUFFERS> vkOffsets;
            uint32 first = 0;
            uint32 count = static_cast<uint32>(buffers.GetSize());

            vkOffsets.Resize(count, 0);

            for (auto& buffer: buffers) {
                auto vkBuffer = (VulkanVertexBuffer*) buffer.Get();
                vkBuffer->NotifyRead(mCurrentFrame, mCurrentScene);
                vkBuffers.Add(vkBuffer->GetBuffer());
                UseResource(buffer);
            }

            vkCmdBindVertexBuffers(mGraphicsCmd, first, count, vkBuffers.GetData(), vkOffsets.GetData());
        }

        void VulkanContext::BindIndexBuffer(const RcPtr<IndexBuffer> &buffer, IndexType indexType) {
            assert(mPipelineBound);

            auto vkBuffer = (VulkanIndexBuffer*) buffer.Get();
            vkBuffer->NotifyRead(mCurrentFrame, mCurrentScene);
            mIndexType = VulkanDefs::GetIndexType(indexType);

            vkCmdBindIndexBuffer(mGraphicsCmd, vkBuffer->GetBuffer(), 0, mIndexType);

            UseResource(buffer);
        }

        void VulkanContext::BindUniformBuffer(const RcPtr<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
            assert(mPipelineBound);

            auto vkBuffer = (VulkanUniformBuffer*) buffer.Get();
            vkBuffer->NotifyRead(mCurrentFrame, mCurrentScene);
            mDescSetMan->BindUniformBuffer(buffer, index, byteOffset, byteSize);

            UseResource(buffer);
        }

        void VulkanContext::BindTexture(const RcPtr<Texture> &texture, uint32 location) {
            BindTexture(texture, location, 0);
        }

        void VulkanContext::BindSampler(const RcPtr<Sampler> &sampler, uint32 location) {
            BindSampler(sampler, location, 0);
        }

        void VulkanContext::BindTexture(const RcPtr<Texture> &texture, uint32 location, uint32 arrayIndex) {
            assert(mPipelineBound);

            auto vkTexture = (VulkanTexture*) texture.Get();
            vkTexture->NotifyRead(mCurrentFrame, mCurrentScene);
            mDescSetMan->BindTexture(texture, location, arrayIndex);

            UseResource(texture);
        }

        void VulkanContext::BindSampler(const RcPtr<Sampler> &sampler, uint32 location, uint32 arrayIndex) {
            assert(mPipelineBound);

            mDescSetMan->BindSampler(sampler, location, arrayIndex);

            UseResource(sampler);
        }

        void VulkanContext::Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {
            assert(mPipelineBound);

            BindDescriptorSet();
            vkCmdDraw(mGraphicsCmd, verticesCount, instancesCount, baseVertex, 0);
        }

        void VulkanContext::DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) {
            assert(mPipelineBound);

            BindDescriptorSet();
            vkCmdDrawIndexed(mGraphicsCmd, indexCount, instanceCount, baseIndex, baseVertex, 0);
        }

        void VulkanContext::EndRenderPass() {
            assert(mInRenderPass);

            BERSERK_VK_END_LABEL(mGraphicsCmd);

            // Finish render pass
            vkCmdEndRenderPass(mGraphicsCmd);

            // Reset state when pass is ended
            mInRenderPass = false;
            mPipelineBound = false;

            mRenderPassDescriptor = VulkanFramebufferCache::RenderPassDescriptor();
            mRenderPassObjects = VulkanFramebufferCache::RenderPassObjects();
            mPipelineDescriptor = VulkanPipelineCache::PipelineDescriptor();
            mPipeline = VulkanPipelineCache::PipelineObjects();
            mDescriptorSet = VK_NULL_HANDLE;
            mIndexType = VK_INDEX_TYPE_MAX_ENUM;
        }

        void VulkanContext::EndScene() {
            assert(mInSceneRendering);
            assert(mGraphSync.GetSize() == 1);

            // Finish sequence and manually pop dummy root
            NodeSync* root;
            mGraphSync.Pop(root);

            BERSERK_VK_END_LABEL(mGraphicsCmd);

            if (mWindow) {
                // Preset image if required
                // Window no more used, can decrement unsafe usage
                auto surface = mSurfaceManager.GetOrCreateSurface(mWindow);
                mCmdBufferManager.EndScene(*surface);

                mWindow->ReleaseUnsafeUsage();
                mWindow = nullptr;
            } else {
                // Submits commands for rendering
                // no presentation
                mCmdBufferManager.EndScene();
            }

            ReleaseNode(root);

            mInSceneRendering = false;
            mCurrentScene += 1;
            mGraphicsCmd = nullptr;
        }

        bool VulkanContext::IsInSeparateThreadMode() const {
            return true;
        }

        void VulkanContext::BindDescriptorSet() {
            auto prev = mDescriptorSet;
            mDescriptorSet = mDescSetMan->GetOrCreateSet();

            if (prev != mDescriptorSet) {
                vkCmdBindDescriptorSets(mGraphicsCmd,
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        mPipeline.layout, 0, 1, &mDescriptorSet,
                                        0,nullptr);
            }
        }

        void VulkanContext::ReleaseNode(VulkanContext::NodeSync *node) {
            mCachedSyncNodes.Add(node);
        }

        VulkanContext::NodeSync *VulkanContext::GetNode(VulkanContext::NodeType type) {
            auto node = mCachedSyncNodes.IsNotEmpty()? mCachedSyncNodes.PopLast(): Memory::Make<NodeSync>();
            node->type = type;
            return node;
        }

        void VulkanContext::BeginRenderPassInternal(const RenderPass &renderPass, const VkRect2D& renderArea) {
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
            renderPassInfo.renderArea = renderArea;
            renderPassInfo.clearValueCount = static_cast<uint32>(clearValues.GetSize());
            renderPassInfo.pClearValues = clearValues.GetData();
            vkCmdBeginRenderPass(mGraphicsCmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            BERSERK_VK_BEGIN_LABEL(mGraphicsCmd, renderPass.name);

            // Setup viewport
            auto viewport = renderPass.viewport;
            VkViewport vkViewport;
            vkViewport.x = static_cast<float>(viewport.left);
            vkViewport.y = static_cast<float>(viewport.bottom);
            vkViewport.width = static_cast<float>(viewport.width);
            vkViewport.height = static_cast<float>(viewport.height);
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

    }
}