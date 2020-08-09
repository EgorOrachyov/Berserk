/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Main.h>
#include <Engine.h>
#include <RHI/RHIDevice.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/System.h>
#include <Graphics.h>
#include <ConsoleRenderer.h>
#include <RenderTargets/WindowTarget.h>
#include <LogMacro.h>
#include <Input/TextInput.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestConsole)
{
    BERSERK_TEST_COND(TestConsole, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& engine = Engine::getSingleton();
            auto& system = System::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& windowManager = WindowManager::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");
            auto windowSize = window->getSize();

            auto windowTarget = TPtrShared<Render::WindowTarget>::make(window);

            TPtrShared<Render::Graphics> graphics = TPtrShared<Render::Graphics>::make(windowSize, Region2i(0,0,windowSize), (TPtrShared<Render::RenderTarget>) windowTarget);
            Render::ConsoleRenderer consoleRenderer(graphics);
            TextInput textInput;

            auto drawList = device.createDrawList();
            auto query = device.createTimeQuery();

            while (!window->shouldClose()) {
                main.execSingleIteration();

                if (window->getSize() != windowSize) {
                    windowSize = window->getSize();
                    windowTarget->update();
                    graphics->fitAreaToTarget();
                    graphics->fitRegionToTarget();
                }

                {
                    // Will be in the render module update line
                    graphics->clear();
                    consoleRenderer.setInputText(textInput.getTextAsString());
                    consoleRenderer.update();
                    consoleRenderer.draw();
                }

                {
                    drawList->begin();
                    drawList->beginQuery(query);
                    windowTarget->bind(*drawList, {EClearOption::Color,EClearOption::Depth});
                    graphics->draw(*drawList);
                    drawList->endQuery(query);
                    drawList->end();
                }

                device.beginRenderFrame();
                device.submitDrawList(drawList);
                device.endRenderFrame();

                if (input.isKeyPressed(EKeyboardKey::F1)) {
                    consoleRenderer.openFull();
                    textInput.setActive(true);
                }

                if (input.isKeyPressed(EKeyboardKey::F2)) {
                    consoleRenderer.closeFull();
                    textInput.setActive(false);
                }

                if (input.isKeyPressed(EKeyboardKey::Up)) {
                    consoleRenderer.scrollUp();
                }

                if (input.isKeyPressed(EKeyboardKey::Down)) {
                    consoleRenderer.scrollDown();
                }

                while (!query->isResultAvailable());
            }
        }
        main.finalize();
    };
}