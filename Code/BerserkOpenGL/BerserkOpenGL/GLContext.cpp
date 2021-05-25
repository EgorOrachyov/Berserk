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
#include <BerserkOpenGL/GLFramebuffer.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

namespace Berserk {
    namespace RHI {

        void GLContext::BeginScene() {
            assert(!mInSceneRendering);
            mInSceneRendering = true;
        }

        void GLContext::BeginParallel() {
            // Empty, has no effect for GL-based implementation
        }

        void GLContext::EndParallel() {
            // Empty, has no effect for GL-based implementation
        }

        void GLContext::BeginSequence() {
            // Empty, has no effect for GL-based implementation
        }

        void GLContext::EndSequence() {
            // Empty, has no effect for GL-based implementation
        }

        void GLContext::UpdateVertexBuffer(const RefCounted<VertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) {
            auto native = (GLVertexBuffer*) buffer.Get();
            assert(native);
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateIndexBuffer(const RefCounted<IndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) {
            auto native = (GLIndexBuffer*) buffer.Get();
            assert(native);
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateUniformBuffer(const RefCounted<UniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const void* memory) {
            auto native = (GLUniformBuffer*) buffer.Get();
            assert(native);
            native->Update(byteOffset, byteSize, memory);
        }

        void GLContext::UpdateTexture2D(const RefCounted<Texture> &texture, uint32 mipLevel, const Math::Rect2u &region,
                                        const PixelData& memory) {
            auto native = (GLTexture*) texture.Get();
            assert(native);
            native->UpdateTexture2D(mipLevel, region, memory);
        }

        void GLContext::UpdateTexture2DArray(const RefCounted<Texture> &texture, uint32 arrayIndex, uint32 mipLevel,
                                             const Math::Rect2u &region, const PixelData& memory) {
            auto native = (GLTexture*) texture.Get();
            assert(native);
            native->UpdateTexture2DArray(arrayIndex, mipLevel, region, memory);
        }

        void GLContext::UpdateTextureCube(const RefCounted<Texture> &texture, TextureCubemapFace face, uint32 mipLevel,
                                          const Math::Rect2u &region, const PixelData &memory) {
            auto native = (GLTexture*) texture.Get();
            assert(native);
            native->UpdateTextureCube(face, mipLevel, region, memory);
        }

        void GLContext::GenerateMipMaps(const RefCounted<Texture> &texture) {
            auto native = (GLTexture*) texture.Get();
            assert(native);
            native->GenerateMipMaps();
        }

        void GLContext::BeginRenderPass(const RenderPass &renderPass, const RefCounted<Framebuffer> &renderTarget) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);
            assert(renderTarget);

            auto colorAttachments = renderTarget->GetDesc().colorTargets.GetSize();

            assert(renderPass.colorAttachments.GetSize() == colorAttachments);

            auto target = (GLFramebuffer*) renderTarget.Get();

            glBindFramebuffer(GL_FRAMEBUFFER, target->GetHandle());
            BERSERK_GL_CATCH_ERRORS();

            auto viewport = renderPass.viewport;

            glViewport(viewport.left, viewport.bottom, viewport.width, viewport.height);
            BERSERK_GL_CATCH_ERRORS();

            // Clear target buffers if need
            for (size_t i = 0; i < colorAttachments; i++) {
                if (GLDefs::NeedClearBefore(renderPass.colorAttachments[i].option)) {
                    auto& c = renderPass.colorAttachments[i].clearColor;

                    glDisable(GL_STENCIL_TEST);
                    BERSERK_GL_CATCH_ERRORS();

                    glColorMaski(i, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    BERSERK_GL_CATCH_ERRORS();

                    glClearBufferfv(GL_COLOR, i, (const GLfloat*) c.GetValues());
                    BERSERK_GL_CATCH_ERRORS();
                }
            }

            auto hasDepth = target->HasDepthBuffer();
            auto needClearDepth = hasDepth && GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.depthOption);
            auto hasStencil = target->HasStencilBuffer();
            auto needClearStencil = hasStencil && GLDefs::NeedClearBefore(renderPass.depthStencilAttachment.stencilOption);

            if (needClearDepth || needClearStencil) {
                // Enable depth writing if it not
                glDepthMask(GL_TRUE);
                BERSERK_GL_CATCH_ERRORS();

                // Enable stencil write
                glStencilMask(0xffffffff);
                BERSERK_GL_CATCH_ERRORS();

                if (needClearDepth && needClearStencil) {
                    auto stencilClear = (int32) renderPass.depthStencilAttachment.stencilClear;
                    glClearBufferfi(GL_DEPTH_STENCIL, 0, renderPass.depthStencilAttachment.depthClear, stencilClear);
                    BERSERK_GL_CATCH_ERRORS();
                } else if (needClearDepth) {
                    glClearBufferfv(GL_DEPTH, 0, &renderPass.depthStencilAttachment.depthClear);
                    BERSERK_GL_CATCH_ERRORS();
                } else {
                    auto stencilClear = (int32) renderPass.depthStencilAttachment.stencilClear;
                    glClearBufferiv(GL_DEPTH, 0, &stencilClear);
                    BERSERK_GL_CATCH_ERRORS();
                }
            }

            mInRenderPass = true;
            mBoundFboColorAttachmentsCount = colorAttachments;
            mBoundFboHasDepthBuffer = hasDepth;
            mBoundFboHasStencilBuffer = hasStencil;
        }

