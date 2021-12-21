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

#include <core/image/Image.hpp>
#include <core/image/ImageUtil.hpp>
#include <core/io/Logger.hpp>

#define STBI_WINDOWS_UTF8
#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stbimage/stb_image.hpp>
#include <stbimage/stb_image_write.hpp>

BRK_NS_BEGIN

Image::Image(uint32 width, uint32 height, Format format) {
    if (!ImageUtil::CanAccept(format)) {
        BRK_ERROR("Unsupported image format " << static_cast<int>(format));
        return;
    }
    mWidth = width;
    mHeight = height;
    mPixelSize = ImageUtil::GetPixelSize(format);
    mStride = width * mPixelSize;
    mFormat = format;
    mPixelData = Data::Make(GetSizeBytes());
}

Image::Image(uint32 width, uint32 height, uint32 stride, uint32 pixelSize, Format format, Ref<Data> pixelData)
    : mWidth(width), mHeight(height), mStride(stride), mPixelSize(pixelSize), mFormat(format), mPixelData(std::move(pixelData)) {
}

bool Image::SaveRgba(const String &filepath, FileFormat fileFormat, int quality) const {
    if (!ImageUtil::CanSaveRgba(GetFormat())) {
        BRK_ERROR("Cannot save image of this format " << static_cast<int>(GetFormat()));
        return false;
    }

    if (Empty()) {
        BRK_WARNING("Cannot save empty image " << filepath);
        return false;
    }

    auto chCount = static_cast<int>(ImageUtil::GetChannelsCount(GetFormat()));
    auto width = static_cast<int>(GetWidth());
    auto height = static_cast<int>(GetHeight());

    if (quality < 1 || quality > 100)
        quality = 20;

    switch (fileFormat) {
        case FileFormat::Bmp:
            return stbi_write_bmp(filepath.c_str(), width, height, chCount, GetPixelData()->GetData());
        case FileFormat::Png:
            return stbi_write_png(filepath.c_str(), width, height, chCount, GetPixelData()->GetData(), GetStride());
        case FileFormat::Jpg:
            return stbi_write_jpg(filepath.c_str(), width, height, chCount, GetPixelData()->GetData(), quality);
        default:
            return false;
    }
}

Image Image::LoadRgba(const String &path, uint32 channels) {
    assert(1 <= channels && channels <= 4);

    if (channels < 1 || channels > 4) {
        BRK_ERROR("Invalid channels count provided " << channels);
        return Image();
    }

    int width, height, chInFile;
    stbi_uc *data = stbi_load(path.c_str(), &width, &height, &chInFile, static_cast<int>(channels));

    if (!data) {
        BRK_ERROR("Failed to load image " << path);
        return Image();
    }

    Format format = Format::Unknown;
    switch (channels) {
        case 1:
            format = Format::R8;
            break;
        case 2:
            format = Format::RG8;
            break;
        case 3:
            format = Format::RGB8;
            break;
        case 4:
            format = Format::RGBA8;
            break;
        default:
            break;
    }

    auto pixelSize = ImageUtil::GetPixelSize(format);
    auto stride = width * pixelSize;
    auto pixelData = Data::Make(data, stride * width);

    stbi_image_free(data);

    return Image(width, height, stride, pixelSize, format, std::move(pixelData));
}

#undef STBI_WINDOWS_UTF8
#undef STBIW_WINDOWS_UTF8
#undef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_WRITE_IMPLEMENTATION

BRK_NS_END
