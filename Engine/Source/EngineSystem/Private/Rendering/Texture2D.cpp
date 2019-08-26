//
// Created by Egor Orachyov on 2019-08-09.
//

#include "Rendering/Texture2D.h"

namespace Berserk
{

    Texture2D::Texture2D(RHIDriver &driver, Berserk::PixelDataRef pixelData,
                         Berserk::EStorageFormat format, bool genMipMaps)
                         : mImageData(std::move(pixelData))
    {
        setWidth(mImageData->getWidth());
        setHeight(mImageData->getHeight());
        setDepth(1);
        setStorageFormat(format);
        setGenMipMaps(genMipMaps);
        setTextureType(TT_Texture2D);
        setSizeCPU(sizeof(Texture2D));
        setSizeGPU((genMipMaps ? (uint32) (1.33f * mImageData->getBufferSize()) : mImageData->getBufferSize()));

        mRHITexture = driver.createTexture(
                getWidth(),
                getHeight(),
                getStorageFormat(),
                mImageData->getPixelFormat(),
                mImageData->getDataType(),
                mImageData->getBuffer(),
                getGenMipMaps());
    }

} // namespace Berserk