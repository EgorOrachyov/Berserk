//
// Created by Egor Orachyov on 2019-08-04.
//

#ifndef BERSERK_IMAGE_H
#define BERSERK_IMAGE_H

#include <Rendering/PixelData.h>
#include <Resource/Resource.h>
#include <Object/Allocatable.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{


    /**
     * Image stores raw image data loaded from disc.
     *
     * Base class provider of image data for render textures.
     * Supported and common image formats: png/bmp/jpg
     *
     * @note In current implementation appears as simple wrapper
     *       for loaded image import data.
     *
     * @note Images possibly stays duplicated on RAM and VRAM side
     *       for render system texture resources.
     */
    class ENGINE_API Image final : public Resource
    {
    public:

        /** Construct image from name, and raw image data */
        Image(const String &name, PixelData &data)
            : mImageData(std::move(data))
        {
            mResourceName = name;
            mSizeCPU = sizeof(Image) + mImageData.getMemoryUsage();
        }

        ~Image() override = default;

        /** @return Image width in pixels */
        uint32 getWidth() const { return mImageData.getWidth(); }

        /** @return Image height in pixels */
        uint32 getHeight() const { return mImageData.getHeight(); }

        /** @return Data type for single pixel color chanel value  */
        EDataType getDataType() const { return mImageData.getDataType(); }

        /** @return Pixel format of internal pixel buffer */
        EPixelFormat getPixelFormat() const { return mImageData.getPixelFormat(); }

        /** @return Storage format of internal pixel buffer */
        EStorageFormat getStorageFormat() const { return mImageData.getStorageFormat(); }

        /** @return Raw pixel data buffer pointer  */
        uint8* getBuffer() const { return mImageData.getBuffer(); }

        /** @return Size of the internal pixel buffer [in bytes] */
        uint32 getBufferSize() const { return mImageData.getBufferSize(); }

    public:

        /** Stores raw data inside */
        PixelData mImageData;

    };

} // namespace Berserk

#endif //BERSERK_IMAGE_H