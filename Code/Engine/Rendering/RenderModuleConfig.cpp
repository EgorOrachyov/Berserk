/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderModuleConfig.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        RenderModuleConfig::RenderModuleConfig() {
            auto& device = RHIDevice::getSingleton();

            mTargetDeviceType = device.getDeviceType();
            mShadersPath = "Shaders/";
            mShadersCachePath = "Shaders/Cache/";
            mShaderCacheMetaName = "ShaderCacheEntries.meta";
            mShaderProfileName = "DefaultShaderProfile";
        }

    }
}