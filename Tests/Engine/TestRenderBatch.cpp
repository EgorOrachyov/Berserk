/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Main.h>
#include <TestMacro.h>
#include <Math/Random.h>
#include <BatchedElements.h>
#include <BatchedElementsRenderer.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestRenderBatch)
{
    BERSERK_TEST_COND(BatchedElements, true)
    {
        Main main;
        main.initialize(0, nullptr);
        {
            // References to common engine singletons
            auto& system = System::getSingleton();
            auto& input = Input::getSingleton();
            auto& device = RHIDevice::getSingleton();
            auto& windowManager = WindowManager::getSingleton();

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");
            auto windowSize = window->getSize();

            Render::BatchedElements elements;
            Render::BatchedElementsRenderer renderer;

            Render::ViewData viewData;

            auto view = Mat4x4f::lookAt({0,0,4}, {0,0,-4}, Vec3f::Y_AXIS);
            auto proj = Mat4x4f::perspective(Math::degToRad(100.0f), window->getAspectRation(), 0.01, 200.0f);
            auto projView = proj * view;

            viewData.viewMatrix = view;
            viewData.projectionMatrix = proj;
            viewData.projectionViewMatrix = projView;

            RHIWindowPassOptions windowPass;
            {
                windowPass.clearColor = Color4f{ 0.0f, 0.0f, 0.0f, 0.0f };
                windowPass.viewport = Region2i{ 0, 0, windowSize[0], windowSize[1] };
                windowPass.clearMask = { EClearOption::Color, EClearOption::Depth };
            }

            auto drawList = device.createDrawList();
            auto writeList = [&]()
            {
                windowPass.viewport = Region2i{ 0, 0, windowSize[0], windowSize[1] };

                drawList->begin();
                drawList->bindWindow(window, windowPass);
                renderer.draw(viewData, elements, *drawList);
                drawList->end();
            };

            while (!window->shouldClose()) {
                main.execSingleIteration();

                if (window->getSize() != windowSize) {
                    windowSize = window->getSize();
                }

                if (input.isKeyRepeated(EKeyboardKey::S)) {
                    static float r = 0.1;
                    static float s = 0.05;

                    Random random;
                    auto color = Vec3f(random.from(0.01f, 0.9f),random.from(0.01f, 0.8f),random.from(0.01f, 0.9f));
                    auto position = Vec3f(0,0,0);
                    auto radius = r; r += s;

                    elements.addWireSphere(position, radius, color);
                }

                {
                    writeList();
                    device.beginRenderFrame();
                    device.submitDrawList(drawList);
                    device.endRenderFrame();
                }
            }
        }
        main.finalize();
    };
}