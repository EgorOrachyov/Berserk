/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ModuleManager.h>

namespace Berserk {

    ModuleManager* ModuleManager::gModuleManager = nullptr;

    ModuleManager::ModuleManager() {
        if (gModuleManager) {
            fprintf(stderr, "[BERSERK Core] Only single ModuleManager could be set as singleton");
            return;
        }

        gModuleManager = this;
    }

    ModuleManager::~ModuleManager() {
        update();

        if (mRawModules.size() > 0) {
            BERSERK_ERROR("Missing unloaded modules (%u)", mRawModules.size());
        }
    }

    void ModuleManager::registerModule(Module &module) {
        mPendingRegister.add(&module);
    }

    ModuleManager& ModuleManager::getSingleton() {
        return *gModuleManager;
    }

    void ModuleManager::update() {
        // Register modules

        for (auto module: mPendingRegister) {
            if (mRawModules.contains(module)) {
                BERSERK_ERROR("Module already registered: %s", module->getModuleName());
                continue;
            }

            mRawModules.add(module);
        }

        mPendingRegister.clear();
    }

    void ModuleManager::initialize() {
        update();

        for (auto& module: mRawModules) {
            module->onPostLoad();
        }
    }

    void ModuleManager::finalize() {
        update();

        for (auto& module: mRawModules) {
            module->onPreUnload();
        }

        mRawModules.clearNoDestructorCall();
    }

}