/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/Texture2D.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        Texture2D::Texture2D(CString textureName, const Image &image, bool mipMaps) {
            auto& device = RHIDevice::getSingleton();

            // Default sampling options
            RHISamplerDesc samplerDesc{};

            mTextureName = std::move(textureName);
            mSRGB = image.isInSRGB();
            mTextureRHI = device.createTexture2D(EBufferUsage::Static, true, image);
            mSamplerRHI = device.createSampler(samplerDesc);
        }
        
        Texture2D::Texture2D(CString textureName, TPtrShared<RHITexture> texture, TPtrShared<RHISampler> sampler) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null RHI texture");
            BERSERK_COND_ERROR_RET(sampler.isNotNull(), "Passed null RHI sampler");

            mTextureName = std::move(textureName);
            mTextureRHI = std::move(texture);
            mSamplerRHI = std::move(sampler);
        }

        bool Texture2D::isInitialized() const {
            return mTextureRHI.isNotNull() && mSamplerRHI.isNotNull();
        }

        bool Texture2D::isInitializedRHI() const {
            return mTextureRHI.isNotNull() && mSamplerRHI.isNotNull();
        }

        TPtrShared<Texture> Texture2D::instance(const CString &textureName) {
            auto* memory = Memory::allocate(sizeof(Texture2D));
            auto texture = new (memory) Texture2D();

            texture->mTextureName = textureName;
            texture->mTextureRHI = mTextureRHI;
            texture->mSamplerRHI = mSamplerRHI;
            texture->mTransparentColor = mTransparentColor;
            texture->mUseTransparentColor = mUseTransparentColor;
            texture->mSRGB = mSRGB;

            return TPtrShared<Texture>(texture, &Memory::DEFAULT_DEALLOC);
        }


    }
}