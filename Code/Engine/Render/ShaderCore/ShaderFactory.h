/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERFACTORY_H
#define BERSERK_SHADERFACTORY_H

#include <ShaderCore/Shader.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader factory
         *
         * Base class for any shader factory, which is capable of producing of
         * shader instances of the specific shader type.
         *
         * Factories are tracked by shader manager. From user perspective, shaders are
         * queried with shader manager by shader name and type, where manager redirects
         * this call to the specific factory, which is capable of serving this type of shaders.
         *
         * In order to implements new shader type, implement required virtual functions
         * of this class and register the instance of the factory with shader manager,
         * to expose new type of the factory to the engine.
         */
        class ShaderFactory {
        public:

            /** Factory name constructor */
            ShaderFactory(CString factoryName) : mFactoryName(std::move(factoryName)) {}

            /** @return Instance of the shader, provided by this factory */
            virtual TPtrShared<Shader> create() = 0;

            /** @return Factory type name, the same shaders, produced by this factory has */
            const CString& getFactoryType() const { return mFactoryName; };

        protected:

            CString mFactoryName;
        };

    }
}

#endif //BERSERK_SHADERFACTORY_H