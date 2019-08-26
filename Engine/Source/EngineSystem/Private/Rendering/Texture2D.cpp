//
// Created by Egor Orachyov on 2019-08-09.
//

#include "Rendering/Texture2D.h"

namespace Berserk
{

    Texture2D::Texture2D(RHIDriver &driver, const PixelDataRef &pixelData)
        : Texture2D(driver, pixelData, SF_RGBA8, true, false)
    {

    }

    Texture2D::Texture2D(RHIDriver &driver, const PixelDataRef &pixelData, EStorageFormat format, bool genMipMaps,
                         bool cacheData)
    {
        setWidth(pixelData->getWidth());
        setHeight(pixelData->getHeight());
        setDepth(1);
        setStorageFormat(format);
        setGenMipMaps(genMipMaps);
        setTextureType(TT_Texture2D);

        mResourceRHI = driver.createTexture(
                getWidth(),
                getHeight(),
                getStorageFormat(),
                pixelData->getPixelFormat(),
                pixelData->getDataType(),
                pixelData->getBuffer(),
                getGenMipMaps());

        if (cacheData) mPixelData = pixelData;
        _computeMemoryUsage(pixelData);
    }

    void Texture2D::_computeMemoryUsage(const PixelDataRef& data)
    {
        if (getCacheOnCPU())
        {
            setSizeCPU(sizeof(Texture2D) + data->getMemoryUsage());
            setSizeGPU(mResourceRHI->getMemoryUsage_GPU());
        }
        else
        {
            setSizeCPU(sizeof(Texture2D));
            setSizeGPU(mResourceRHI->getMemoryUsage_GPU());
        }
    }

} // namespace Berserk