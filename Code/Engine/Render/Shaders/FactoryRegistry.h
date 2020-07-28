/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FACTORYREGISTRY_H
#define BERSERK_FACTORYREGISTRY_H

#include <Shaders/TestGeometryShader.h>

namespace Berserk {
    namespace Render {

        class FactoryRegistry {
        public:
            FactoryRegistry();
            ~FactoryRegistry() = default;

        private:
            TestGeometryShader testGeometryShader;
        };


    }
}



#endif //BERSERK_FACTORYREGISTRY_H
