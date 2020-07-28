/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TESTGEOMETRYSHADER_H
#define BERSERK_TESTGEOMETRYSHADER_H

#include <ShaderCore/ShaderFactory.h>

namespace Berserk {
    namespace Render {

        /** Test only shader factory for simple geometry */
        class TestGeometryShader : public ShaderFactory {
        public:
            TestGeometryShader() : ShaderFactory("TestGeometry") {}
            TPtrShared<Shader> create() override;

        private:
            TPtrShared<Shader> mCachedInstance;
        };

    }
}

#endif //BERSERK_TESTGEOMETRYSHADER_H