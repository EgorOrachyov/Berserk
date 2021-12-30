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

#ifndef BERSERK_GLCOMMANDLIST_HPP
#define BERSERK_GLCOMMANDLIST_HPP

#include <rhi/RHICommandList.hpp>
#include <rhi/RHIDefs.hpp>
#include <rhi/opengl/GLDefs.hpp>
#include <rhi/opengl/GLGraphicsPipeline.hpp>
#include <rhi/opengl/GLRenderPass.hpp>
#include <rhi/opengl/GLResourceSet.hpp>
#include <rhi/opengl/GLVaoCache.hpp>

#include <array>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class
 * @brief
 */
class GLCommandList final : public RHICommandList {
public:
    BRK_API GLCommandList() = default;
    BRK_API ~GLCommandList() override = default;

    BRK_API void UpdateVertexBuffer(const Ref<RHIVertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) override;
    BRK_API void UpdateIndexBuffer(const Ref<RHIIndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) override;
    BRK_API void UpdateUniformBuffer(const Ref<RHIUniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) override;
    BRK_API void UpdateTexture2D(const Ref<RHITexture> &texture, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) override;
    BRK_API void UpdateTexture2DArray(const Ref<RHITexture> &texture, uint32 arrayIndex, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) override;
    BRK_API void UpdateTextureCube(const Ref<RHITexture> &texture, RHITextureCubemapFace face, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) override;
    BRK_API void GenerateMipMaps(const Ref<RHITexture> &texture) override;

    BRK_API void BeginRenderPass(const Ref<RHIRenderPass> &renderPass, const RHIRenderPassBeginInfo &beginInfo) override;
    BRK_API void BindGraphicsPipeline(const Ref<RHIGraphicsPipeline> &pipeline) override;
    BRK_API void BindVertexBuffers(const std::vector<Ref<RHIVertexBuffer>> &buffers) override;
    BRK_API void BindIndexBuffer(const Ref<RHIIndexBuffer> &buffer, RHIIndexType indexType) override;
    BRK_API void BindResourceSet(const Ref<RHIResourceSet> &resourceSet, uint32 set) override;
    BRK_API void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) override;
    BRK_API void DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 instanceCount) override;
    BRK_API void EndRenderPass() override;

    BRK_API void SwapBuffers(const Ref<Window> &window) override;
    BRK_API void Submit() override;

private:
    void PipelineCleanUp();

private:
    GLResourceBindingState mResourceBindingState;
    GLRenderPassStateVars mStateVars;
    GLVaoCache mVaoCache;

    std::array<Ref<GLResourceSet>, RHILimits::MAX_RESOURCE_SETS> mSets;
    Ref<GLGraphicsPipeline> mGraphicsPipeline;
    Ref<GLRenderPass> mRenderPass;
    Ref<GLShader> mShader;
    GLVaoDescriptor mVaoDesc;
    GLuint mCurrentVao = GL_NONE;
    GLenum mIndexType = GL_NONE;
    GLenum mPrimitivesType = GL_NONE;

    bool mInRenderPass = false;
    bool mPipelineBound = false;
    bool mNeedUpdateVao = true;

    size_t mSubmitCount = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLCOMMANDLIST_HPP
