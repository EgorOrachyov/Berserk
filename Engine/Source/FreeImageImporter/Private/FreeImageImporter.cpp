//
// Created by Egor Orachyov on 2019-07-23.
//

#include "FreeImageImporter.h"
#include <Logging/DebugLogMacros.h>
#include <FreeImage/FreeImage.h>

namespace Berserk
{

    void fillBuffer32(const char* filename, EPixelFormat format, uint32 width, uint32 height, uint32* buffer, FIBITMAP* fibitmap)
    {
        RGBQUAD color = {0,0,0,0};
        uint8* bytes = (uint8*) buffer;

        switch (format)
        {
            // a r g b
            case PF_BGRA:
                for(uint32 i = 0; i < width; i++)
                {
                    for(uint32 j = 0; j < height; j++)
                    {
                        auto e = buffer[j * width + i];

                        color.rgbRed   = (uint8) ( ( e >> 16 ) & 0x000000FF );
                        color.rgbGreen = (uint8) ( ( e >> 8  ) & 0x000000FF );
                        color.rgbBlue  = (uint8) ( ( e       ) & 0x000000FF );

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);
                    }
                }
                break;

            // a b g r
            case PF_RGBA:
                for(uint32 i = 0; i < width; i++)
                {
                    for(uint32 j = 0; j < height; j++)
                    {
                        auto e = buffer[j * width + i];

                        color.rgbBlue  = (uint8) ( ( e >> 16 ) & 0x000000FF );
                        color.rgbGreen = (uint8) ( ( e >> 8  ) & 0x000000FF );
                        color.rgbRed   = (uint8) ( ( e       ) & 0x000000FF );

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);
                    }
                }
                break;

