/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLCONTEXT_HPP
#define BERSERK_GLCONTEXT_HPP

#include <BerserkRHI/RHIContext.hpp>

namespace Berserk {
    namespace RHI {

        class GLContext final : public Context {
        public:
            ~GLContext() override = default;

            void UpdateVertexBuffer(const Ref<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize,
                                    const Ref<MemoryBuffer> &memory) override;

            void UpdateIndexBuffer(const Ref<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize,
                                   const Ref<MemoryBuffer> &memory) override;

            void UpdateUniformBuffer(const Ref<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize,
                                     const Ref<MemoryBuffer> &memory) override;

            void UpdateTexture2D(const Ref<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region,
                                 const Ref<MemoryBuffer> &memory) override;

            void GenerateMipMaps(const Ref<Texture> &texture) override;

            void BeginRenderPass(const RenderPass &renderPass, const Ref<RenderTarget> &renderTarget) override;

            void BeginRenderPass(const RenderPass &renderPass, const Ref<Window> &renderTarget) override;

            void BindPipelineState(const PipelineState &pipelineState) override;

            void
            BindVertexBuffers(const ArrayFixed<Ref<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) override;

            void BindIndexBuffer(const Ref<IndexBuffer> &buffer) override;

            void BindUniformBuffer(const Ref<UniformBuffer> &buffer, uint32 index, uint32 byteOffset,
                                   uint32 byteSize) override;

            void BindTexture(const Ref<Texture> &texture, uint32 index) override;

            void BindSampler(const Ref<Sampler> &sampler, uint32 index) override;

            void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount, uint32 baseInstance) override;

            void DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount, uint32 baseInstance) override;

            void EndRenderPass() override;

            bool IsInSeparateThreadMode() const override;
        };

    }
}

#endif //BERSERK_GLCONTEXT_HPP