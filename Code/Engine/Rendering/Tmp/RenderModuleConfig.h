/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERMODULECONFIG_H
#define BERSERK_RENDERMODULECONFIG_H

#include <String/CString.h>
#include <Platform/System.h>

namespace Berserk {
    namespace Rendering {

        /** Global rendering module configuration entries */
        class RenderModuleConfig {
        public:
            RenderModuleConfig();
            ~RenderModuleConfig() = default;

            // todo: load from ini file

            ERenderDeviceType getTargetDeviceType() const { return mTargetDeviceType; };
            const CString& getShadersPath() const { return mShadersPath; }
            const CString& getShadersCachePath() const { return mShadersCachePath; }
            const CString& getShaderCacheMetaName() const { return mShaderCacheMetaName; }
            const CString& getShaderProfileName() const { return mShaderProfileName; }

        protected:
            ERenderDeviceType mTargetDeviceType;
            CString mShadersPath;
            CString mShadersCachePath;
            CString mShaderCacheMetaName;
            CString mShaderProfileName;
        };

    }
}

#endif //BERSERK_RENDERMODULECONFIG_H