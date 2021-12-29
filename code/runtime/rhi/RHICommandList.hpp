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

#ifndef BERSERK_RHICOMMANDLIST_HPP
#define BERSERK_RHICOMMANDLIST_HPP

#include <core/Config.hpp>
#include <core/Data.hpp>
#include <core/Typedefs.hpp>
#include <rhi/RHIBuffer.hpp>
#include <rhi/RHIGraphicsPipeline.hpp>
#include <rhi/RHIRenderPass.hpp>
#include <rhi/RHIResource.hpp>
#include <rhi/RHIResourceSet.hpp>
#include <rhi/RHITexture.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHICommandList
 * @brief Command list for commands capturing and submission
 *
 * @warning Must be accessed on render thread only
 */
class RHICommandList : public RHIResource {
public:
    BRK_API ~RHICommandList() override = default;

    /** Update vertex buffer with data */
    BRK_API virtual void UpdateVertexBuffer(const Ref<RHIVertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) = 0;

    /** Update index buffer with data */
    BRK_API virtual void UpdateIndexBuffer(const Ref<RHIIndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) = 0;

    /** Update uniform buffer with data */
    BRK_API virtual void UpdateUniformBuffer(const Ref<RHIUniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data) = 0;

    /** Update chosen region and mip level of 2d texture */
    BRK_API virtual void UpdateTexture2D(const Ref<RHITexture> &texture, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) = 0;

    /** Update chosen array index, region and mip level of 2d texture array */
    BRK_API virtual void UpdateTexture2DArray(const Ref<RHITexture> &texture, uint32 arrayIndex, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) = 0;

    /** Update chosen face, region and mip level of cube texture */
    BRK_API virtual void UpdateTextureCube(const Ref<RHITexture> &texture, RHITextureCubemapFace face, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data) = 0;

    /** Generate mip maps for the texture (2d, 2d array, cube) */
    BRK_API virtual void GenerateMipMaps(const Ref<RHITexture> &texture) = 0;

    /** Begin render pass for drawing (must be followed with end call)*/
    BRK_API virtual void BeginRenderPass(const Ref<RHIRenderPass> &renderPass, const RHIRenderPassBeginInfo &beginInfo) = 0;

    /** Bind compiled graphics pipeline for the rendering (must be called inside render pass) */
    BRK_API virtual void BindGraphicsPipeline(const Ref<RHIGraphicsPipeline> &pipeline) = 0;

    /** Bind vertex buffers (must be called inside render pass) */
    BRK_API virtual void BindVertexBuffers(const std::vector<Ref<RHIVertexBuffer>> &buffers) = 0;

    /** Bind index buffer (must be called inside render pass) */
    BRK_API virtual void BindIndexBuffer(const Ref<RHIIndexBuffer> &buffer, RHIIndexType indexType) = 0;

    /** Bind resource set to specified set index (must be called inside render pass) */
    BRK_API virtual void BindResourceSet(const Ref<RHIResourceSet> &resourceSet, uint32 set) = 0;

    /** Issue draw call for vertex data only */
    BRK_API virtual void Draw(uint32 verticesCount, uint32 baseVertex, uint32 instancesCount) = 0;

    /** Issue draw call for indexed data rendering only */
    BRK_API virtual void DrawIndexed(uint32 indexCount, uint32 baseVertex, uint32 baseIndex, uint32 instanceCount) = 0;

    /** End render pass (must be called in the end after begin render pass) */
    BRK_API virtual void EndRenderPass() = 0;

    /** Request buffers swap */
    BRK_API virtual void SwapBuffers(const Ref<Window> &window) = 0;

    /** Submit list for execution */
    BRK_API virtual void Submit() = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHICOMMANDLIST_HPP
