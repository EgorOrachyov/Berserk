/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_IMAGE_HPP
#define BERSERK_IMAGE_HPP

#include <BerserkCore/Memory/MemoryBuffer.hpp>
#include <BerserkCore/Image/Color.hpp>

namespace Berserk {

    /**
     * @brief Image class
     *
     * Represents 2d image.
     */
    class Image {
    public:

        enum class PixelFormat {
            Unknown             ,
            R8                  ,
            R8G8B8A8            ,
            DepthStencil        ,
            R16G16B16A16f
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
        Image(size_t width, size_t height, PixelFormat pixelFormat, Ref<MemoryBuffer> buffer);

        ~Image() = default;

        /** @return Image memory buffer */
        Ref<MemoryBuffer> GetBufferRef() const { return mBuffer; }

        /** @return Image width in pixels */
        size_t GetWidth() const { return mWidth; }

        /** @return Image height in pixels */
        size_t GetHeight() const { return mHeight; }

        /** @return Pixels format */
        PixelFormat GetPixelFormat() const { return mPixelFormat; }

        /** @return Image in rgba format with specified size and color */
        static Image Create(size_t width, size_t height, const Color& color);

        /** @return Size of single pixel in bytes */
        static size_t GetPixelSize(PixelFormat format);

    protected:
        Ref<MemoryBuffer> mBuffer;
        size_t mWidth = 0;
        size_t mHeight = 0;
        PixelFormat mPixelFormat = PixelFormat::Unknown;
    };

}

#endif //BERSERK_IMAGE_HPP