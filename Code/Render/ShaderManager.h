/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERMANAGER_H
#define BERSERK_SHADERMANAGER_H

#include <Shader.h>
#include <ShaderFactory.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader manager
         *
         * Manages loading and caching of the all shaders and shader type
         * (type factories) in the engine.
         */
        class ShaderManager {
        public:
            ShaderManager();
            ~ShaderManager();

            /** Register factory for creating shaders */
            void registerFactory(ShaderFactory& factory);

            /** @return Shader of specified type (null if failed load or create) */
            TPtrShared<Shader> load(const CString &factoryType);

            /** @return Shader of specified type and factory options (null if failed load or create) */
            TPtrShared<Shader> load(const CString &factoryType, const ShaderFactoryOptions& options);

            /** @return Shader of specified type (null if failed load or create) */
            TPtrShared<Shader> loadGlobalShader(const CString &shaderName);

            /** @return Singleton instance of render engine shader manager */
            static ShaderManager& getSingleton();

        private:
            TMap<CString,ShaderFactory*> mFactoryByType;

            /** Singleton instance */
            static ShaderManager* gManager;
        };

    }
}

#endif //BERSERK_SHADERMANAGER_H