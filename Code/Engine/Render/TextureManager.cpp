/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TextureManager.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        TextureManager* TextureManager::gTextureManager = nullptr;

        TextureManager::TextureManager() {
            BERSERK_COND_ERROR_RET(gTextureManager == nullptr, "Allowed only single TextureManager instance");
            gTextureManager = this;

            auto& device = RHIDevice::getSingleton();

            // Default filtering
            RHISamplerDesc samplerDesc;
            samplerDesc.useMips = false;
            samplerDesc.min = ESamplerFilter::Nearest;
            samplerDesc.mag = ESamplerFilter::Nearest;
            samplerDesc.mipmapMode = ESamplerFilter::Nearest;
            samplerDesc.u = ESamplerRepeatMode::Repeat;
            samplerDesc.v = ESamplerRepeatMode::Repeat;

            auto sampler = device.createSampler(samplerDesc);

            mWhiteTexture = TPtrShared<Texture2D>::make("WhiteTexture", device.getWhiteTexture(), sampler);
            mBlackTexture = TPtrShared<Texture2D>::make("BlackTexture", device.getBlackTexture(), sampler);
        }

        TextureManager::~TextureManager() {
            gTextureManager = nullptr;
        }

        TextureManager& TextureManager::getSingleton() {
            return *gTextureManager;
        }

    }
}