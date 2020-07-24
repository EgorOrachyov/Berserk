/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TEXTURE_H
#define BERSERK_TEXTURE_H

#include <RenderResources/RenderResource.h>
#include <RHI/RHITexture.h>
#include <RHI/RHISampler.h>

namespace Berserk {
    namespace Rendering {

        class Texture : public RenderResource {
        public:

            ~Texture() override = default;

            /** @return Texture name for debug */
            CString getFriendlyName() const override { return mTextureName; }

            /** @return RHI texture resource */
            const TPtrShared<RHITexture> &getTextureRHI() const { return mTextureRHI; }

            /** @return RHI sampler resource, used to sample this texture */
            const TPtrShared<RHISampler> &getSamplerRHI() const { return mSamplerRHI; }

            /** @return Last time texture was bound for rendering  */
            TimeValue getLastRenderTime() const { return mLastRenderTime; }

            /** @return True if texture in srgb color space */
            bool isInSRGB() const { return mSRGB; }

        protected:

            /** Texture name for debug */
            CString mTextureName;

            /** RHI texture resource */
            TPtrShared<RHITexture> mTextureRHI;

            /** RHI sampler resource, used to sample this texture */
            TPtrShared<RHISampler> mSamplerRHI;

            /** Last time texture was bound for rendering  */
            TimeValue mLastRenderTime = TimeValue::nowAsTime();

            /** True if texture in srgb color space */
            bool mSRGB = false;

        };

    }
}




#endif //BERSERK_TEXTURE_H
