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
    namespace Render {

        class Texture : public RenderResource {
        public:

            ~Texture() override = default;

            /** @return Texture name for debug */
            CString getFriendlyName() const override { return mTextureName; }

            /** @return RHI texture resource */
            const TPtrShared<RHITexture> &getTextureRHI() const { return mTextureRHI; }

            /** @return RHI sampler resource, used to sample this texture */
            const TPtrShared<RHISampler> &getSamplerRHI() const { return mSamplerRHI; }

            /** @return Color used to discard pixels */
            const Color4f& getTransparentColor() const { return mTransparentColor; }

            /** Set color used as transparent */
            void setTransparentColor(const Color4f& color) { mTransparentColor = color; }

            /** Enable transparency */
            void setTransparency(bool enable) { mUseTransparentColor = true; }

            /** Enable alpha */
            void setAlpha(bool enable) { mUseAlpha = enable; }

            /** @return True if texture in srgb color space */
            bool isInSRGB() const { return mSRGB; }

            /** @return True if can render texture with alpha blending */
            bool isUsingAlpha() const { return mUseAlpha; }

            /** @return True, if can use transparent color */
            bool isUsingTransparentColor() const { return mUseTransparentColor; }

            /** @return New instance of texture build from this one */
            virtual TPtrShared<Texture> instance(const CString& textureName) = 0;

        protected:

            /** Texture name for debug */
            CString mTextureName;

            /** RHI texture resource */
            TPtrShared<RHITexture> mTextureRHI;

            /** RHI sampler resource, used to sample this texture */
            TPtrShared<RHISampler> mSamplerRHI;

            /** Pixels of this color discarded on the rendering */
            Color4f mTransparentColor;

            /** True, if can use transparent color */
            bool mUseTransparentColor = false;

            /** True if texture in srgb color space */
            bool mSRGB = false;

            /** True if can render texture with alpha blending */
            bool mUseAlpha = false;

        };

    }
}




#endif //BERSERK_TEXTURE_H
