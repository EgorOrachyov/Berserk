//
// Created by Egor Orachyov on 2019-08-04.
//

#ifndef BERSERK_IMAGE_H
#define BERSERK_IMAGE_H

#include <Rendering/ImageData.h>
#include <Resource/IResource.h>
#include <Object/Allocatable.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /** Image resource, stores raw image data and provides additional image functions/access */
    class ENGINE_API Image final : public IResource, public Allocatable
    {
    public:

        /** Construct image from name, and raw image data */
        Image(const String& name, const TSharedPtr<ImageData> &data)
            : mImageName(name), mImageData(data)
        {

        }

        ~Image() override = default;

        /** @return Image data */
        const TSharedPtr<ImageData> &getImageData() const
        {
            return mImageData;
        }

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() const override
        {
            return sizeof(Image) + mImageData->getMemoryUsage();
        }

        /** @copydoc IResource::getName() */
        const String &getName() const override
        {
            return mImageName;
        }

    public:

        String mImageName;
        TSharedPtr<ImageData> mImageData;

    };

} // namespace Berserk

#endif //BERSERK_IMAGE_H