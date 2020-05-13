/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IMAGE_H
#define BERSERK_IMAGE_H

#include <Resource.h>
#include <Containers/TArray.h>
#include <PixelFormat.h>
#include <Math/Color4.h>
#include <IO/IResourceImporter.h>

namespace Berserk {

    /** Options to import image with desired properties */
    class ImageImportOptions : public IResourceImportOptions {
    public:
        ~ImageImportOptions() override = default;

        void setPixelFormat(EPixelFormat format) { pixelFormat = format; }
        void setSize(uint32 w, uint32 h) { width = w; height = h; }
        void setInSRGB(bool srgb) { inSRGB = srgb; }

        EPixelFormat getFormat() const { return pixelFormat; }
        uint32 getWidth() const { return width; }
        uint32 getHeight() const { return height; }
        bool getInSRGB() const { return inSRGB; }

    private:
        EPixelFormat pixelFormat = EPixelFormat::R8G8B8A8;
        uint32 width = 0;
        uint32 height = 0;
        bool inSRGB = false;
    };

    /**
     * @brief Image object
     *
     * Represent single 2-dimensional image object with possibly generated mip maps.
     * Data of the image stored in rows from the top left image corner.
     */
    class Image : public Resource {
    public:

        Image() = default;
        ~Image() override = default;
        Image(const Image& other) = default;

        /** Create image of size and format with default black color */
        void create(uint32 width, uint32 height, EPixelFormat format);

        /** Create image of size and format with specified color */
        void create(uint32 width, uint32 height, EPixelFormat format, const Color4f &color);

        /** Create image of size and format with specified image data */
        void create(uint32 width, uint32 height, EPixelFormat format, const uint8* data);

        /** Flip (i.e. mirror) image along the X axis (left side becomes right and vise versa) */
        void flipAlongX();

        /** Flip (i.e. mirror) image along the Y axis (top side becomes down and vise versa) */
        void flipAlongY();

        /** Power each image pixel component */
        void power(float factor = 2.2f);

        /** Resize an image to new size (supported for not all formats) */
        bool resize(uint32 newWidth, uint32 newHeight, bool sRGB = false);

        /** @return True if image width of height equals 0 */
        bool empty();

        /** @return True if pixel in image bounds */
        bool belongs(uint32 x, uint32 y) const;

        /** Set pixel color */
        void setPixel(uint32 x, uint32 y, const Color4f &color);

        /** @return Pixel color */
        Color4f getPixel(uint32 x, uint32 y) const;

        EPixelFormat getPixelFormat() const { return mFormat; }
        uint32 getWidth() const { return mWidth; }
        uint32 getHeight() const { return mHeight; }
        uint32 getMipmapsCount() const { return mMipmapsCount; }
        uint32 getPixelSize() const { return mPixelSize; }
        bool isMipmapsGenerated() const { return mGenerateMipmaps; }
        const TArray<uint8> &getPixelData() const { return mPixelData; }

        /** @return Size in bytes on single pixel component */
        static uint32 getPixelSizeFromFormat(EPixelFormat format);

        /** Flips pixel data along the X axis without extra allocation (useful for RHI driver to convert images to proper XY layout) */
        static void pixelDataFlipAlongX(uint32 width, uint32 height, uint32 pixelSize, TArray<uint8> &pixelData);

        /** Flips pixel data along the Y axis without extra allocation (useful for RHI driver to convert images to proper XY layout) */
        static void pixelDataFlipAlongY(uint32 width, uint32 height, uint32 pixelSize, TArray<uint8> &pixelData);

        /** Power each pixel color component  */
        static void pixelDataPow(uint32 width, uint32 height, EPixelFormat format, float factor, TArray<uint8> &pixelData);

        /** @return True, if successfully resize data. Resize pixel data */
        static bool pixelDataResize(uint32 oldWidth, uint32 oldHeight, const TArray<uint8> &oldPixelData, uint32 width, uint32 height, const TArray<uint8> &pixelData, EPixelFormat format, bool sRGB = false);

        /** @return Default import options */
        static const TPtrShared<ImageImportOptions> &getDefaultImportOptions();

    private:
        uint32 mWidth = 0;
        uint32 mHeight = 0;
        uint32 mMipmapsCount = 1;
        uint32 mPixelSize = 0;
        bool mGenerateMipmaps = false;
        EPixelFormat mFormat = EPixelFormat::Unknown;
        TArray<uint8> mPixelData;
    };

}

#endif //BERSERK_IMAGE_H