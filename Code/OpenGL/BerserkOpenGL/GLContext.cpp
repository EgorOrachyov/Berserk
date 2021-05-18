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

#include <BerserkOpenGL/GLContext.hpp>
#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLUniformBuffer.hpp>
#include <BerserkOpenGL/GLSampler.hpp>
#include <BerserkOpenGL/GLTexture.hpp>

namespace Berserk {
    namespace RHI {

        void GLContext::BeginScene() {

        }

        void GLContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            auto native = (GLVertexBuffer*) buffer.Get();
            assert(native);
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            auto native = (GLIndexBuffer*) buffer.Get();
            assert(native);
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            auto native = (GLUniformBuffer*) buffer.Get();
            assert(native);
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region,
                                        const RefCounted<PixelData> &memory) {
            auto native = (GLTexture*) texture.Get();
            assert(native);
            native->UpdateTexture2D(mipLevel, region, memory);
        }

        void GLContext::GenerateMipMaps(const RefCounted<Texture> &texture) {
            auto native = (GLTexture*) texture.Get();
            assert(native);
            native->GenerateMipMaps();
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

        void GLContext::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {

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

        void GLContext::EndScene() {

        }

        bool GLContext::IsInSeparateThreadMode() const {
            return true;
        }
    }
}