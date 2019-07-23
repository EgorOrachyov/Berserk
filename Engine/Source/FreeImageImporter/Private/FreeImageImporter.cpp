//
// Created by Egor Orachyov on 2019-07-23.
//

#include "FreeImageImporter.h"
#include <Logging/DebugLogMacros.h>
#include <FreeImage/FreeImage.h>

namespace Berserk
{

    FreeImageImporter::FreeImageImporter(Berserk::IAllocator &allocator)
        : mAllocator(allocator)
    {
        FreeImage_Initialise(false);
    }

    FreeImageImporter::~FreeImageImporter()
    {
        FreeImage_DeInitialise();
    }

    TSharedPtr<ImageData> FreeImageImporter::load(const char *filename)
    {
        uint8* buffer;
        uint32 width, height;
        EDataType dataType;
        EPixelFormat pixelFormat;
        EStorageFormat storageFormat;

        FIBITMAP* fibitmap;
        FREE_IMAGE_TYPE type;
        FREE_IMAGE_FORMAT format;

        {
            // Get image format, load image and get image
            // raw data (pinter to)

            format = FreeImage_GetFileType(filename, 0);

            if (format == FIF_UNKNOWN)
            {
                format = FreeImage_GetFIFFromFilename(filename);

                if (format == FIF_UNKNOWN)
                {
                    DEBUG_LOG_WARNING("FreeImageImporter: unknown format for image [filename: %s]", filename);
                    return TSharedPtr<ImageData>();
                }
            }

            if (!FreeImage_FIFSupportsReading(format))
            {
                DEBUG_LOG_WARNING("FreeImageImporter: cannot import image [filename: %s]", filename);
                return TSharedPtr<ImageData>();
            }

            fibitmap = FreeImage_Load(format, filename);

            if (!fibitmap)
            {
                DEBUG_LOG_WARNING("FreeImageImporter: fail load bitmap for image [filename: %s]", filename);
                return TSharedPtr<ImageData>();
            }

            buffer = FreeImage_GetBits(fibitmap);

            if (!buffer)
            {
                DEBUG_LOG_WARNING("FreeImageImporter: empty image buffer [filename: %s]", filename);
                return TSharedPtr<ImageData>();
            }
        }

        {
            // Figure out the info about loaded image to successfully
            // pass it to the platform gpu device driver

            width = FreeImage_GetWidth(fibitmap);
            height = FreeImage_GetHeight(fibitmap);
            type = FreeImage_GetImageType(fibitmap);

            switch (type)
            {
                case FIT_BITMAP:
                    dataType = DT_UnsignedByte;
                    pixelFormat = PF_BGRA;
                    storageFormat = SF_RGBA8;
                    break;

                case FIT_UINT32:
                    dataType = DT_UnsignedInt;
                    pixelFormat = PF_BGRA;
                    storageFormat = SF_RGBA8;
                    break;

                default:
                    DEBUG_LOG_ERROR("Unsupported image format and pixel type [filename: %s]", filename)
                    FreeImage_Unload(fibitmap);
                    return TSharedPtr<ImageData>();
            }
        }

        String name(filename);
        auto data = mAllocator.engnie_new<ImageData>(
                name,
                width,
                height,
                dataType,
                pixelFormat,
                storageFormat,
                buffer,
                mAllocator);

        return TSharedPtr<ImageData>(data, &mAllocator);
    }

    bool FreeImageImporter::save(const char *filename, const ImageData &image)
    {
        return false;
    }

    bool FreeImageImporter::isReadingSupported(const char *filename)
    {
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

        if (format == FIF_UNKNOWN)
        {
            format = FreeImage_GetFIFFromFilename(filename);
            return (FreeImage_FIFSupportsReading(format) != 0);
        }

        return (FreeImage_FIFSupportsReading(format) != 0);
    }

    bool FreeImageImporter::isWritingSupported(const char *filename)
    {
        FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(filename);
        return (FreeImage_FIFSupportsWriting(format) != 0);
    }


} // namespace Berserk