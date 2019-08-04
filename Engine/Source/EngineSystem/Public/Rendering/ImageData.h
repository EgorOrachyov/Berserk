//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_IMAGEDATA_H
#define BERSERK_IMAGEDATA_H

#include <Rendering/Definitions.h>
#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Containers/TArray.h>
#include <Strings/String.h>
#include <Object/Allocatable.h>

namespace Berserk
{

    /** Stores loaded image common data to pass to RHI driver */
    class ENGINE_API ImageData : public Allocatable
    {
    public:

        ImageData(uint32 width, uint32 height,
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

        ImageData(uint32 width, uint32 height,
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

        }

        uint32 getWidth() const { return mWidth; }

        uint32 getHeight() const { return mHeight; }

        EDataType getDataType() const { return mDataType; }

        EPixelFormat getPixelFormat() const { return mPixelFormat; }

        EStorageFormat getStorageFormat() const { return mStorageFormat; }

        uint8* getBuffer() const { return mBuffer.getRawBuffer(); }

        uint32 getBufferSize() const { return mBuffer.getSize(); }

        uint32 getMemoryUsage() const { return sizeof(ImageData) + mBuffer.getMemoryUsage(); }

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

} // namespace Berserk

#endif //BERSERK_IMAGEDATA_H