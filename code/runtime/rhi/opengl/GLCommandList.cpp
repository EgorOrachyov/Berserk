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

#include <core/Engine.hpp>
#include <rhi/opengl/GLBuffer.hpp>
#include <rhi/opengl/GLCommandList.hpp>
#include <rhi/opengl/GLDevice.hpp>
#include <rhi/opengl/GLTexture.hpp>

#include <cassert>

BRK_NS_BEGIN

#define BRK_GL_UPDATE_BUFFER(gl_type)       \
    assert(!mInRenderPass);                 \
    assert(buffer.IsNotNull());             \
    assert(data.IsNotNull());               \
    auto native = (gl_type *) buffer.Get(); \
    native->Update(byteOffset, byteSize, data->GetData());

void GLCommandList::UpdateVertexBuffer(const Ref<RHIVertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) {
    BRK_GL_UPDATE_BUFFER(GLVertexBuffer);
}

void GLCommandList::UpdateIndexBuffer(const Ref<RHIIndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) {
    BRK_GL_UPDATE_BUFFER(GLIndexBuffer);
}

void GLCommandList::UpdateUniformBuffer(const Ref<RHIUniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) {
    BRK_GL_UPDATE_BUFFER(GLUniformBuffer);
}

#undef BRK_GL_UPDATE_BUFFER

#define BRK_GL_TEXTURE_SETUP     \
    assert(!mInRenderPass);      \
    assert(texture.IsNotNull()); \
    auto native = (GLTexture *) texture.Get();

#define BRK_GL_TEXTURE_UPDATE_SETUP \
    assert(data.IsNotNull());       \
    BRK_GL_TEXTURE_SETUP

void GLCommandList::UpdateTexture2D(const Ref<RHITexture> &texture, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) {
    BRK_GL_TEXTURE_UPDATE_SETUP;
    native->UpdateTexture2D(mipLevel, region, data);
}

void GLCommandList::UpdateTexture2DArray(const Ref<RHITexture> &texture, uint32 arrayIndex, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) {
    BRK_GL_TEXTURE_UPDATE_SETUP;
    native->UpdateTexture2DArray(arrayIndex, mipLevel, region, data);
}

void GLCommandList::UpdateTextureCube(const Ref<RHITexture> &texture, RHITextureCubemapFace face, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) {
    BRK_GL_TEXTURE_UPDATE_SETUP;
    native->UpdateTextureCube(face, mipLevel, region, data);
}

void GLCommandList::UpdateResourceSet(const Ref<RHIResourceSet> &set, const RHIResourceSetDesc &desc) {
    assert(!mInRenderPass);
    assert(set.IsNotNull());
    auto native = (GLResourceSet *) set.Get();
    native->Update(desc);
}

void GLCommandList::GenerateMipMaps(const Ref<RHITexture> &texture) {
    BRK_GL_TEXTURE_SETUP;
    native->GenerateMipMaps();
}

#undef BRK_GL_TEXTURE_UPDATE_SETUP
#undef BRK_GL_TEXTURE_SETUP

void GLCommandList::BeginRenderPass(const Ref<RHIRenderPass> &renderPass, const RHIRenderPassBeginInfo &beginInfo) {
    assert(!mInRenderPass);
    assert(renderPass.IsNotNull());

    mRenderPass = std::move(renderPass.Cast<GLRenderPass>());
    mRenderPass->Bind(mStateVars, beginInfo);
    mInRenderPass = true;
}

void GLCommandList::BindGraphicsPipeline(const Ref<RHIGraphicsPipeline> &pipeline) {
    assert(mInRenderPass);
    assert(pipeline.IsNotNull());
    assert(pipeline->GetDesc().renderPass.Get() == static_cast<RHIRenderPass *>(mRenderPass.Get()));

    PipelineCleanUp();

    mGraphicsPipeline = std::move(pipeline.Cast<GLGraphicsPipeline>());
    mGraphicsPipeline->Bind(mStateVars);
    mShader = std::move(mGraphicsPipeline->GetDesc().shader.Cast<GLShader>());
    mVaoDesc.declaration = mGraphicsPipeline->GetDesc().declaration;
    mPrimitivesType = GLDefs::GetPrimitivesType(mGraphicsPipeline->GetDesc().primitivesType);
    mPipelineBound = true;
}

