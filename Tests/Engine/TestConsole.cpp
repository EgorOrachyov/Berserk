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

            class Collector : public LogListener {
            public:
                Render::ConsoleRenderer* console;
                Collector(Render::ConsoleRenderer* console) : console(console) {}

                void onLog(ELogType logType, const char *message) override {
                    WString m = WString::from(CString(message));
                    console->addEntries({m}, {logType});
                }
            } collector(&consoleRenderer);

            system.getLog().addListener(collector);

            auto drawList = device.createDrawList();
            auto query = device.createTimeQuery();

            while (!window->shouldClose()) {
                main.execSingleIteration();

                auto renderingTime = query->tryGetElapsedTime().getMilliseconds();
                auto cpuTime = engine.getFrameTimePerformance() * 1000.0f;
                auto syncTime = engine.getFrameTime() * 1000.0f;
                auto fps = engine.getFPS();

                if (window->getSize() != windowSize) {
                    windowSize = window->getSize();
                    windowTarget->update();
                    graphics->fitAreaToTarget();
                    graphics->fitRegionToTarget();
                }

                {
                    // Will be in the render module update line
                    graphics->clear();
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

                if (input.isKeyPressed(EKeyboardKey::O)) {
                    consoleRenderer.openPart();
                }

                if (input.isKeyPressed(EKeyboardKey::P)) {
                    consoleRenderer.openFull();
                }

                if (input.isKeyPressed(EKeyboardKey::L)) {
                    consoleRenderer.closeFull();
                }

                if (input.isKeyPressed(EKeyboardKey::T)) {
                    const WString s[] = { L"Write some command", L"Hello", L"It is text!" };
                    static int32 i = 0; int32 c = sizeof(s) / sizeof(s[0]);

                    consoleRenderer.setInputText(s[i]);
                    i = (i + 1) % c;
                }

                if (input.isKeyPressed(EKeyboardKey::A)) {
                    consoleRenderer.addEntries({ L"ERROR: function: looks like error",L"WARNING: this on looks like warning message...",L"INFO: do something with this one in any way"  }, { ELogType::Error, ELogType::Warning, ELogType::Info });
                }

                if (input.isKeyPressed(EKeyboardKey::C)) {
                    consoleRenderer.clearEntries();
                }

                if (input.isKeyPressed(EKeyboardKey::Up)) {
                    consoleRenderer.scrollUp();
                }

                if (input.isKeyPressed(EKeyboardKey::Down)) {
                    consoleRenderer.scrollDown();
                }

                if (input.isKeyPressed(EKeyboardKey::S)) {
                    BERSERK_LOG_WARNING("Rendering time (GPU): %fms\n", renderingTime);
                    BERSERK_LOG_WARNING("Frame time (CPU): %fms\n", cpuTime);
                    BERSERK_LOG_WARNING("Sync time (CPU): %fms (FPS: %f)\n", syncTime, fps);
                    BERSERK_LOG_WARNING("Memory (CPU): alloc: %llu free: %llu\n", Memory::getAllocCalls(), Memory::getFreeCalls());
                }

                while (!query->isResultAvailable());
            }

            // Note to remove, since console is destroyed
            system.getLog().removeListener(collector);
        }
        main.finalize();
    };
}