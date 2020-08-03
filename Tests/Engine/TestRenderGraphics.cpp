/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Main.h>
#include <Graphics.h>
#include <GraphicsRenderer.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/System.h>
#include <RenderTargets/WindowTarget.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderGraphics)
{
    BERSERK_TEST_COND(GraphicsRendering, true)
    {
        Main main;
        main.initialize(0, nullptr);

        // References to common engine singletons
        auto& system = System::getSingleton();
        auto& input = Input::getSingleton();
        auto& device = RHIDevice::getSingleton();
        auto& windowManager = WindowManager::getSingleton();

        // Default window, created on engine start-up
        auto window = windowManager.find("MAIN_WINDOW");
        auto windowSize = window->getSize();

        auto windowTarget = TPtrShared<Render::WindowTarget>::make(window);

        Render::Graphics graphics(Size2i(1280,720),Color4f(0.1f,0.1f,0.1f));
        Render::GraphicsRenderer renderer(graphics, (TPtrShared<Render::RenderTarget>) windowTarget);


        auto drawList = device.createDrawList();

        // Bitmap data to create image
        uint32 bitmap[] = {
                Color4f(1.0f,0.0f,0.0f).toR8G8B8A8(), Color4f(0.0f,1.0f,0.0f).toR8G8B8A8(),
                Color4f(0.0f,0.0f,1.0f).toR8G8B8A8(), Color4f(1.0f,0.0f,1.0f).toR8G8B8A8()
        };

        Image image;
        image.create(2,2,EPixelFormat::R8G8B8A8, (const uint8*) bitmap, false);
        auto texture = TPtrShared<Render::Texture2D>::make("White", image, false);

        while (!window->shouldClose()) {
            main.execSingleIteration();

            if (window->getSize() != windowSize) {
                windowSize = window->getSize();
                graphics.setGraphicsSize(windowSize);
                renderer.setRegion(Region2i(0,0,windowSize));

                // todo: remove
                windowTarget->update();
            }

            {
                drawList->begin();
                renderer.draw(*drawList);
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

            if (inState) {
                auto point = input.getMousePosition();
                // Draw image under mouse in area of size 40 x 40 pixels
                graphics.drawTexture(point, texture, Size2i(40,40));
            }

            if (input.isKeyPressed(EKeyboardKey::K)) {
                graphics.clear();
            }
        }

        main.finalize();
    };
}