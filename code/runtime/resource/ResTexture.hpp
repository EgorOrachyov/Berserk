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

#ifndef BERSERK_RESTEXTURE_HPP
#define BERSERK_RESTEXTURE_HPP

#include <core/image/Image.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceImporter.hpp>

#include <rhi/RHISampler.hpp>
#include <rhi/RHITexture.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

class ResTextureImportOptions : public ResourceImportOptions {
public:
    BRK_API ResTextureImportOptions() = default;
    BRK_API ~ResTextureImportOptions() override = default;

    int width = -1;        /** Desired width; -1 use native from file */
    int height = -1;       /** Desired height; -1 use native from file */
    bool mipmaps = false;  /** Generate mip maps for texture */
    bool cacheCPU = false; /** Cache loaded image data on cpu */
    uint32 channels = 4;   /** Number of color channels to load */
};

/**
 * @class ResTexture
 * @brief 2d texture resource for rendering
 */
class ResTexture final : public Resource {
public:
    BRK_API ResTexture() = default;
    BRK_API ~ResTexture() override = default;

    BRK_API const StringName &GetResourceType() const override;
    BRK_API static const StringName &GetResourceTypeStatic();

    BRK_API void Create(const Image &image, bool mipmaps, bool cache);
    BRK_API void SetSampler(Ref<RHISampler> sampler);

    BRK_API uint32 GetWidth() const { return mWidth; }
    BRK_API uint32 GetHeight() const { return mHeight; }
    BRK_API Image::Format GetFormat() const { return mFormat; }
    BRK_API bool MipMaps() const { return mMipmaps; }

    BRK_API const Image &GetImage() const { return mImage; }
    BRK_API const Ref<RHITexture> &GetRHITexture() const { return mRHITexture; };
    BRK_API const Ref<RHISampler> &GetRHISampler() const { return mRHISampler; };

private:
    Ref<RHITexture> mRHITexture; /** GPU texture handle */
    Ref<RHISampler> mRHISampler; /** GPU sampler to filter texture */

    Image mImage;                                   /** Cached CPU data; may be empty */
    Image::Format mFormat = Image::Format::Unknown; /** Data format */

    uint32 mWidth = 0;     /** Size in pixels */
    uint32 mHeight = 0;    /** Size in pixels */
    bool mMipmaps = false; /** If mip maps generated */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESTEXTURE_HPP
