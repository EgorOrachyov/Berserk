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
#include <BerserkOpenGL/GLProgram.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

namespace Berserk {
    namespace RHI {

        void GLContext::BeginScene() {
            assert(!mInSceneRendering);
            mInSceneRendering = true;
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

        void GLContext::BeginRenderPass(const RenderPass &renderPass, const RefCounted<Framebuffer> &renderTarget) {

        }

        void GLContext::BeginRenderPass(const RenderPass &renderPass, const SharedPtr<Window> &renderTarget) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            assert(renderTarget);
            assert(renderPass.colorAttachments.GetSize() == 1);

            // Make context of the window current, so render pass drawing ends-up in this window
            // Assume, that we use glfw + opengl
            renderTarget->MakeContextCurrent();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            BERSERK_GL_CATCH_ERRORS();

            auto viewport = renderPass.viewport;

            glViewport(viewport.left, viewport.bottom, viewport.width, viewport.height);
            BERSERK_GL_CATCH_ERRORS();

            // Clear target buffers if need
            GLbitfield clearMask = 0;

            if (GLDefs::NeedClearBefore(renderPass.colorAttachments[0].option)) {
                auto& c= renderPass.colorAttachments[0].clearColor;
                clearMask |= GL_COLOR_BUFFER_BIT;

                glDisable(GL_STENCIL_TEST);
                BERSERK_GL_CATCH_ERRORS();

                glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                BERSERK_GL_CATCH_ERRORS();

                glClearColor(c.R(), c.G(), c.B(), c.A());
                BERSERK_GL_CATCH_ERRORS();
            }
            if (GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.depthOption)) {
                clearMask |= GL_DEPTH_BUFFER_BIT;

                glDepthMask(GL_TRUE);
                BERSERK_GL_CATCH_ERRORS();

                glClearDepthf(renderPass.depthStencilAttachment.depthClear);
                BERSERK_GL_CATCH_ERRORS();
            }
            if (GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.stencilOption)) {
                clearMask |= GL_STENCIL_BUFFER_BIT;

                glStencilMask(0xffffffff);
                BERSERK_GL_CATCH_ERRORS();

                glClearStencil(renderPass.depthStencilAttachment.stencilClear);
                BERSERK_GL_CATCH_ERRORS();
            }

            glClear(clearMask);
            BERSERK_GL_CATCH_ERRORS();

            mInRenderPass = true;
        }

        void GLContext::BindPipelineState(const PipelineState &pipelineState) {
            assert(mInRenderPass);

            assert(pipelineState.program);
            assert(pipelineState.declaration);

            // Partial reset state when pipeline is bound
            mPipelineBound = true;
            mNeedUpdateVao = true;
            mVaoDesc = GLVaoCache::VaoDescriptor();

            glBindVertexArray(0);
            BERSERK_GL_CATCH_ERRORS();

            // Pipeline update
            mPipelineState = pipelineState;
            mVaoDesc.declaration = mPipelineState.declaration;

            mProgram = (GLProgram*) mPipelineState.program.Get();
            mProgram->Use();
        }

        void GLContext::BindVertexBuffers(const ArrayFixed<RefCounted<VertexBuffer>, Limits::MAX_VERTEX_ATTRIBUTES> &buffers) {
            assert(mPipelineBound);

            mVaoDesc.buffers = buffers;
            mNeedUpdateVao = true;
        }

        void GLContext::BindIndexBuffer(const RefCounted <IndexBuffer> &buffer, IndexType indexType) {
            assert(mPipelineBound);

            mVaoDesc.indices = buffer;
            mNeedUpdateVao = true;
            mIndexType = GLDefs::GetIndexType(indexType);
        }

        void GLContext::BindUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 index, uint32 byteOffset, uint32 byteSize) {
            assert(mPipelineBound);

            assert(buffer);

            mBoundUniformBuffers[index] = buffer;
            auto native = (GLUniformBuffer*) buffer.Get();
            native->Bind(index, byteOffset, byteSize);
            mProgram->BindUniformBlock(index);
        }

        void GLContext::BindTexture(const RefCounted<Texture> &texture, uint32 slot) {
            assert(mPipelineBound);

            assert(texture);

            mBoundTextures[slot] = texture;
            auto native = (GLTexture*) texture.Get();
            native->Bind(slot, slot);
        }

        void GLContext::BindSampler(const RefCounted<Sampler> &sampler, uint32 slot) {
            assert(mPipelineBound);

            assert(sampler);

            mBoundSamplers[slot] = sampler;
            auto native = (GLSampler*) sampler.Get();
            native->Bind(slot);
        }

        void GLContext::Draw(PrimitivesType primType, uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {
            assert(mPipelineBound);

            if (mNeedUpdateVao) {
                mCurrentVao = mVaoCache.GetOrCreateVao(mVaoDesc);
                mNeedUpdateVao = false;
            }

            mPrimitivesType = GLDefs::GetPrimitivesType(primType);

            glBindVertexArray(mCurrentVao);
            BERSERK_GL_CATCH_ERRORS();

            glDrawArraysInstanced(mPrimitivesType, baseVertex, verticesCount, instancesCount);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLContext::DrawIndexed(PrimitivesType primType, uint32 indexCount, uint32 baseVertex, uint32 baseIndex,
                                    uint32 instanceCount) {
            assert(mPipelineBound);

            if (mNeedUpdateVao) {
                mCurrentVao = mVaoCache.GetOrCreateVao(mVaoDesc);
                mNeedUpdateVao = false;
            }

            mPrimitivesType = GLDefs::GetPrimitivesType(primType);

            glBindVertexArray(mCurrentVao);
            BERSERK_GL_CATCH_ERRORS();

            glDrawElementsInstancedBaseVertex(mPrimitivesType, indexCount, mIndexType, nullptr, instanceCount, baseVertex);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLContext::EndRenderPass() {
            assert(mInRenderPass);

            // Reset state when pass is ended
            mInRenderPass = false;
            mPipelineBound = false;
            mNeedUpdateVao = true;
            mVaoDesc = GLVaoCache::VaoDescriptor();
            mPipelineState = PipelineState();

            // Also release bound objects
            mBoundTextures.Clear();
            mBoundSamplers.Clear();
            mBoundUniformBuffers.Clear();

            glUseProgram(0);
            BERSERK_GL_CATCH_ERRORS();

            glBindVertexArray(0);
            BERSERK_GL_CATCH_ERRORS();
        }

        void GLContext::EndScene() {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            mInSceneRendering = false;
        }

        bool GLContext::IsInSeparateThreadMode() const {
            return true;
        }
    }
}