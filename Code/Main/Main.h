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
#include <Engine.h>
#include <ModuleManager.h>
#include <UpdateManager.h>
#include <Console/ConsoleManager.h>
#include <ImageImporter/ImageImporter.h>
#include <RenderModule.h>

namespace Berserk {

    /**
     * @brief Engine main
     *
     * Create instance of this class to instantiate engine
     * an all its subsystems, required for the work.
     */
    class Main {
    public:

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

        Ptr<Engine> gEngine;
        Ptr<ConsoleManager> gConsoleManager;
        Ptr<ModuleManager> gModuleManager;
        Ptr<UpdateManager> gUpdateManager;
        Ptr<ImageImporter> gImageImporter;
        Ptr<Render::RenderModule> gRenderModule;

    };

}

#endif //BERSERK_MAIN_H