//
// Created by Egor Orachyov on 2019-08-09.
//

#include "Rendering/Texture2D.h"

namespace Berserk
{

    Texture2D::Texture2D(Berserk::RHIDriverRef &driver, Berserk::ImageImportDataRef imageData,
                         Berserk::EStorageFormat format, bool genMipMaps)
                         : mImageData(std::move(imageData))
    {
        setWidth(mImageData->getWidth());
        setHeight(mImageData->getHeight());
        setDepth(1);
        setStorageFormat(format);
        setGenMipMaps(genMipMaps);
        setTextureType(TT_Texture2D);
        setSizeCPU(sizeof(Texture2D));
        setSizeGPU((genMipMaps ? (uint32) (1.33f * imageData->getBufferSize()) : imageData->getBufferSize()));

        mRHITexture = driver->createTexture(
                getWidth(),
                getHeight(),
                getStorageFormat(),
                mImageData->getPixelFormat(),
                mImageData->getDataType(),
                mImageData->getBuffer(),
                getGenMipMaps());
    }

} // namespace Berserk