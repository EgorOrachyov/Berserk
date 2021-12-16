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
#include <rhi/RHIDefs.hpp>
#include <rhi/RHIFramebuffer.hpp>
#include <rhi/RHIGraphicsPipeline.hpp>
#include <rhi/RHIRenderPass.hpp>
#include <rhi/RHIResource.hpp>
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

    /** @return Create framebuffer from desc */
    BRK_API virtual Ref<RHIFramebuffer> CreateFramebuffer(const RHIFramebufferDesc &desc) = 0;

    /** @return Create and compile shader from desc */
    BRK_API virtual Ref<RHIShader> CreateShader(const RHIShaderDesc &desc) = 0;

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
