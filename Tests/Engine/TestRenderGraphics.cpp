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
#include <Graphics.h>
#include <RHI/RHIDevice.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/System.h>
#include <RenderTargets/WindowTarget.h>
#include <Paths.h>
#include <Font.h>
#include <Image.h>
#include <Math/Random.h>
#include <ResourceImporters.h>
#include <RenderResources/GpuFont.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderGraphics)
{
    BERSERK_TEST_COND(GraphicsRendering, true)
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

            Render::GraphicsPen pen;
            pen.setUsingAlpha(true);
            Render::Graphics graphics(windowSize, Region2i(0,0,windowSize), (TPtrShared<Render::RenderTarget>) windowTarget);

            auto drawList = device.createDrawList();

            // Bitmap data to create image
            uint32 bitmap[] = {
                    Color4f(1.0f,0.0f,0.0f).toR8G8B8A8(), Color4f(0.0f,1.0f,0.0f).toR8G8B8A8(),
                    Color4f(0.0f,0.0f,1.0f).toR8G8B8A8(), Color4f(1.0f,0.0f,1.0f).toR8G8B8A8()
            };

            TPtrShared<Render::Texture2D> texture;
            TPtrShared<Render::GpuFont> gpuFont;
            {
                auto path = Paths::getFullPathFor("Assets/Fonts/Arial.ttf", EPathType::Engine);
                auto importer = ResourceImporters::getSingleton().findImporterFromPath(path);
                TPtrShared<Resource> font;
                auto result = importer->import(font, path, nullptr);
                auto fontPtr = (Font*) font.getPtr();
                fontPtr->showDebugInfo();
                gpuFont = TPtrShared<Render::GpuFont>::make(*fontPtr);
                texture = gpuFont->getBitmapTexture();
            }


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
                    graphics.fitAreaToTarget();
                    graphics.fitRegionToTarget();
                }

                {
                    drawList->begin();
                    drawList->beginQuery(query);
                    windowTarget->bind(*drawList, {EClearOption::Color,EClearOption::Depth});
                    graphics.draw(*drawList);
                    drawList->endQuery(query);
                    drawList->end();
                }

                device.beginRenderFrame();
                device.submitDrawList(drawList);
                device.endRenderFrame();

                static bool inState = false;

                if (input.isButtonPressed(EMouseButton::Left)) {
                    inState = true;
                }

                if (input.isButtonReleased(EMouseButton::Left)) {
                    inState = false;
                }

                auto point = input.getMousePosition();

                if (inState) {
                    auto color = Color4f((float) point.x() / windowSize.width(), (float) point.y() / windowSize.height(), 0.3f, 0.5f);

                    pen.setUsingAlpha(false);
                    pen.setColor(color);
                    //graphics.drawLine(pen, point, point + Size2i(20,20), 9);
                    //graphics.drawFilledEllipse(pen, point, { 50, 40 }, 16);
                    //graphics.drawFilledCircle(pen, point, 60, 32);
                    //graphics.drawRect(pen, point, { 50, 40 }, 2);
                    graphics.drawEllipse(pen, point, {50,40}, 32, 4);
                }

                if (input.isKeyPressed(EKeyboardKey::I)) {
                    pen.setUsingAlpha(true);
                    pen.setColor(Color4f(1.0f,1.0f,1.0f));
                    graphics.drawTexture(pen, point, texture);
                }

                if (input.isKeyPressed(EKeyboardKey::C)) {
                    graphics.clear();
                }

                if (input.isKeyPressed(EKeyboardKey::T)) {
                    printf("Rendering time (GPU): %fms\n", renderingTime);
                    printf("Frame time (CPU): %fms\n", cpuTime);
                    printf("Sync time (CPU): %fms (FPS: %f)\n", syncTime, fps);
                    printf("Memory (CPU): alloc: %llu free: %llu\n", Memory::getAllocCalls(), Memory::getFreeCalls());
                }

                while (!query->isResultAvailable());

            }
        }
        main.finalize();
    };
}