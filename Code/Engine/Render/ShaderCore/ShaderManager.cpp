/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderManager.h>

namespace Berserk {
    namespace Render {

        ShaderManager* ShaderManager::gManager = nullptr;

        ShaderManager::ShaderManager() {
            if (gManager) {
                fprintf(stderr, "[BERSERK Render] Allowed only single ShaderManager global instance");
                return;
            }

            gManager = this;
        }

        ShaderManager::~ShaderManager() {
            // nothing
        }

        void ShaderManager::registerFactory(Berserk::Render::ShaderFactory &factory) {
            auto ptr = &factory;
            auto& type = factory.getFactoryType();

            BERSERK_COND_ERROR_RET(!mFactoryByType.contains(type), "Factory (%s) already registered", type.data());
            mFactoryByType.add(type, ptr);
        }

        TPtrShared<Shader> ShaderManager::load(const Berserk::CString &shaderType) {
            BERSERK_COND_ERROR_RET_VALUE(nullptr, mFactoryByType.contains(shaderType), "No such factory type: %s", shaderType.data());

            auto ptr = mFactoryByType[shaderType];
            return ptr->create();
        }

        ShaderManager& ShaderManager::getSingleton() {
            return *gManager;
        }

    }
}