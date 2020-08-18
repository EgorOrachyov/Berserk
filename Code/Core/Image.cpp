/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ErrorMacro.h>
#include <Containers/TAlgo.h>
#include <Image.h>
#include <ResourceImporters.h>

// Currently use STB as resize utility
#include <stb_image_resize.h>

namespace Berserk {

    void Image::create(uint32 width, uint32 height, EPixelFormat format, bool inSRGB) {
        Image::create(width, height, format, Color4f(0.0f), inSRGB);
    }

    void Image::create(uint32 width, uint32 height, EPixelFormat format, const Color4f &color, bool inSRGB) {
        mInSRGB = inSRGB;
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

    void Image::create(uint32 width, uint32 height, EPixelFormat format, const uint8 *data, bool inSRGB) {
        mInSRGB = inSRGB;
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

    void Image::power(float factor) {
        if (empty()) return;
        Image::pixelDataPow(mWidth, mHeight, mFormat, factor, mPixelData);
    }

    bool Image::resize(uint32 newWidth, uint32 newHeight) {
        if (empty()) return false;
        if (newWidth == 0 || newHeight == 0) return false;

        TArray<uint8> newPixelData;
        newPixelData.resize(newWidth * newHeight * mPixelSize);

        auto result =Image::pixelDataResize(mWidth, mHeight, mPixelData, newWidth, newHeight, newPixelData, mFormat, mInSRGB);

        if (result) {
            mWidth = newWidth;
            mHeight = newHeight;
            mPixelData = std::move(newPixelData);
        }

        return result;
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

    void Image::convertToSRGB() {
        if (!isInSRGB()) {
            mInSRGB = true;
            power(1.0f / 2.2f);
        }
    }

    void Image::convertToLinearSpace() {
        if (isInSRGB()) {
            mInSRGB = false;
            power(2.2f);
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

    void Image::pixelDataPow(uint32 width, uint32 height, EPixelFormat format, float factor, TArray<uint8> &pixelData) {
        auto data = (uint8*) pixelData.data();

        switch (format) {
            case EPixelFormat::R8: {
                auto pixel = data;
                for (uint32 i = 0; i < height; i++) {
                    for (uint32 j = 0; j < width; j++) {
                        auto value = (float) pixel[i * width + j];
                        value = Math::pow(value, factor);
                        value = Math::clamp(value, 0.0f, 255.0f);
                        pixel[i * width + j] = (uint8) value;
                    }
                }
            }
            case EPixelFormat::R8G8B8A8: {
                auto pixel = (uint32*) data;
                for (uint32 i = 0; i < height; i++) {
                    for (uint32 j = 0; j < width; j++) {
                        Color4f color4F = Color4f::fromR8G8B8A8(pixel[i * width + j]);
                        color4F = color4F.toLinear(factor);
                        pixel[i * width + j] = color4F.toR8G8B8A8();
                    }
                }
            }
            default:
                return;
        }
    }

    bool Image::pixelDataResize(uint32 oldWidth, uint32 oldHeight,
                                const TArray<uint8> &oldPixelData, uint32 width,
                                uint32 height, const TArray<uint8> &pixelData,
                                EPixelFormat format, bool sRGB) {

        auto result = 0;
        auto oldData = (uint8*) oldPixelData.data();
        auto data = (uint8*) pixelData.data();

        switch (format) {
            case EPixelFormat::R8: {
                if (sRGB)
                    result = stbir_resize_uint8(
                            oldData, oldWidth, oldHeight, 0,
                            data, width, height, 0, 1
                            );
                else
                    result = stbir_resize_uint8_srgb(
                            oldData, oldWidth, oldHeight, 0,
                            data, width, height, 0,
                            1,
                            STBIR_ALPHA_CHANNEL_NONE, 0);

                return result != 0;
            }
            case EPixelFormat::R8G8B8A8: {
                if (sRGB)
                    result = stbir_resize_uint8(
                            oldData, oldWidth, oldHeight, 0,
                            data, width, height, 0,
                            4);
                else
                    result = stbir_resize_uint8_srgb(
                            oldData, oldWidth, oldHeight, 0,
                            data, width, height, 0,
                            4, 4, STBIR_FLAG_ALPHA_USES_COLORSPACE);

                return result != 0;
            }
            default:
                return result != 0;
        }
    }

    const TPtrShared<ImageImportOptions>& Image::getDefaultImportOptions() {
        static TPtrShared<ImageImportOptions> importOptions = TPtrShared<ImageImportOptions>::make();
        return importOptions;
    }

    TPtrShared<Image> Image::loadImage(Berserk::ImageImportOptions &options, const Berserk::CString &filePath) {
        auto& importers = ResourceImporters::getSingleton();
        auto importer = importers.findImporterFromPath(filePath);

        if (importer.isNotNull()) {
            TPtrShared<Resource> resource;
            auto status = importer->import(resource, filePath, options);

            BERSERK_COND_ERROR_RET_VALUE(nullptr, status == EError::OK, "Failed to import image: %s", filePath.data());

            return resource.castTo<Image>();
        }

        return nullptr;
    }

}