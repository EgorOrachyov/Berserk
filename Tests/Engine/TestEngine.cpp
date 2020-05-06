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

using namespace Berserk;

BERSERK_TEST_SECTION(Engine)
{
    BERSERK_TEST_COND(EngineStartUp, true)
    {
        Engine engine;
        ConsoleManager consoleManager;
        ImageImporter imageImporter;
        RenderModule renderModule;

        auto& system = ISystem::getSingleton();


        engine.initialize("../../../Engine/", false);

        while (!system.shouldClose(ISystem::MAIN_WINDOW)) {
            engine.update();

            if (engine.getFramesCount() % 222 == 0) {
                consoleManager.forEachConsoleObjectWithPrefix("e.", [](const IConsoleObject& object){
                    if (object.isVariable())
                        BERSERK_LOG_INFO("%s = %s", object.getName().data(), ((IConsoleVariable&)object).getString().data());
                });
            }

            if (engine.getFramesCount() == 65) {
                OutputDeviceStd outputDeviceStd;
                IConsoleManager::getSingleton().processUserInput("e.Fps ?", outputDeviceStd);
            }

            if (engine.getFramesCount() == 66) {
                OutputDeviceStd outputDeviceStd;
                IConsoleManager::getSingleton().processUserInput("e.Fps 25", outputDeviceStd);
            }
        }

        engine.finalize();
    };
}
