/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resources/Texture.h>
#include <RHI/RHIDevice.h>

namespace Berserk {

    void ETextureFilteringUtil::getSamplerDescFromFiltering(ETextureFiltering filtering, RHISamplerDesc &samplerDesc) {

        switch (filtering) {
            case ETextureFiltering::LinearWithLOD: {
                samplerDesc.min = ESamplerFilter::Linear;
                samplerDesc.mag = ESamplerFilter::Linear;
                samplerDesc.mipmapMode = ESamplerFilter::Linear;
                samplerDesc.minLod = RHISamplerDesc::MIN_LOD;
                samplerDesc.maxLod = RHISamplerDesc::MAX_LOD;
                samplerDesc.u = ESamplerRepeatMode::Repeat;
                samplerDesc.v = ESamplerRepeatMode::Repeat;
                samplerDesc.w = ESamplerRepeatMode::Repeat;
                samplerDesc.useMips = true;
                samplerDesc.color = ESamplerBorderColor::White;
                return;
            }
            case ETextureFiltering::LinearWithoutLOD: {
                samplerDesc.min = ESamplerFilter::Linear;
                samplerDesc.mag = ESamplerFilter::Linear;
                samplerDesc.mipmapMode = ESamplerFilter::Linear;
                samplerDesc.minLod = RHISamplerDesc::MIN_LOD;
                samplerDesc.maxLod = RHISamplerDesc::MAX_LOD;
                samplerDesc.u = ESamplerRepeatMode::Repeat;
                samplerDesc.v = ESamplerRepeatMode::Repeat;
                samplerDesc.w = ESamplerRepeatMode::Repeat;
                samplerDesc.useMips = false;
                samplerDesc.color = ESamplerBorderColor::White;
                return;
            }
            default:
                samplerDesc = RHISamplerDesc();
                return;
        }

    }

    Texture2D::Texture2D(const TPtrShared<Image> &image, ETextureFiltering filtering, bool generateMipMaps, bool cacheDataCPU, bool sRGB) {
        BERSERK_COND_ERROR(image.isNotNull(), "An attempt to create texture from null image");

        if (cacheDataCPU) mCachedImage = image;

        mFormat = image->getPixelFormat();
        mWidth = image->getWidth();
        mHeight = image->getHeight();
        mDepth = 1;
        mFiltering = filtering;
        mGenerateMipMaps = generateMipMaps;
        mCacheDataCPU = cacheDataCPU && image.isNotNull();
        msRGB = sRGB;

        // todo: command queue creation process

        auto& device = RHIDevice::getSingleton();

        RHISamplerDesc samplerDesc;
        ETextureFilteringUtil::getSamplerDescFromFiltering(mFiltering, samplerDesc);
        mTextureResource = device.createTexture2D(EMemoryType::Static, generateMipMaps, *image);
        mSampler = device.createSampler(samplerDesc);
    }

    Texture2D::~Texture2D() {
        // todo: command queue destruction process
    }

}