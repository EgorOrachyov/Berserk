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

namespace Berserk {

    static Mutex gModulesMutex;
    static TArray<IModule*> gModules;

    IModule::IModule() {
        registerModule();
    }

    IModule::~IModule() {
        unregisterModule();
    }

    void IModule::registerModule() {
        Guard guard(gModulesMutex);
        gModules.add(this);
    }

    void IModule::unregisterModule() {
        Guard guard(gModulesMutex);
        gModules.removeElement(this);
    }

    IModule* IModule::getModule(const char *name) {
        Guard guard(gModulesMutex);

        for (auto module: gModules) {
            if (CStringUtility::compare(name, module->getModuleName()) == 0)
                return module;
        }

        return nullptr;
    }

}
