/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IMAGEIMPORTOPTIONS_H
#define BERSERK_IMAGEIMPORTOPTIONS_H

#include <IO/ResourceImportOptions.h>
#include <PixelFormat.h>

namespace Berserk {

    /** Options to import image with desired properties */
    class ImageImportOptions : public ResourceImportOptions {
    public:
        ~ImageImportOptions() override = default;

        /** Set pixel format of the imported image */
        void setPixelFormat(EPixelFormat format) { pixelFormat = format; }

        /** Set desired size of the imported image */
        void setSize(uint32 w, uint32 h) { width = w; height = h; }

        /** Set true if target image is in sRGB color space */
        void setFromSRGB(bool srgb) { fromSRGB = srgb; }

        /** Set true to keep imported image in sRGB space */
        void setKeepSRGB(bool srgb) { keepSRGB = srgb; }

        /** @return Pixel format of the imported image */
        EPixelFormat getFormat() const { return pixelFormat; }

        /** @return Desired width of the imported image */
        uint32 getWidth() const { return width; }

        /** @return Desired height of the imported image */
        uint32 getHeight() const { return height; }

        /** @return True if target image is in sRGB color space */
        bool getFromSRGB() const { return fromSRGB; }

        /** @return True to keep imported image in sRGB space */
        bool getKeepSRGB() const { return keepSRGB; }

    private:

        EPixelFormat pixelFormat = EPixelFormat::R8G8B8A8;
        uint32 width = 0;
        uint32 height = 0;
        bool fromSRGB = false;
        bool keepSRGB = false;
    };

}

#endif //BERSERK_IMAGEIMPORTOPTIONS_H
