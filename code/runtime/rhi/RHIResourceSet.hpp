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

#ifndef BERSERK_RHIRESOURCESET_HPP
#define BERSERK_RHIRESOURCESET_HPP

#include <core/Config.hpp>
#include <core/Data.hpp>
#include <core/Typedefs.hpp>
#include <rhi/RHIBuffer.hpp>
#include <rhi/RHIResource.hpp>
#include <rhi/RHISampler.hpp>
#include <rhi/RHITexture.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIResourceSetDesc
 * @brief Describes single set of GPU resource for shader
 */
class RHIResourceSetDesc {
public:
    struct TextureBinding {
        Ref<RHITexture> texture{};
        uint32 location{};
        uint32 arrayIndex{};
    };

    struct SamplerBinding {
        Ref<RHISampler> sampler{};
        uint32 location{};
        uint32 arrayIndex{};
    };

    struct BufferBinding {
        Ref<RHIUniformBuffer> buffer{};
        uint32 location{};
        uint32 offset{};
        uint32 range{};
    };

    BRK_API RHIResourceSetDesc() = default;
    BRK_API ~RHIResourceSetDesc() = default;

    /** Add texture binding to the set */
    BRK_API void AddTexture(Ref<RHITexture> texture, uint32 location, uint32 arrayIndex = 0);
    /** Add sampler binding to the set */
    BRK_API void AddSampler(Ref<RHISampler> sampler, uint32 location, uint32 arrayIndex = 0);
    /** Add buffer binding to the set */
    BRK_API void AddBuffer(Ref<RHIUniformBuffer> buffer, uint32 location, uint32 offset, uint32 range);


    /** Update (or add new) previously set texture binding in the set */
    BRK_API bool SetTexture(Ref<RHITexture> texture, uint32 location, uint32 arrayIndex = 0);
    /** Update (or add new) previously set sampler binding in the set */
    BRK_API bool SetSampler(Ref<RHISampler> sampler, uint32 location, uint32 arrayIndex = 0);
    /** Update (or add new) previously set buffer binding in the set */
    BRK_API bool SetBuffer(Ref<RHIUniformBuffer> buffer, uint32 location, uint32 offset, uint32 range);

    /** Clear all bindings */
    BRK_API void Clear();

    /** @return Texture bindings */
    BRK_API const std::vector<TextureBinding> &GetTextures() const { return mTextures; }
    /** @return Sampler bindings */
    BRK_API const std::vector<SamplerBinding> &GetSamplers() const { return mSamplers; };
    /** @return Buffer bindings */
    BRK_API const std::vector<BufferBinding> &GetBuffers() const { return mBuffers; };

private:
    std::vector<TextureBinding> mTextures;
    std::vector<SamplerBinding> mSamplers;
    std::vector<BufferBinding> mBuffers;
};

/**
 * @class RHIResourceSet
 * @brief Set of resource ready to be bound to the pipeline
 */
class RHIResourceSet : public RHIResource {
public:
    BRK_API ~RHIResourceSet() override = default;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIRESOURCESET_HPP
