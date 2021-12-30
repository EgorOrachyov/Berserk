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
#include <core/io/Logger.hpp>
#include <resource/ResTexture.hpp>
#include <resource/importers/TextureImporter.hpp>

BRK_NS_BEGIN

TextureImporter::TextureImporter() {
    mExtensions.emplace_back(BRK_TEXT("png"));
    mExtensions.emplace_back(BRK_TEXT("jpg"));
    mExtensions.emplace_back(BRK_TEXT("jpeg"));
    mExtensions.emplace_back(BRK_TEXT("bmp"));
    mExtensions.emplace_back(BRK_TEXT("gif"));
    mExtensions.emplace_back(BRK_TEXT("tga"));
    mExtensions.emplace_back(BRK_TEXT("pic"));
    mExtensions.emplace_back(BRK_TEXT("ppm"));
    mExtensions.emplace_back(BRK_TEXT("pgm"));
}

Ref<ResourceImportOptions> TextureImporter::CreateDefaultOptions() const {
    return Ref<ResourceImportOptions>(new ResTextureImportOptions);
}

const std::vector<String> &TextureImporter::GetSupportedExtensions() const {
    return mExtensions;
}

void TextureImporter::Import(const String &fullpath, const Ref<ResourceImportOptions> &options, ResourceImportResult &result) {
    auto ops = options.Cast<ResTextureImportOptions>();

    if (ops.IsNull()) {
        BRK_ERROR("Provided invalid options for resource " << fullpath);
        ops = Ref<ResTextureImportOptions>(new ResTextureImportOptions);
    }

    auto image = Image::LoadRgba(fullpath, ops->channels);

    if (image.Empty()) {
        result.failed = true;
        result.error = BRK_TEXT("Failed load rgba image");
        return;
    }

    if (ops->width > 0 || ops->height > 0) {
        image = image.Resize(ops->width > 0 ? ops->width : image.GetWidth(), ops->height > 0 ? ops->height : image.GetHeight());
    }

    Ref<ResTexture> texture(new ResTexture());
    texture->Create(image, ops->mipmaps, ops->cacheCPU);

    // todo: remove
    RHISamplerDesc samplerDesc;
    samplerDesc.minFilter = RHISamplerMinFilter::LinearMipmapLinear;
    samplerDesc.magFilter = RHISamplerMagFilter::Linear;
    samplerDesc.maxAnisotropy = 16.0f;
    samplerDesc.useAnisotropy = true;
    samplerDesc.u = RHISamplerRepeatMode::Repeat;
    samplerDesc.v = RHISamplerRepeatMode::Repeat;
    samplerDesc.w = RHISamplerRepeatMode::Repeat;
    auto sampler = Engine::Instance().GetRHIDevice().CreateSampler(samplerDesc);

    texture->SetSampler(sampler);
    result.resource = texture.As<Resource>();
}


BRK_NS_END
