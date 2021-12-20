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

#ifndef BERSERK_RHITEXTURE_HPP
#define BERSERK_RHITEXTURE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/StringName.hpp>
#include <core/templates/Mask.hpp>
#include <rhi/RHIResource.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHITextureDesc
 * @brief RHI Texture primitive descriptor
 */
struct RHITextureDesc {
    StringName name{};
    uint32 width = 0;
    uint32 height = 0;
    uint32 depth = 0;
    uint32 mipsCount = 1;
    uint32 arraySlices = 1;
    RHITextureType textureType = RHITextureType::Unknown;
    RHITextureFormat textureFormat = RHITextureFormat::Unknown;
    Mask<RHITextureUsage> textureUsage{};
};

/**
 * @class RHITexture
 * @brief RHI Texture primitive
 */
class RHITexture : public RHIResource {
public:
    BRK_API ~RHITexture() override = default;

    /** @return Debug texture name (vk profiling tools) */
    const StringName &GetName() const { return mDesc.name; }

    /** @return Texture width in pixels */
    uint32 GetWidth() const { return mDesc.width; }

    /** @return Texture height in pixels */
    uint32 GetHeight() const { return mDesc.height; }

    /** @return Texture depth in pixels */
    uint32 GetDepth() const { return mDesc.depth; }

    /** @return Texture mip-maps count */
    uint32 GetMipsCount() const { return mDesc.mipsCount; }

    /** @return Texture array slices (more actual for array textures) */
    uint32 GetArraySlices() const { return mDesc.arraySlices; }

    /** @return Texture type */
    RHITextureType GetTextureType() const { return mDesc.textureType; }

    /** @return Texture internal storage format */
    RHITextureFormat GetTextureFormat() const { return mDesc.textureFormat; }

    /** @return Texture usage flags */
    Mask<RHITextureUsage> GetTextureUsage() const { return mDesc.textureUsage; }

    /** @return True if can use for sampling within shaders */
    bool UsageShaderSampling() const {
        return mDesc.textureUsage.Get(RHITextureUsage::Sampling);
    }

    /** @return True if can use as color attachment */
    bool UsageColorAttachment() const {
        return mDesc.textureUsage.Get(RHITextureUsage::ColorAttachment);
    }

    /** @return True if can use as depth/stencil attachment */
    bool UsageDepthStencilAttachment() const {
        return mDesc.textureUsage.Get(RHITextureUsage::DepthStencilAttachment) ||
               mDesc.textureUsage.Get(RHITextureUsage::DepthAttachment);
    }

    /** @return Texture desc */
    const RHITextureDesc &GetDesc() const { return mDesc; }

protected:
    /** Texture desc */
    RHITextureDesc mDesc;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHITEXTURE_HPP
