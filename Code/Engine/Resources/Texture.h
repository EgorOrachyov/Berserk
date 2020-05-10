/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TEXTURE_H
#define BERSERK_TEXTURE_H

#include <Resource.h>
#include <Threading/Async.h>
#include <RHI/RHIResources.h>

namespace Berserk {

    /** Texture filtering quality for materials */
    enum class ETextureFiltering : uint32 {
        LinearWithLOD,
        LinearWithoutLOD,
    };

    /** Extract sampler description from filtering enum */
    class ETextureFilteringUtil {
    public:
        static void getSamplerDescFromFiltering(ETextureFiltering filtering, RHISamplerDesc &samplerDesc);
    };

    class Texture : public Resource {
    public:
        bool isLoaded() const override { return (mIsLoaded.isPresent() ? mIsLoaded.isLoaded() : true); }

        EPixelFormat getPixelFormat() const { return mFormat; }
        ETextureFiltering getFiltering() const { return mFiltering; }

        uint32 getWidth() const { return mWidth; }
        uint32 getHeight() const { return mHeight; }
        uint32 getDepth() const { return mDepth; }

        bool getGenerateMipMaps() const { return mGenerateMipMaps; }
        bool getCacheDataCPU() const { return mCacheDataCPU; }
        bool getsRGB() const { return msRGB; }

    protected:
        EPixelFormat mFormat;
        ETextureFiltering mFiltering;
        uint32 mWidth;
        uint32 mHeight;
        uint32 mDepth;
        bool mGenerateMipMaps;
        bool mCacheDataCPU;
        bool msRGB = false;

        Async mIsLoaded;

        // Render thread resources
        TPtrShared<RHISampler> mSampler;
        TPtrShared<RHITexture> mTextureResource;
    };

    class Texture2D : public Texture {
    public:
        Texture2D(const TPtrShared<Image> &image, ETextureFiltering filtering, bool generateMipMaps, bool cacheDataCPU, bool sRGB);
        ~Texture2D() override;

        const TPtrShared<Image> &getCachedImage() const { return mCachedImage; }

    private:
        TPtrShared<Image> mCachedImage;
    };
}

#endif //BERSERK_TEXTURE_H