            // r g b a
            case PF_ABGR:
                for(uint32 i = 0; i < width; i++)
                {
                    for(uint32 j = 0; j < height; j++)
                    {
                        auto e = buffer[j * width + i];

                        color.rgbRed   = (char) ( ( e >> 16 ) & 0x000000FF );
                        color.rgbGreen = (char) ( ( e >>  8 ) & 0x000000FF );
                        color.rgbBlue  = (char) ( ( e       ) & 0x000000FF );

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);
                    }
                }
                break;

            // r g b
            case PF_RGB:
                for(uint32 j = 0; j < height; j++)
                {
                    for(uint32 i = 0; i < width; i++)
                    {
                        color.rgbRed   = (char) bytes[0];
                        color.rgbGreen = (char) bytes[1];
                        color.rgbBlue  = (char) bytes[2];

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);

                        bytes += 3;
                    }
                }
                break;

            // b g r
            case PF_BGR:
                for(uint32 j = 0; j < height; j++)
                {
                    for(uint32 i = 0; i < width; i++)
                    {
                        color.rgbRed   = (char) bytes[2];
                        color.rgbGreen = (char) bytes[1];
                        color.rgbBlue  = (char) bytes[0];

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);

                        bytes += 3;
                    }
                }
                break;

            // r
            case PF_R:
                for(uint32 j = 0; j < height; j++)
                {
                    for(uint32 i = 0; i < width; i++)
                    {
                        color.rgbRed   = (char) bytes[0];
                        color.rgbGreen = (char)        0;
                        color.rgbBlue  = (char)        0;

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);

                        bytes += 1;
                    }
                }
                break;

            // r g
            case PF_RG:
                for(uint32 j = 0; j < height; j++)
                {
                    for(uint32 i = 0; i < width; i++)
                    {
                        color.rgbRed   = (char) bytes[0];
                        color.rgbGreen = (char) bytes[1];
                        color.rgbBlue  = (char)        0;

                        FreeImage_SetPixelColor(fibitmap, i, j, &color);

                        bytes += 2;
                    }
                }
                break;

            default:
                DEBUG_LOG_ERROR("Unsupported pixel type [filename: %s]", filename)
        }
    }

    FreeImageImporter::FreeImageImporter(Berserk::IAllocator &allocator)
        : mAllocator(allocator),
          mExtensions(allocator)
    {
        FreeImage_Initialise(false);

        mExtensions.ensureCapacity(FreeImage_GetFIFCount());
        for (uint32 i = 0; i < FreeImage_GetFIFCount(); i++)
        {
            if (FreeImage_FIFSupportsReading((FREE_IMAGE_FORMAT)i))
            {
                Wrapper extension = FreeImage_GetFormatFromFIF((FREE_IMAGE_FORMAT)i);
                mExtensions.add(extension.toLowerCase());
            }
        }
    }

    FreeImageImporter::~FreeImageImporter()
    {
        FreeImage_DeInitialise();
    }

    TSharedPtr<PixelData> FreeImageImporter::importRaw(const char *filename)
    {
        uint8* buffer;
        uint32 bufferSize;
        uint32 width, height;
        EDataType dataType;
        EPixelFormat pixelFormat;
        EStorageFormat storageFormat;

        FIBITMAP* fibitmap;
        FIBITMAP* converted;
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
                    return TSharedPtr<PixelData>();
                }
            }

            if (!FreeImage_FIFSupportsReading(format))
            {
                DEBUG_LOG_WARNING("FreeImageImporter: cannot import image [filename: %s]", filename);
                return TSharedPtr<PixelData>();
            }

            fibitmap = FreeImage_Load(format, filename);

            if (!fibitmap)
            {
                DEBUG_LOG_WARNING("FreeImageImporter: fail load bitmap for image [filename: %s]", filename);
                return TSharedPtr<PixelData>();
            }

            converted = FreeImage_ConvertTo32Bits(fibitmap);
            buffer = FreeImage_GetBits(converted);

            if (!buffer)
            {
                DEBUG_LOG_WARNING("FreeImageImporter: empty image buffer [filename: %s]", filename);
                return TSharedPtr<PixelData>();
            }
        }

        {
            // Figure out the info about loaded image to successfully
            // pass it to the platform gpu device driver

            width = FreeImage_GetWidth(converted);
            height = FreeImage_GetHeight(converted);
            type = FreeImage_GetImageType(converted);
            bufferSize = 4 * width * height;

            switch (type)
            {
                case FIT_BITMAP:
                    dataType = DT_UnsignedByte;
                    pixelFormat = PF_BGRA;
                    storageFormat = SF_RGBA8;
                    break;

                default:
                    DEBUG_LOG_ERROR("Unsupported image format and pixel type [filename: %s]", filename)
                    FreeImage_Unload(fibitmap);
                    FreeImage_Unload(converted);
                    return TSharedPtr<PixelData>();
            }
        }

        auto data = mAllocator.engnie_new<PixelData>(
                width,
                height,
                dataType,
                pixelFormat,
                storageFormat,
                buffer,
                bufferSize,
                mAllocator);

        FreeImage_Unload(fibitmap);
        FreeImage_Unload(converted);

        return TSharedPtr<PixelData>(data, &mAllocator);
    }

    bool FreeImageImporter::exportRaw(const char *filename, const PixelData &image)
    {
        EPixelFormat pixelFormat = image.getPixelFormat();
        EDataType dataType = image.getDataType();

        uint32 width = image.getWidth();
        uint32 height = image.getHeight();
        auto buffer = (uint32*) image.getBuffer();

        if (dataType == DT_UnsignedByte)
        {
            uint32 bitsPerPixel = 32;

            FIBITMAP* fibitmap = FreeImage_Allocate(width, height, bitsPerPixel);
            fillBuffer32(filename, pixelFormat, width, height, buffer, fibitmap);

            FreeImage_Save(FIF_BMP, fibitmap, filename);
            FreeImage_Unload(fibitmap);

            return true;
        }

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

    const TArray<String> &FreeImageImporter::getSupportedReadingExtensions() const
    {

    }


} // namespace Berserk