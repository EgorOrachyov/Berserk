/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISAMPLER_H
#define BERSERK_RHISAMPLER_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHISampler : public RHIResource {
    public:
        ~RHISampler() override = default;

        /** @return Sampler minification filter */
        ESamplerFilter getMinFilter() const { return mMin; }

        /** @return Sampler magnification filter */
        ESamplerFilter getMagFilter() const { return mMag; }

        /** @return Sampler filter along mip levels */
        ESamplerFilter getMipmapMode() const { return mMipmapMode; }

        /** @return Repeat mode along u axis (coordinate) */
        ESamplerRepeatMode getRepeatU() const { return mRepeatU; }

        /** @return Repeat mode along v axis (coordinate) */
        ESamplerRepeatMode getRepeatV() const { return mRepeatV; }

        /** @return Repeat mode along w axis (coordinate) */
        ESamplerRepeatMode getRepeatW() const { return mRepeatW; }

        /** @return Sampler border color for Clamp to border repeat mode */
        ESamplerBorderColor getBorderColor() const { return mBorderColor; }

        /** @return True if sample filtering uses mipmaps */
        bool getUsesMips() const { return mUseMips; }

        /** @return Min possible lod number */
        int32 getMinLodLevel() const { return mMinLod; }

        /** @return Max possible lod number */
        int32 getMaxLodLevel() const { return mMaxLod; }

    protected:

        /** Min possible lod number */
        int8 mMinLod;

        /** Max possible lod number */
        int8 mMaxLod;

        /** True if sample filtering uses mipmaps */
        bool mUseMips : 1;

        /** Sampler minification filter */
        ESamplerFilter mMin :2;

        /** Sampler magnification filter */
        ESamplerFilter mMag : 2;

        /** Sampler filter along mip levels */
        ESamplerFilter mMipmapMode : 2;

        /** Repeat mode along u axis (coordinate) */
        ESamplerRepeatMode mRepeatU : 4;

        /** Repeat mode along v axis (coordinate) */
        ESamplerRepeatMode mRepeatV : 4;

        /** Repeat mode along w axis (coordinate) */
        ESamplerRepeatMode mRepeatW : 4;

        /** Sampler border color for Clamp to border repeat mode */
        ESamplerBorderColor mBorderColor : 4;

    };

}

#endif //BERSERK_RHISAMPLER_H
