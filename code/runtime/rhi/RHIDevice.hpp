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

#ifndef BERSERK_RHIDEVICE_HPP
#define BERSERK_RHIDEVICE_HPP

#include <core/math/TMatMxN.hpp>

#include <rhi/RHIBuffer.hpp>
#include <rhi/RHICommandList.hpp>
#include <rhi/RHIDefs.hpp>
#include <rhi/RHIFramebuffer.hpp>
#include <rhi/RHIGraphicsPipeline.hpp>
#include <rhi/RHIRenderPass.hpp>
#include <rhi/RHIResource.hpp>
#include <rhi/RHIResourceSet.hpp>
#include <rhi/RHISampler.hpp>
#include <rhi/RHIShader.hpp>
#include <rhi/RHITexture.hpp>
#include <rhi/RHIVertexDeclaration.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIDevice
 * @brief RHI device
 *
 * Device object exposes common object creation api.
 *
 * Device provided objects can be safely created from any thread. Create functions
 * immediately return the created object reference, but actual object creation
 * on the GPU side is deferred, until the RHI execution thread reaches
 * object creation and does it its side.
 *
 * From user side, created objects can be safely and immediately used in
 * the drawing and update api via command buffers.
 */
class RHIDevice {
public:
    BRK_API virtual ~RHIDevice() = default;

    /** @return Create vertex declaration from desc */
    BRK_API virtual Ref<RHIVertexDeclaration> CreateVertexDeclaration(const RHIVertexDeclarationDesc &desc) = 0;

    /** @return Create vertex buffer from desc */
    BRK_API virtual Ref<RHIVertexBuffer> CreateVertexBuffer(const RHIBufferDesc &desc) = 0;

    /** @return Create vertex buffer from desc */
    BRK_API virtual Ref<RHIIndexBuffer> CreateIndexBuffer(const RHIBufferDesc &desc) = 0;

    /** @return Create vertex buffer from desc */
    BRK_API virtual Ref<RHIUniformBuffer> CreateUniformBuffer(const RHIBufferDesc &desc) = 0;

    /** @return Create sampler object from desc */
    BRK_API virtual Ref<RHISampler> CreateSampler(const RHISamplerDesc &desc) = 0;

    /** @return Create texture from desc */
    BRK_API virtual Ref<RHITexture> CreateTexture(const RHITextureDesc &desc) = 0;

    /** @return Create resource set from desc */
    BRK_API virtual Ref<RHIResourceSet> CreateResourceSet(const RHIResourceSetDesc &desc) = 0;

    /** @return Create framebuffer from desc */
    BRK_API virtual Ref<RHIFramebuffer> CreateFramebuffer(const RHIFramebufferDesc &desc) = 0;

    /** @return Create and compile shader from desc */
    BRK_API virtual Ref<RHIShader> CreateShader(const RHIShaderDesc &desc) = 0;

    /** @return Create render pass from desc */
    BRK_API virtual Ref<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc &desc) = 0;

    /** @return Create and compile pipeline from desc */
    BRK_API virtual Ref<RHIGraphicsPipeline> CreateGraphicsPipeline(const RHIGraphicsPipelineDesc &desc) = 0;

    /** Update vertex buffer with data */
    BRK_API virtual void UpdateVertexBuffer(const Ref<RHIVertexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data);

    /** Update index buffer with data */
    BRK_API virtual void UpdateIndexBuffer(const Ref<RHIIndexBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data);

    /** Update uniform buffer with data */
    BRK_API virtual void UpdateUniformBuffer(const Ref<RHIUniformBuffer> &buffer, uint32 byteOffset, uint32 byteSize, const Ref<Data> &data);

    /** Update chosen region and mip level of 2d texture */
    BRK_API virtual void UpdateTexture2D(const Ref<RHITexture> &texture, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data);

    /** Update chosen array index, region and mip level of 2d texture array */
    BRK_API virtual void UpdateTexture2DArray(const Ref<RHITexture> &texture, uint32 arrayIndex, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data);

    /** Update chosen face, region and mip level of cube texture */
    BRK_API virtual void UpdateTextureCube(const Ref<RHITexture> &texture, RHITextureCubemapFace face, uint32 mipLevel, const Rect2u &region, const Ref<Data> &data);

    /** Update resource set */
    BRK_API virtual void UpdateResourceSet(const Ref<RHIResourceSet> &set, const RHIResourceSetDesc &desc);

    /** Generate mip maps for the texture (2d, 2d array, cube) */
    BRK_API virtual void GenerateMipMaps(const Ref<RHITexture> &texture);

    /** @return Core command list for commands capturing */
    BRK_API virtual Ref<RHICommandList> GetCoreCommandList() = 0;

    /** @return List of supported texture formats */
    BRK_API virtual const std::vector<RHITextureFormat> &GetSupportedFormats() const;

    /** @return List of supported source shader languages */
    BRK_API virtual const std::vector<RHIShaderLanguage> &GetSupportedShaderLanguages() const;

    /** @return Type of underlying native API */
    BRK_API virtual RHIType GetDriverType() const;

    /** @return Device capabilities */
    BRK_API virtual const RHIDeviceCaps &GetCaps() const;

    /** @return Clip matrix */
    BRK_API virtual const Mat4x4f &GetClipMatrix() const;

    /** @return True if language supported */
    BRK_API bool IsSupported(RHIShaderLanguage language) const;

protected:
    std::vector<RHITextureFormat> mSupportedTextureFormats;
    std::vector<RHIShaderLanguage> mSupportedShaderLanguages;
    RHIDeviceCaps mCaps;
    Mat4x4f mClipMatrix;
    RHIType mType;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIDEVICE_HPP