        void GLContext::BeginRenderPass(const RenderPass &renderPass, const SharedPtr<Window> &renderTarget) {
            assert(mInSceneRendering);
            assert(!mInRenderPass);

            assert(renderTarget);
            assert(renderPass.colorAttachments.GetSize() == 1);

            // Make context of the window current, so render pass drawing ends-up in this window
            // Assume, that we use glfw + opengl
            renderTarget->MakeContextCurrent();

            // Save window to later release unsafe usage
            mWindow = renderTarget;

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

            if (clearMask != 0) {
                glClear(clearMask);
                BERSERK_GL_CATCH_ERRORS();
            }

            mInRenderPass = true;
            mBoundFboColorAttachmentsCount = 1;
            mBoundFboHasDepthBuffer = true;
            mBoundFboHasStencilBuffer = true;
        }

        void GLContext::BindPipelineState(const PipelineState &pipelineState) {
            assert(mInRenderPass);

            assert(pipelineState.program);
            assert(pipelineState.declaration);
            assert(pipelineState.program->GetCompilationStatus() == Program::Status::Compiled);

            // Partial reset state when pipeline is bound
            mPipelineBound = true;
            mNeedUpdateVao = true;
            mVaoDesc = GLVaoCache::VaoDescriptor();

            // Also release bound objects
            mNextTextureSlotToBind = 0;
            mBoundSlots.Clear();
            mBoundTextures.Clear();
            mBoundSamplers.Clear();
            mBoundUniformBuffers.Clear();

            // Pipeline update
            mPipelineState = pipelineState;
            mVaoDesc.declaration = mPipelineState.declaration;

            // Bind shader for drawing
            mProgram = (GLProgram*) mPipelineState.program.Get();
            mProgram->Use();

            glBindVertexArray(0);
            BERSERK_GL_CATCH_ERRORS();

            // Setup the rest of the params
            auto& rstrState = pipelineState.rasterState;

            glLineWidth(rstrState.lineWidth);
            BERSERK_GL_CATCH_ERRORS();

            glFrontFace(GLDefs::GetPolygonFrontFace(rstrState.frontFace));
            BERSERK_GL_CATCH_ERRORS();

            glPolygonMode(GL_FRONT_AND_BACK, GLDefs::GetPolygonMode(rstrState.mode));
            BERSERK_GL_CATCH_ERRORS();

            if (rstrState.cullMode != PolygonCullMode::Disabled) {
                glEnable(GL_CULL_FACE);
                BERSERK_GL_CATCH_ERRORS();

                glCullFace(GLDefs::GetPolygonCullMode(rstrState.cullMode));
                BERSERK_GL_CATCH_ERRORS();
            } else {
                glDisable(GL_CULL_FACE);
                BERSERK_GL_CATCH_ERRORS();
            }

            auto& dpstState = pipelineState.depthStencilState;

            if (mBoundFboHasDepthBuffer && dpstState.depthEnable) {
                glEnable(GL_DEPTH_TEST);
                BERSERK_GL_CATCH_ERRORS();

                glDepthMask(dpstState.depthWrite ? GL_TRUE : GL_FALSE);
                BERSERK_GL_CATCH_ERRORS();

                glDepthFunc(GLDefs::GetCompareFunc(dpstState.depthCompare));
                BERSERK_GL_CATCH_ERRORS();
            } else {
                glDisable(GL_DEPTH_TEST);
                BERSERK_GL_CATCH_ERRORS();
            }

            if (mBoundFboHasStencilBuffer && dpstState.stencilEnable) {
                glEnable(GL_STENCIL_TEST);
                BERSERK_GL_CATCH_ERRORS();

                glStencilMask(dpstState.writeMask);
                BERSERK_GL_CATCH_ERRORS();

                glStencilFunc(GLDefs::GetCompareFunc(dpstState.compareFunction), dpstState.referenceValue, dpstState.compareMask);
                BERSERK_GL_CATCH_ERRORS();

                glStencilOp(GLDefs::GetStencilOp(dpstState.sfail), GLDefs::GetStencilOp(dpstState.dfail), GLDefs::GetStencilOp(dpstState.dpass));
                BERSERK_GL_CATCH_ERRORS();
            } else {
                glDisable(GL_STENCIL_TEST);
                BERSERK_GL_CATCH_ERRORS();
            }

            auto& bldState = pipelineState.blendState;
            assert(bldState.attachments.GetSize() == mBoundFboColorAttachmentsCount);

            auto enableBlend = false;

            for (size_t i = 0; i < bldState.attachments.GetSize(); i++) {
                auto& attach = bldState.attachments[i];

                if (attach.enable) {
                    enableBlend = true;
                    glBlendEquationSeparatei(i, GLDefs::GetBlendOperation(attach.colorBlendOp), GLDefs::GetBlendOperation(attach.alphaBlendOp));
                    BERSERK_GL_CATCH_ERRORS();

                    glBlendFuncSeparatei(i, GLDefs::GetBlendFactor(attach.srcColorBlendFactor), GLDefs::GetBlendFactor(attach.dstColorBlendFactor),
                                         GLDefs::GetBlendFactor(attach.srcAlphaBlendFactor), GLDefs::GetBlendFactor(attach.dstAlphaBlendFactor));
                    BERSERK_GL_CATCH_ERRORS();
                }
            }

            if (enableBlend) {
                glEnable(GL_BLEND);
                BERSERK_GL_CATCH_ERRORS();
            } else {
                glDisable(GL_BLEND);
                BERSERK_GL_CATCH_ERRORS();
            }
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

        void GLContext::BindTexture(const RefCounted<Texture> &texture, uint32 location) {
            assert(mPipelineBound);

            assert(texture);

            auto& slot = mBoundSlots[location];
            if (slot.index == UNUSED_SLOT) slot.index = mNextTextureSlotToBind++;

            mBoundTextures[location] = texture;
            auto native = (GLTexture*) texture.Get();

            assert(native->UsageSampling());
            native->Bind(location, slot.index);
        }

        void GLContext::BindSampler(const RefCounted<Sampler> &sampler, uint32 location) {
            assert(mPipelineBound);

            assert(sampler);

            auto& slot = mBoundSlots[location];
            if (slot.index == UNUSED_SLOT) slot.index = mNextTextureSlotToBind++;

            mBoundSamplers[location] = sampler;
            auto native = (GLSampler*) sampler.Get();
            native->Bind(slot.index);
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

        void GLContext::DrawIndexed(PrimitivesType primType, uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) {
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
            mProgram = nullptr;

            // Window no more used, can decrement unsafe usage
            if (mWindow) {
                mWindow->ReleaseUnsafeUsage();
                mWindow = nullptr;
            }

            // Also release bound objects
            mNextTextureSlotToBind = 0;
            mBoundSlots.Clear();
            mBoundTextures.Clear();
            mBoundSamplers.Clear();
            mBoundUniformBuffers.Clear();

            // Fbo info
            mBoundFboColorAttachmentsCount = 0;
            mBoundFboHasDepthBuffer = false;
            mBoundFboHasStencilBuffer = false;

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

        void GLContext::GC() {
            mVaoCache.GC();
        }
    }
}