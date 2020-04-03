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
#include <TArray.h>
#include <PixelFormat.h>
#include <Math/Color4f.h>

namespace Berserk {

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
        /** @return True if image width of height equals 0 */
        bool empty();
        /** @return True if pixel in image bounds */
        bool belongs(uint32 x, uint32 y) const;
        /** Set pixel color */
        void setPixel(uint32 x, uint32 y, const Color4f &color);
        /** @return Pixel color */
        Color4f getPixel(uint32 x, uint32 y) const;

        EPixelFormat getImageFormat() const { return mFormat; }
        uint32 getWidth() const { return mWidth; }
        uint32 getHeight() const { return mHeight; }
        uint32 getMipmapsCount() const { return mMipmapsCount; }
        uint32 getPixelSize() const { return mPixelSize; }
        bool isMipmapsGenerated() const { return mGenerateMipmaps; }
        const TArray<uint8> &getPixelData() const { return mPixelData; }

        static uint32 getPixelSizeFromFormat(EPixelFormat format);

    private:

        uint32 mWidth = 0;
        uint32 mHeight = 0;
        uint32 mMipmapsCount = 1;
        uint32 mPixelSize = 0;
        bool mGenerateMipmaps = false;
        EPixelFormat mFormat = EPixelFormat::Undefined;
        TArray<uint8> mPixelData;

    };

}

#endif //BERSERK_IMAGE_H