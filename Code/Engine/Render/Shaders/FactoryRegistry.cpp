/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Shaders/FactoryRegistry.h>
#include <ShaderCore/ShaderManager.h>

namespace Berserk {
    namespace Render {

        FactoryRegistry::FactoryRegistry() {
            auto& manager = ShaderManager::getSingleton();

            manager.registerFactory(testGeometryShader);
            manager.registerFactory(batchedSphereShader);
        }

    }
}