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

#include <BerserkCore/Templates/MemoryBuffer.hpp>
#include <BerserkCore/Image/PixelData.hpp>
#include <BerserkCore/Image/Color.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Templates/Contracts.hpp>

namespace Berserk {

    /**
     * @brief Image class
     *
     * Represents 2d image.
     */
    class Image {
    public:

        enum class PixelFormat {
            Unknown = 0         ,
            R8                  ,
            R8G8                ,
            R8G8B8              ,
            R8G8B8A8
        };

        static const uint64 PixelFormatCount = 5;

        enum class Channels {
            Unknown,
            R,
            RG,
            RGB,
            RGBA
        };

        enum class SaveFormat {
            Unknown,
            Bmp,
            Jpg,
            Png
        };

        Image() = default;

        /**
         * Create image from data.
         *
         * @param width Image width in pixels
         * @param height Image height in pixels
         * @param pixelFormat Format of the pixels
         * @param buffer Actual memory buffer with data
         */
        Image(uint32 width, uint32 height, PixelFormat pixelFormat, RefCounted<ReadOnlyMemoryBuffer> buffer);
        ~Image() = default;

        /** @return Image memory buffer */
        RefCounted<ReadOnlyMemoryBuffer> GetBufferRef() const { return mBuffer; }

        /** @return Image width in pixels */
        uint32 GetWidth() const { return mWidth; }

        /** @return Image height in pixels */
        uint32 GetHeight() const { return mHeight; }

        /** @return Pixels format */
        PixelFormat GetPixelFormat() const { return mPixelFormat; }

        /** @return True if image empty */
        bool IsEmpty() const { return mWidth * mHeight == 0; }

        /**
        * Saves image in the specified file path.
        *
        * @param filepath Path where to save the image, with bmp extension.
        */
        bool SaveBmp(const String& filepath) const;

        /**
         * Saves image in the specified file path.
         *
         * @warning Not recommended
         * @param filepath Path where to save the image, with png extension.
         */
        bool SavePng(const String& filepath) const;

        /**
         * Saves image in the specified file path.
         *
         * @param filepath Path where to save the image, with jpg or jpeg extension.
         * @param quality Quality of the result image [1..100], 100 - high quality, 1 - lowest quality.
         */
        bool SaveJpg(const String& filepath, uint32 quality = 20) const;

        /**
         * Load image from file.
         *
         * @param filepath Path to file with image
         * @param channels Desired number of channels to load
         *
         * @return Load image on success, or empty image
         */
        static Image Load(const String& filepath, Channels channels = Channels::RGBA);

        /**
         * Create image with specified properties.
         *
         * @param width Image width in pixels
         * @param height Image height in pixels
         * @param color Image color to fill
         *
         * @return Created image
         */
        static Image Create(uint32 width, uint32 height, const Color& color);

        /** @return Size of single pixel in bytes */
        static uint32 GetPixelSize(PixelFormat format);

        /** @return Num of channels */
        static uint32 GetChannelsCount(Channels channels);

        /** @return Channels mode */
        static Channels GetChannelsFromCount(uint32 count);

        /** @return Channels mode */
        static Channels GetChannelsFromPixelFormat(PixelFormat format);

        /** @return True, if can save images of this format to the disc */
        static bool IsSupportedFormatForSaving(PixelFormat format);

    protected:
        RefCounted<ReadOnlyMemoryBuffer> mBuffer;
        uint32 mWidth = 0;
        uint32 mHeight = 0;
        PixelFormat mPixelFormat = PixelFormat::Unknown;
    };

    template<>
    class TextPrint<Image::PixelFormat> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, Image::PixelFormat format) {
            const String::Char8u* formats[] = {
                BERSERK_TEXT("Unknown"),
                BERSERK_TEXT("R8"),
                BERSERK_TEXT("R8G8"),
                BERSERK_TEXT("R8G8B8"),
                BERSERK_TEXT("R8G8B8A8")
            };

            auto id = (uint32) format < Image::PixelFormatCount? (uint32) format: 0;
            stream.Add(formats[id]);
        }
    };

}

#endif //BERSERK_IMAGE_HPP