/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IModule.h>
#include <TArray.h>
#include <Platform/Mutex.h>
#include <String/TStringUtility.h>
#include <Threading/TSync.h>

namespace Berserk {

    static TSync<TArray<IModule*>> gModules;

    IModule::IModule() {
        registerModule();
    }

    IModule::~IModule() {
        unregisterModule();
    }

    void IModule::registerModule() {
        TGuard<TArray<IModule*>> guard(gModules);
        guard->add(this);
    }

    void IModule::unregisterModule() {
        TGuard<TArray<IModule*>> guard(gModules);
        guard->removeElement(this);
    }

    TRef<IModule> IModule::getModule(const char *name) {
        TGuard<TArray<IModule*>> guard(gModules);

        for (auto module: guard.get()) {
            if (CStringUtility::compare(name, module->getModuleName()) == 0)
                return module;
        }

        return nullptr;
    }

}
