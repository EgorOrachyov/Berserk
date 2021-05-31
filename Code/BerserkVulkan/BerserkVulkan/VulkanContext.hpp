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

#ifndef BERSERK_VULKANCONTEXT_HPP
#define BERSERK_VULKANCONTEXT_HPP

#include <BerserkRHI/RHIContext.hpp>
#include <BerserkCore/Templates/Stack.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanPipelineCache.hpp>
#include <BerserkVulkan/VulkanFramebufferCache.hpp>

namespace Berserk {
    namespace RHI {

        class VulkanContext final: public Context {
        public:
            explicit VulkanContext(class VulkanDevice& device);
            VulkanContext(const VulkanContext&) = delete;
            VulkanContext(VulkanContext&&) noexcept = delete;
            ~VulkanContext() override;

            void BeginFrame();
            void EndFrame();

            void BeginScene(const SharedPtr<Window> &window) override;

            void BeginParallel() override;
            void EndParallel() override;
            void BeginSequence() override;
            void EndSequence() override;

            void UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) override;
            void UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) override;
            void UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void *memory) override;
            void UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) override;
            void UpdateTexture2DArray(const RefCounted<Texture> &texture, uint32 arrayIndex, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) override;
            void UpdateTextureCube(const RefCounted<Texture> &texture, TextureCubemapFace face, uint32 mipLevel, const Math::Rect2u &region, const PixelData &memory) override;
            void GenerateMipMaps(const RefCounted<Texture> &texture) override;

            void BeginRenderPass(const RenderPass &renderPass, const RefCounted<Framebuffer> &renderTarget) override;
            void BeginRenderPass(const RenderPass &renderPass) override;

            void BindPipelineState(const PipelineState &pipelineState) override;
            void BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) override;
            void BindIndexBuffer(const RefCounted<IndexBuffer> &buffer, IndexType indexType) override;
            void BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) override;
            void BindTexture(const RefCounted<Texture> &texture, uint32 location) override;
            void BindSampler(const RefCounted<Sampler> &sampler, uint32 location) override;
            void BindTexture(const RefCounted<Texture> &texture, uint32 location, uint32 arrayIndex) override;
            void BindSampler(const RefCounted<Sampler> &sampler, uint32 location, uint32 arrayIndex) override;

            void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) override;
            void DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) override;

            void EndRenderPass() override;
            void EndScene() override;

            bool IsInSeparateThreadMode() const override;

        private:
            enum class NodeType {
                Sequence,
                Parallel
            };

            struct NodeSync {
                NodeType type = NodeType::Sequence;
            };

            /** Binds descriptor set to the cmd buffer */
            void BindDescriptorSet();
            /** Release graph sync node */
            void ReleaseNode(NodeSync* node);
            /** @return Create new graph sync node of specified type */
            NodeSync* GetNode(NodeType type);
            /** Store resources, used in the frame (to be sure, that nothing is released while gpu processing frame)*/
            template<typename T>
            void UseResource(const RefCounted<T> &resource) {
                auto r = (RefCounted<Resource>) resource;
                mResources[mCurrentFrame % Limits::MAX_FRAMES_IN_FLIGHT].Move(r);
            }

        private:
            bool mInSceneRendering = false;
            bool mInRenderPass = false;
            bool mPipelineBound = false;

            // Frames tracking and synchronization
            ArrayFixed<HashSet<RefCounted<Resource>>, Limits::MAX_FRAMES_IN_FLIGHT> mResources;
            uint32 mCurrentFrame = 1;
            uint32 mCurrentScene = 0;

            Stack<NodeSync*> mGraphSync;
            Array<NodeSync*> mCachedSyncNodes;

            VkCommandBuffer mGraphicsCmd = nullptr;

            // Current bound state
            VulkanPipelineCache::PipelineDescriptor mPipelineDescriptor;
            VulkanFramebufferCache::RenderPassDescriptor mRenderPassDescriptor;
            VulkanFramebufferCache::RenderPassObjects mRenderPassObjects{};
            VulkanPipelineCache::PipelineObjects mPipeline{};
            VkIndexType mIndexType = VK_INDEX_TYPE_MAX_ENUM;
            VkDescriptorSet mDescriptorSet = nullptr;
            SharedPtr<Window> mWindow;
            Array<SharedPtr<Window>> mUsedWindows;                  // To track usage of windows almond scenes

            // Cache for objects used for bind/draw operations
            SharedPtr<class VulkanFramebufferCache> mFboCache;
            SharedPtr<class VulkanPipelineCache> mPipelineCache;
            SharedPtr<class VulkanDescriptorSetManager> mDescSetMan;

            // Global object for easier access (managed by device)
            class VulkanDevice& mDevice;
            class VulkanQueues& mQueues;
            class VulkanSurfaceManager& mSurfaceManager;
            class VulkanCmdBufferManager& mCmdBufferManager;
        };

    }
}

#endif //BERSERK_VULKANCONTEXT_HPP