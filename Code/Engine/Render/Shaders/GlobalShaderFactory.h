/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLOBALSHADERFACTORY_H
#define BERSERK_GLOBALSHADERFACTORY_H

#include <Paths.h>
#include <Shader.h>
#include <ShaderFactory.h>

namespace Berserk {
    namespace Render {

        /** Default factory for shaders loaded from file */
        class GlobalShaderFactory : public ShaderFactory {
        public:

            /** Default */
            GlobalShaderFactory();

            /** Not supported */
            TPtrShared<Shader> create() override;

            /** Load shader by name  */
            TPtrShared<Shader> create(const CString &shaderName) override;

        private:

            /** Cached instances of the specific shaders */
            TMap<CString,TPtrShared<Shader>> mCached;
            EPathType mPathType = EPathType::Root;

        };

    }
}




#endif //BERSERK_GLOBALSHADERFACTORY_H
