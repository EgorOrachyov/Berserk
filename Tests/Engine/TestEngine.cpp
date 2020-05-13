/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Engine.h>
#include <LogMacro.h>
#include <Platform/ISystem.h>
#include <Console/ConsoleManager.h>
#include <Rendering/RenderModule.h>
#include <ImageImporter/ImageImporter.h>

#include <Rendering/RenderTargetProxy.h>

using namespace Berserk;
using namespace Berserk::Rendering;

BERSERK_TEST_SECTION(Engine)
{
    BERSERK_TEST_COND(EngineStartUp, true)
    {
        Engine engine;
        ConsoleManager consoleManager;
        ImageImporter imageImporter;
        RenderModule renderModule;

        engine.initialize("../../../Engine/", false);

        auto& system = ISystem::getSingleton();

        while (!system.shouldClose(ISystem::MAIN_WINDOW)) {
            engine.update();

            // Dump CVars
            if (engine.getFramesCount() == 2000) {
                IConsoleManager::getSingleton().forEachConsoleObjectWithPrefix("", [](const IConsoleObject& obj){
                    if (obj.isVariable()) {
                        auto& var = (IConsoleVariable&)obj;
                        printf("%s = %s\n%s\n", var.getName().data(), var.getString().data(), var.getHelpText().data());
                    }
                });
            }
        }

        engine.finalize();
    };
}
