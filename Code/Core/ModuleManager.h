/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MODULEMANAGER_H
#define BERSERK_MODULEMANAGER_H

#include <Module.h>

namespace Berserk {

    /** Manages all the modules, loaded in the engine */
    class ModuleManager {
    public:

        ModuleManager();
        ~ModuleManager();

        /** Register module. When module actually registered, it gets called post init callback */
        void registerModule(Module& module);

        /** Register module. When module actually registered, it gets called post finalize callback */
        void unregisterModule(Module& module);

        /** @return Singleton module manager instance */
        static ModuleManager& getSingleton();

    private:

        /** Called to update module manager state */
        void update();
        void initialize();
        void finalize();

        friend class Main;

        TArray<Module*> mPendingRegister;
        TArray<Module*> mPendingUnregister;
        TArray<Module*> mModules;

        static ModuleManager* gModuleManager;

    };

}

#endif //BERSERK_MODULEMANAGER_H