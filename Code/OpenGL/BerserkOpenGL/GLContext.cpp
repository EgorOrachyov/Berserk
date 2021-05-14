/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLContext.hpp>
#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLUniformBuffer.hpp>
#include <BerserkOpenGL/GLSampler.hpp>

namespace Berserk {
    namespace RHI {

        void GLContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            auto native = dynamic_cast<GLVertexBuffer*>(buffer.Get());
            assert(buffer.IsNotNull());
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            auto native = dynamic_cast<GLIndexBuffer*>(buffer.Get());
            assert(buffer.IsNotNull());
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            auto native = dynamic_cast<GLUniformBuffer*>(buffer.Get());
            assert(buffer.IsNotNull());
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region,
                                        const RefCounted<ReadOnlyMemoryBuffer> &memory) {

        }

        void GLContext::GenerateMipMaps(const RefCounted<Texture> &texture) {

        }

        void GLContext::BeginRenderPass(const RenderPass &renderPass, const RefCounted<RenderTarget> &renderTarget) {

        }

        void GLContext::BeginRenderPass(const RenderPass &renderPass, const SharedPtr<Window> &renderTarget) {

        }

        void GLContext::BindPipelineState(const PipelineState &pipelineState) {

        }

        void GLContext::BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {

        }

        void GLContext::BindIndexBuffer(const RefCounted<IndexBuffer> &buffer) {

        }

        void GLContext::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset,
                                          uint32 byteSize) {

        }

        void GLContext::BindTexture(const RefCounted<Texture> &texture, uint32 index) {

        }

        void GLContext::BindSampler(const RefCounted<Sampler> &sampler, uint32 index) {

        }

        void GLContext::Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount, uint32 baseInstance) {

        }

        void GLContext::DrawIndexed(uint32 indexCount, uint32 baseIndex, uint32 instanceCount, uint32 baseInstance) {

        }

        void GLContext::EndRenderPass() {

        }

        bool GLContext::IsInSeparateThreadMode() const {
            return true;
        }
    }
}