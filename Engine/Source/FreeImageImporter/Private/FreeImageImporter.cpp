//
// Created by Egor Orachyov on 22.02.2019.
//

#include "Misc/Assert.h"
#include "FreeImageImporter.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void FreeImageImporter::initialize()
    {
        FreeImage_Initialise();
        mBitmap = nullptr;
        PUSH("FreeImageImporter: initialize");
    }

    void FreeImageImporter::release()
    {
        unload();
        FreeImage_DeInitialise();
        PUSH("FreeImageImporter: de-initialize");
    }

    bool FreeImageImporter::isReadingSupported(const char *name)
    {
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(name, 0);

        if (format == FIF_UNKNOWN)
        {
            format = FreeImage_GetFIFFromFilename(name);
            return (FreeImage_FIFSupportsReading(format) != 0);
        }

        return (FreeImage_FIFSupportsReading(format) != 0);
    }

    bool FreeImageImporter::isWritingSupported(const char *name)
    {
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(name, 0);

        if (format == FIF_UNKNOWN)
        {
            format = FreeImage_GetFIFFromFilename(name);
            return (FreeImage_FIFSupportsWriting(format) != 0);
        }

        return (FreeImage_FIFSupportsWriting(format) != 0);
    }

    bool FreeImageImporter::import(const char *name, ImageData &data)
    {
        unload();

        void* buffer;
        uint32 width, height;
        PixelType pixelType;
        ImageFormat imageFormat;
        FREE_IMAGE_TYPE type;
        FREE_IMAGE_FORMAT format;

        {
            // Get image format, load image and get image
            // raw data (pinter to). Notice: that you cannot unload
            // data before you pass it to the gpu VRAM.

            format = FreeImage_GetFileType(name, 0);

            if (format == FIF_UNKNOWN)
            {
                format = FreeImage_GetFIFFromFilename(name);

                if (format == FIF_UNKNOWN)
                {
                    WARNING("Unknown format for image [name: %s]", name);
                    return false;
                }
            }

            if (!FreeImage_FIFSupportsReading(format))
            {
                WARNING("Cannot import image [name: %s]", name);
                return false;
            }

            mBitmap = FreeImage_Load(format, name);

            if (!mBitmap)
            {
                WARNING("Fail load bitmap for image [name: %s]", name);
                return false;
            }

            buffer = FreeImage_GetBits(mBitmap);

            if (!buffer)
            {
                WARNING("Empty image buffer [name: %s]", name);
                return false;
            }
        }

        {
            // Figure out the info about loaded image to successfully
            // pass it to the platform gpu device driver

            width = FreeImage_GetWidth(mBitmap);
            height = FreeImage_GetHeight(mBitmap);
            type = FreeImage_GetImageType(mBitmap);

            PUSH("FreeImageImporter: type: %u [name: %s]", type, name);

            switch (type)
            {
                case FIT_BITMAP:
                    pixelType = UNSIGNED_BYTE;
                    imageFormat = BGRA;
                    break;

                case FIT_UINT32:
                    pixelType = UNSIGNED_INT;
                    imageFormat = BGRA;
                    break;

                case FIT_RGBF:
                    pixelType = RGBF;
                    imageFormat = BGR;
                    break;

                case FIT_RGBAF:
                    pixelType = RGBAF;
                    imageFormat = BGRA;
                    break;

                default:
                    FAIL(false, "Unsupported image format and pixel type [name: %s]", name)
            }
        }

        {
            // Save loaded data in the output structure

            data.width = width;
            data.height = height;
            data.buffer = buffer;
            data.pixelType = pixelType;
            data.format = imageFormat;
        }

        return true;
    }

    bool FreeImageImporter::save(const char *name, const ImageData &data)
    {

    }

    void FreeImageImporter::unload()
    {
        if (mBitmap)
        {
            FreeImage_Unload(mBitmap);
            mBitmap = nullptr;
        }
    }

} // namespace Berserk