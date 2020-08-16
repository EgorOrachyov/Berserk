/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Main.h>
#include <Platform/System.h>
#include <Engine.h>
#include <ModuleManager.h>
#include <UpdateManager.h>
#include <ResourceImporters.h>
#include <Console/ConsoleManager.h>
#include <Reflection/ClassManager.h>
#include <DefaultImporters.h>
#include <RenderModule.h>
#include <Debug/Console.h>
#include <Debug/ConsoleMessages.h>
#include <CoreRegisterClasses.h>

namespace Berserk {

    Main::Main() {
        // keep empty
    }

    Main::~Main() {
        // keep empty
    }

    void Main::initialize(int32 argc, const char **argv) {
        // Initialize first to be able to collect messages for input stage
        gConsoleMessages = Ptr<ConsoleMessages>::make();
        gEngine = TPtrUnique<Engine>::make();

        // Required for proper logging setup
        gEngine->initialize(false);

        gConsoleManager = Ptr<ConsoleManager>::make();
        gClassManager = Ptr<ClassManager>::make();
        gModuleManager = Ptr<ModuleManager>::make();
        gUpdateManager = Ptr<UpdateManager>::make();
        gResourceImporters = Ptr<ResourceImporters>::make();
        gImporters = Ptr<DefaultImporters>::make();
        gRenderModule = Ptr<Render::RenderModule>::make();

        // Register reflection info classes
        CoreRegisterClasses::registerClasses();

        // After that point: primary engine singletons exist and can be used
        // for example: now can register console variables and etc.

        // Now can initialize engine console variables
        gEngine->initializeConsoleVariables();

        // Notify modules registered: call post init
        gModuleManager->initialize();

        // Main engine logic setup
        gConsole = Ptr<Console>::make();

        // Setup console variables values from config file
        // This must be called after all the engine components are created.
        gEngine->initializeConsoleVariablesFromConfig();
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
        gConsole.free();

        // Unload all modules
        gModuleManager->finalize();

        // Reverse order
        gRenderModule.free();
        gImporters.free();
        gResourceImporters.free();
        gUpdateManager.free();
        gModuleManager.free();
        gClassManager.free();
        gConsoleManager.free();

        // Engine close
        gEngine->finalize();
        gEngine.free();
        gConsoleMessages.free();
    }

}