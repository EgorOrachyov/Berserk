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
#include <Math/Random.h>

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

            Render::Graphics graphics(windowSize, Region2i(0,0,windowSize), (TPtrShared<Render::RenderTarget>) windowTarget);

            auto drawList = device.createDrawList();
            auto alpha = 0.3;

            // Bitmap data to create image
            uint32 bitmap[] = {
                    Color4f(1.0f,0.0f,0.0f,alpha).toR8G8B8A8(), Color4f(0.0f,1.0f,0.0f,alpha).toR8G8B8A8(),
                    Color4f(0.0f,0.0f,1.0f,alpha).toR8G8B8A8(), Color4f(1.0f,0.0f,1.0f,alpha).toR8G8B8A8()
            };

            Image image;
            image.create(2,2,EPixelFormat::R8G8B8A8, (const uint8*) bitmap, false);
            TPtrShared<Render::Texture2D> texture = TPtrShared<Render::Texture2D>::make("Bit", image, false);

            while (!window->shouldClose()) {
                main.execSingleIteration();

                if (window->getSize() != windowSize) {
                    windowSize = window->getSize();
                    graphics.setGraphicsSize(windowSize);
                    graphics.setDrawRegion(Region2i(0,0,windowSize));

                    // todo: remove
                    windowTarget->update();
                }

                {
                    drawList->begin();
                    windowTarget->bind(*drawList, {EClearOption::Color,EClearOption::Depth});
                    graphics.draw(*drawList);
                    drawList->end();
                }

                device.beginRenderFrame();
                device.submitDrawList(drawList);
                device.endRenderFrame();

                static Random rand;
                static bool inState = false;

                if (input.isButtonPressed(EMouseButton::Left)) {
                    inState = true;
                }

                if (input.isButtonReleased(EMouseButton::Left)) {
                    inState = false;
                }

                if (inState) {
                    auto point = input.getMousePosition();
                    graphics.drawTexture(point, texture, Size2i(40,40));
                }

                if (input.isKeyPressed(EKeyboardKey::C)) {
                    graphics.clear();
                }

                if (engine.getFramesCount() % 100 == 0) {
                    auto enable = !texture->isUsingAlpha();
                    texture->setAlpha(enable);
                }
            }
        }
        main.finalize();
    };
}