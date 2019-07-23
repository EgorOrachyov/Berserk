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

namespace Berserk
{

    /** Stores loaded image common data to pass to RHI driver */
    class ENGINE_API ImageData
    {
    public:

        ImageData(String& name, uint32 width, uint32 height,
                EDataType type, EPixelFormat format, EStorageFormat storageFormat,
                const uint8* buffer,
                IAllocator& allocator = Allocator::get())
                : mWidth(width),
                  mHeight(height),
                  mDataType(type),
                  mPixelFormat(format),
                  mStorageFormat(storageFormat),
                  mName(name),
                  mBuffer(allocator)
        {
            const uint32 size = width * height;
            mBuffer.append(buffer, size);
        }

        uint32 getWidth() const { return mWidth; }

        uint32 getHeight() const { return mHeight; }

        const String& getName() const { return mName; }

        EDataType getDataType() const { return mDataType; }

        EPixelFormat getPixelFormat() const { return mPixelFormat; }

        EStorageFormat getStorageFormat() const { return mStorageFormat; }

        const TArray<uint8>& getBuffer() const { return mBuffer; }

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

        /** Image name/full path name */
        String mName;

        /** Data */
        TArray<uint8> mBuffer;

    };

} // namespace Berserk

#endif //BERSERK_IMAGEDATA_H