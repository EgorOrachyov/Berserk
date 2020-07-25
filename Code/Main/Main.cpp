/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Main.h>

namespace Berserk {

    void Main::initialize(int32 arc, const char **argv) {
        gEngine = TPtrUnique<Engine>::make();

        // Required for proper logging setup
        gEngine->initialize(false);

        gConsoleManager = TPtrUnique<ConsoleManager>::make();
        gModuleManager = TPtrUnique<ModuleManager>::make();
        gUpdateManager = TPtrUnique<UpdateManager>::make();
        gImageImporter = TPtrUnique<ImageImporter>::make();
        gRenderModule = TPtrUnique<Render::RenderModule>::make();

        // After that point: all the engine singletons exist and can be used
        // for example: now can register console variables and etc.

        // Now can initialize engine console variables
        gEngine->initializeConsoleVariables();

        // Notify modules registered: call post init
        gModuleManager->initialize();
    }

    void Main::enterMainLoop() {
        static System& system = System::getSingleton();

        // Engine itself can request close, and here it will be seen
        while (!system.isCloseRequested()) {
            execSingleIteration();
        }
    }

    void Main::execSingleIteration() {
        // Eval frame stat, update platform system and other stuff
        gEngine->update();

        // Load, unload modules before main update
        gModuleManager->update();

        // Run actual main loop updates per system
        gUpdateManager->update();
    }

    void Main::finalize() {
        // Unload all modules
        gModuleManager->finalize();

        // Reverse order
        gRenderModule.free();
        gImageImporter.free();
        gUpdateManager.free();
        gModuleManager.free();
        gConsoleManager.free();

        // Engine close
        gEngine->finalize();
        gEngine.free();
    }

}