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

#ifndef BERSERK_IMAGE_HPP
#define BERSERK_IMAGE_HPP

#include <core/Config.hpp>
#include <core/Data.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>
#include <rhi/RHIDefs.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Image
 * @brief Hardware-independent image representation.
 *
 * Image class allows to load/save image data, get access
 * to the pixels, manipulate, transform, change data,
 * pass pixel data to rendering-hardware for the rendering.
 */
class Image {
public:
    /**
     * @class Format
     * @brief Format of the image stored data
     */
    using Format = RHITextureFormat;

    /** @brief Formats to save image */
    enum class FileFormat {
        Png,
        Jpg,
        Bmp
    };

    BRK_API Image() = default;
    BRK_API Image(uint32 width, uint32 height, Format format);
    BRK_API Image(uint32 width, uint32 height, uint32 stride, uint32 pixelSize, Format format, Ref<Data> pixelData);
    BRK_API Image(const Image &) = default;
    BRK_API Image(Image &&) noexcept = default;
    BRK_API ~Image() = default;

    BRK_API Image &operator=(const Image &) = default;
    BRK_API Image &operator=(Image &&) noexcept = default;

    bool Empty() const { return mWidth * mHeight == 0; }

    uint32 GetWidth() const { return mWidth; }
    uint32 GetHeight() const { return mHeight; }
    uint32 GetStride() const { return mStride; }
    uint32 GetPixelSize() const { return mPixelSize; }
    uint32 GetSizeBytes() const { return mStride * mHeight; }
    Format GetFormat() const { return mFormat; }
    const Ref<Data> &GetPixelData() const { return mPixelData; }

    /**
     * @brief Save rgba image to the file
     *
     * @note Use engine file system class to retrieve correct and full path to file.
     *
     * @param filepath Path to the file to save
     * @param fileFormat Image format to save
     * @param quality Image quality; must be within [1..100]
     *
     * @return True if saved
     */
    BRK_API bool SaveRgba(const String &filepath, FileFormat fileFormat, int quality = 100) const;

    /**
     * @brief Load rgba image from the disc
     *
     * @note Use engine file system class to retrieve correct and full path to file.
     *
     * @param path Path to the image file
     * @param channels Number of channels to load; must be within {1,2,3,4}
     *
     * @return Loaded image; empty if failed
     */
    BRK_API static Image LoadRgba(const String &path, uint32 channels = 4);

private:
    uint32 mWidth = 0;
    uint32 mHeight = 0;
    uint32 mStride = 0;
    uint32 mPixelSize = 0;
    Format mFormat = Format::Unknown;
    Ref<Data> mPixelData;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_IMAGE_HPP