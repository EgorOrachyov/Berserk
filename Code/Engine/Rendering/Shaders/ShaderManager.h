/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERMANAGER_H
#define BERSERK_SHADERMANAGER_H

#include <Rendering/Resources/Shader.h>
#include <Rendering/Shaders/ShaderCache.h>
#include <Rendering/Shaders/ShaderProfile.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Shader manager
         * Manages compiled and loaded in engine shaders.
         * Allows to reuse and share shaders in order to reduce number of GPU state changes.
         */
        class ShaderManager final {
        public:

            ShaderManager(CString prefixPath);
            ~ShaderManager();

            /**
             * Loads shader profile from name.
             * @note Looks for file in engine shaders directory.
             * @param name Name of shader profile
             * @return True if profile is loaded
             */
            bool loadProfile(const CString &name);

            /**
             * Attempt to find shader in loaded shaders cache.
             *
             * @param name Shader global name (names used as ids)
             * @return Reference to initialized shader (null if failed to find)
             */
            TPtrShared<Shader> findShader(const CString& name) const;

            /**
             * Attempts to load shader with specified name.
             * If shader already loaded, simply returns reference, otherwise attempts to
             * loaded shader from default shader cache. If failed to load shader from
             * cache attempt to compile shader from sources accordingly default shader profile.
             *
             * @param name Shader global name (names used as ids)
             * @return Reference to initialized shader (null if failed to load)
             */
            TPtrShared<Shader> loadShader(const CString& name);

            /** Default profile for loading shaders from source code */
            const ShaderProfile& getDefaultProfile() const { return mShaderProfiles[mDefaultProfileId]; }

        private:

            void loadShaderSource(const CString &path, TArray<uint8> &source);

            CString mPrefixPath;
            TRef<ShaderCache> mDefaultCache;
            TMap<CString,TPtrShared<Shader>> mShaders;
            TArray<ShaderProfile> mShaderProfiles;
            uint32 mDefaultProfileId = 0;
        };

    }
}

#endif //BERSERK_SHADERMANAGER_H