void GLCommandList::BindVertexBuffers(const std::vector<Ref<RHIVertexBuffer>> &buffers) {
    assert(mPipelineBound);
    assert(!buffers.empty());
    assert(buffers.size() <= RHILimits::MAX_VERTEX_BUFFERS);

    std::fill(mVaoDesc.buffers.begin(), mVaoDesc.buffers.end(), Ref<RHIVertexBuffer>());
    std::copy(buffers.begin(), buffers.end(), mVaoDesc.buffers.begin());
    mNeedUpdateVao = true;
}

void GLCommandList::BindIndexBuffer(const Ref<RHIIndexBuffer> &buffer, RHIIndexType indexType) {
    assert(mPipelineBound);
    assert(buffer.IsNotNull());

    mVaoDesc.indices = buffer;
    mIndexType = GLDefs::GetIndexType(indexType);
    mNeedUpdateVao = true;
}

void GLCommandList::BindResourceSet(const Ref<RHIResourceSet> &resourceSet, uint32 set) {
    assert(mPipelineBound);
    assert(resourceSet.IsNotNull());
    assert(set < RHILimits::MAX_RESOURCE_SETS);

    mSets[set] = std::move(resourceSet.Cast<GLResourceSet>());
    mSets[set]->Bind(mResourceBindingState, mShader);
}

void GLCommandList::Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) {
    assert(mPipelineBound);

    if (mNeedUpdateVao) {
        mCurrentVao = mVaoCache.GetOrCreateVao(mVaoDesc);
        mNeedUpdateVao = false;
    }

    glBindVertexArray(mCurrentVao);
    BRK_GL_CATCH_ERR();

    glDrawArraysInstanced(mPrimitivesType, static_cast<GLint>(baseVertex), static_cast<GLint>(verticesCount), instancesCount);
    BRK_GL_CATCH_ERR();
}

void GLCommandList::DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) {
    assert(mPipelineBound);

    if (mNeedUpdateVao) {
        mCurrentVao = mVaoCache.GetOrCreateVao(mVaoDesc);
        mNeedUpdateVao = false;
    }

    glBindVertexArray(mCurrentVao);
    BRK_GL_CATCH_ERR();

    glDrawElementsInstancedBaseVertex(mPrimitivesType, static_cast<GLint>(indexCount), mIndexType, nullptr, static_cast<GLsizei>(instanceCount), static_cast<GLint>(baseVertex));
    BRK_GL_CATCH_ERR();
}

void GLCommandList::EndRenderPass() {
    assert(mInRenderPass);

    PipelineCleanUp();
    mStateVars.Reset();
    mInRenderPass = false;
    mPipelineBound = false;
}

void GLCommandList::SwapBuffers(const Ref<Window> &window) {
    assert(!mInRenderPass);
    auto &engine = Engine::Instance();
    auto *device = (GLDevice *) &engine.GetRHIDevice();
    device->GetSwapFunc()(window);
}

void GLCommandList::Submit() {
    // nothing to do
    mSubmitCount += 1;
    mVaoCache.GC();
}

void GLCommandList::PipelineCleanUp() {
    mResourceBindingState.Clear();
    mVaoDesc.Reset();
    mShader.Reset();
    mGraphicsPipeline.Reset();
    mSets.fill(Ref<GLResourceSet>());
    mCurrentVao = GL_NONE;
    mIndexType = GL_NONE;
    mPrimitivesType = GL_NONE;
    mNeedUpdateVao = true;
}

BRK_NS_END
