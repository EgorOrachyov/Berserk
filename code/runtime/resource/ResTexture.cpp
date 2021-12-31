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
#include <core/image/ImageUtil.hpp>
#include <core/io/Logger.hpp>
#include <resource/ResTexture.hpp>

BRK_NS_BEGIN

const StringName &ResTexture::GetResourceType() const {
    return GetResourceTypeStatic();
}

const StringName &ResTexture::GetResourceTypeStatic() {
    static StringName resourceType(BRK_TEXT("_brk_resource_texture"));
    return resourceType;
}

void ResTexture::CreateFromImage(const Image &image, bool mipmaps, bool cache) {
    if (image.Empty()) {
        BRK_ERROR("an attempt to create texture from empty image");
        return;
    }
    if (cache) {
        mImage = image;
    }

    mWidth = image.GetWidth();
    mHeight = image.GetHeight();
    mFormat = image.GetFormat();
    mMipmaps = mipmaps;

    auto &device = Engine::Instance().GetRHIDevice();

    RHITextureDesc textureDesc{};
    textureDesc.name = GetName();
    textureDesc.width = GetWidth();
    textureDesc.height = GetHeight();
    textureDesc.depth = 1;
    textureDesc.mipsCount = ImageUtil::GetMaxMipsCount(GetWidth(), GetHeight(), 1);
    textureDesc.arraySlices = 1;
    textureDesc.textureType = RHITextureType::Texture2d;
    textureDesc.textureFormat = GetFormat();
    textureDesc.textureUsage = {RHITextureUsage::Sampling};
    mRHITexture = device.CreateTexture(textureDesc);

    // Upload actual data
    device.UpdateTexture2D(mRHITexture, 0, {0, 0, image.GetWidth(), image.GetHeight()}, image.GetPixelData());

    // Generate mip maps if required
    if (MipMaps())
        device.GenerateMipMaps(mRHITexture);
}

void ResTexture::SetSampler(Ref<RHISampler> sampler) {
    mRHISampler = std::move(sampler);
}

BRK_NS_END