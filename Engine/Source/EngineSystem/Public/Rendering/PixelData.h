//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_PIXELDATA_H
#define BERSERK_PIXELDATA_H

#include <Rendering/Definitions.h>
#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Containers/TArray.h>
#include <Strings/String.h>
#include <Object/Allocatable.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /** Stores loaded image common data to pass to RHI driver */
    class ENGINE_API PixelData : public Allocatable
    {
    public:

        PixelData(uint32 width, uint32 height,
              EDataType type, EPixelFormat format, EStorageFormat storageFormat,
              const uint8* buffer, uint32 bufferSize,
              IAllocator& allocator = Allocator::get())

              : mWidth(width),
                mHeight(height),
                mDataType(type),
                mPixelFormat(format),
                mStorageFormat(storageFormat),
                mBuffer(allocator)
        {
            mBuffer.append(buffer, bufferSize);
        }

        PixelData(uint32 width, uint32 height,
              EDataType type, EPixelFormat format, EStorageFormat storageFormat,
              uint32 bufferSize,
              IAllocator& allocator = Allocator::get())

              : mWidth(width),
                mHeight(height),
                mDataType(type),
                mPixelFormat(format),
                mStorageFormat(storageFormat),
                mBuffer(bufferSize, allocator)
        {
            /** To provide proper copy of image data */
            mBuffer.setSizeExplicit(bufferSize);
        }

        /** @return Image width in pixels */
        uint32 getWidth() const { return mWidth; }

        /** @return Image height in pixels */
        uint32 getHeight() const { return mHeight; }

        /** @return Data type for single pixel color chanel value  */
        EDataType getDataType() const { return mDataType; }

        /** @return Pixel format of internal pixel buffer */
        EPixelFormat getPixelFormat() const { return mPixelFormat; }

        /** @return Storage format of internal pixel buffer */
        EStorageFormat getStorageFormat() const { return mStorageFormat; }

        /** @return Raw pixel data buffer pointer  */
        uint8* getBuffer() const { return mBuffer.getRawBuffer(); }

        /** @return Size of the internal pixel buffer [in bytes] */
        uint32 getBufferSize() const { return mBuffer.getSize(); }

        /** @return Memory usage by internal buffer */
        uint32 getMemoryUsage() const { return mBuffer.getMemoryUsage(); }

    protected:

        /** Sizes */
        uint32 mWidth;
        uint32 mHeight;

        /** Actual buffer data representation */
        EDataType mDataType;

        /** Format of the data */
        EPixelFormat mPixelFormat;

        /** Preferred storage format */
        EStorageFormat mStorageFormat;

        /** Data */
        TArray<uint8> mBuffer;

    };

    typedef TSharedPtr<PixelData> PixelDataRef;

} // namespace Berserk

#endif //BERSERK_PIXELDATA_H