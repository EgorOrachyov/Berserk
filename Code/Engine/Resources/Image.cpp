/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resources/Image.h>
#include <ErrorMacro.h>
#include <TAlgo.h>

namespace Berserk {

    void Image::create(uint32 width, uint32 height, EPixelFormat format) {
        Image::create(width, height, format, Color4f(0.0f));
    }

    void Image::create(uint32 width, uint32 height, EPixelFormat format, const Color4f &color) {
        mWidth = width;
        mHeight = height;
        mFormat = format;
        mPixelSize = getPixelSizeFromFormat(format);

        uint32 dataSize = mWidth * mHeight * mPixelSize;
        mPixelData.resize(dataSize);

        switch (format) {
            case EPixelFormat::R8G8B8A8: {
                auto rawColor = color.toR8G8B8A8();
                auto data = (uint32*) getPixelData().data();
                for (int i = 0; i < mWidth * mHeight; i++) {
                    data[i] = rawColor;
                }
            } break;
            case EPixelFormat::R8: {
                auto rawColor = color.toR8();
                auto data = (uint8*) getPixelData().data();
                for (int i = 0; i < mWidth * mHeight; i++) {
                    data[i] = rawColor;
                }
            } break;
            default:
                BERSERK_ERROR("Attempt to create image in undefined layout");
        }
    }

    void Image::create(uint32 width, uint32 height, EPixelFormat format, const uint8 *data) {
        mWidth = width;
        mHeight = height;
        mFormat = format;
        mPixelSize = getPixelSizeFromFormat(format);

        uint32 dataSize = mWidth * mHeight * mPixelSize;
        mPixelData.clear();
        mPixelData.resize(dataSize);

        Memory::copy(mPixelData.data(), data, dataSize);
    }

    void Image::flipAlongX() {
        if (empty()) return;
        Image::pixelDataFlipAlongX(mWidth, mHeight, mPixelSize, mPixelData);
    }

    void Image::flipAlongY() {
        if (empty()) return;
        Image::pixelDataFlipAlongY(mWidth, mHeight, mPixelSize, mPixelData);
    }

    bool Image::empty() {
        return mWidth == 0 || mHeight == 0;
    }

    bool Image::belongs(uint32 x, uint32 y) const {
        return 0 <= x && x <= mWidth && 0 <= y && y <= mHeight;
    }

    void Image::setPixel(uint32 x, uint32 y, const Color4f &color) {
        BERSERK_COND_ERROR_RET(belongs(x,y), "Pixel is out of the image bounds");

        uint32 index = y * mWidth + x;

        switch (mFormat) {
            case EPixelFormat::R8G8B8A8: {
                auto rawColor = color.toR8G8B8A8();
                auto data = (uint32*) getPixelData().data();
                data[index] = rawColor;
            } break;
            case EPixelFormat::R8: {
                auto rawColor = color.toR8();
                auto data = (uint8*) getPixelData().data();
                data[index] = rawColor;
            } break;
            default:
                BERSERK_ERROR_RET("Image format is undefined");
        }
    }

    Color4f Image::getPixel(uint32 x, uint32 y) const {
        BERSERK_COND_ERROR_RET_VALUE(Color4f(), belongs(x,y), "Pixel is out of the image bounds");

        uint32 index = y * mWidth + x;

        switch (mFormat) {
            case EPixelFormat::R8G8B8A8: {
                auto data = (uint32*) getPixelData().data();
                auto rawColor = data[index];
                return Color4f::fromR8G8B8A8(rawColor);
            }
            case EPixelFormat::R8: {
                auto data = getPixelData().data();
                auto rawColor = data[index];
                return Color4f::fromR8(rawColor);
            }
            default:
                BERSERK_ERROR_RET_VALUE(Color4f(), "Image format is undefined");
        }
    }

    uint32 Image::getPixelSizeFromFormat(EPixelFormat format) {
        switch (format) {
            case EPixelFormat::R8:
                return 1;
            case EPixelFormat::R8G8B8A8:
                return 4;
            default:
                return 0;
        }
    }

    void Image::pixelDataFlipAlongX(uint32 width, uint32 height, uint32 pixelSize, TArray<uint8> &pixelData) {
        auto data = (uint8*) pixelData.data();
        auto stride = width * pixelSize;
        auto step = pixelSize;

        for (uint32 i = 0; i < height; i++) {
            auto line = data + i * stride;

            for (uint32 j = 0; j < width / 2; j++) {
                auto j1 = j;
                auto j2 = width - j - 1;

                auto pixel1 = line + j1 * step;
                auto pixel2 = line + j2 * step;

                for (uint32 k = 0; k < step; k++) {
                    TAlgo::swap(pixel1[k], pixel2[k]);
                }
            }
        }
    }

    void Image::pixelDataFlipAlongY(uint32 width, uint32 height, uint32 pixelSize, TArray<uint8> &pixelData) {
        auto data = (uint8*) pixelData.data();
        auto stride = width * pixelSize;

        for (uint32 y = 0; y < height / 2; y++) {
            auto line1 = data + y * stride;
            auto line2 = data + (height - y - 1) * stride;

            for (uint32 x = 0; x < stride; x++) {
                TAlgo::swap(line1[x], line2[x]);
            }
        }
    }

}