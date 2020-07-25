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
        if (mModules.size() > 0) {
            BERSERK_ERROR("Missing unloaded modules (%u)", mModules.size());
        }
    }

    void ModuleManager::registerModule(Berserk::Module &module) {
        mPendingRegister.add(&module);
    }

    void ModuleManager::unregisterModule(Berserk::Module &module) {
        mPendingUnregister.add(&module);
    }

    ModuleManager& ModuleManager::getSingleton() {
        return *gModuleManager;
    }

    void ModuleManager::update() {
        // Unregister modules
        // Register modules

        for (auto module: mPendingUnregister) {
            if (!mModules.contains(module)) {
                BERSERK_ERROR("Module was not registered: %s", module->getModuleName());
                continue;
            }

            module->onPostFinalize();
            mModules.removeElement(module);
        }

        mPendingUnregister.clear();

        for (auto module: mPendingRegister) {
            if (mModules.contains(module)) {
                BERSERK_ERROR("Module already registered: %s", module->getModuleName());
                continue;
            }

            mModules.add(module);
            module->onPostInitialize();
        }

        mPendingRegister.clear();
    }

}