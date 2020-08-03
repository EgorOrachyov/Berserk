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

        Texture2D::Texture2D(const CString &textureName, const Image &image, bool mipMaps) {
            auto& device = RHIDevice::getSingleton();

            // todo: remove - add samplers cache
            RHISamplerDesc samplerDesc{};

            samplerDesc.mipmapMode = ESamplerFilter::Nearest;
            samplerDesc.min = ESamplerFilter::Nearest;
            samplerDesc.mag = ESamplerFilter::Nearest;

            mTextureName = textureName;
            mTextureRHI = device.createTexture2D(EBufferUsage::Static, true, image);
            mSamplerRHI = device.createSampler(samplerDesc);
        }

        bool Texture2D::isInitialized() const {
            return mTextureRHI.isNotNull() || mSamplerRHI.isNotNull();
        }

        bool Texture2D::isInitializedRHI() const {
            return mTextureRHI.isNotNull() || mSamplerRHI.isNotNull();
        }


    }
}