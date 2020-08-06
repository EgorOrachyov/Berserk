/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MAIN_H
#define BERSERK_MAIN_H

#include <TPtrUnique.h>
#include <RenderModuleFwd.h>

namespace Berserk {

    /**
     * @brief Engine main
     *
     * Create instance of this class to instantiate engine
     * an all its subsystems, required for the work.
     */
    class Main {
    public:

        Main();
        ~Main();

        /** Initialize engine with main arguments pass */
        void initialize(int32 argc, const char **argv);

        /** Enters main application loop. Returns control only on engine exit */
        void enterMainLoop();

        /** Execute single iteration of the main loop */
        void execSingleIteration();

        /** Finalize engine state. Release all resources */
        void finalize();

    private:

        template <typename T>
        using Ptr = TPtrUnique<T>;

        Ptr<class Engine> gEngine;
        Ptr<class ConsoleManager> gConsoleManager;
        Ptr<class ClassManager> gClassManager;
        Ptr<class ModuleManager> gModuleManager;
        Ptr<class UpdateManager> gUpdateManager;
        Ptr<class ResourceImporters> gResourceImporters;
        Ptr<class DefaultImporters> gImporters;
        Ptr<class Render::RenderModule> gRenderModule;

    };

}

#endif //BERSERK_MAIN_H