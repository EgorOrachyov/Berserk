/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Main.h>
#include <TestMacro.h>
#include <Math/Vec3f.h>
#include <Math/Quatf.h>
#include <Math/Random.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <RenderModule.h>
#include <Scene/Scene.h>
#include <Scene/SceneView.h>

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
            auto& renderModule = Render::RenderModule::getSingleton();
            auto  view = renderModule.getPrimarySceneView();
            auto  scene = view->scene;
            auto& camera = *view->camera;
            auto& batch = *scene->mBatch;

            // Default window, created on engine start-up
            auto window = windowManager.find("MAIN_WINDOW");

            camera.move({0,0,4});

            while (!window->shouldClose()) {
                main.execSingleIteration();

                static auto angle = 0.0f;

                if (input.isKeyRepeated(EKeyboardKey::S)) {
                    batch.clear();

                    static Random random;
                    static float r = 0.5;
                    static float s = 0.02;
                    static float eps = 0.005;

                    auto color = Vec3f(random.from(0.01f, 0.9f),random.from(0.01f, 0.8f),random.from(0.01f, 0.9f));
                    auto position = Vec3f(0,0,-1);
                    auto rotation = Quatf::rotation({0.4,0.6,0.0}, angle);

                    r += s;

                    angle += 0.09f;

                    //batch.addBox(position, {r,r,r}, color, rotation);
                    //batch.addWireBox(position, {r+eps,r+eps,r+eps}, Color4f(1.0f,1.0f,1.0f), rotation);
                    batch.addCylinder(position, {r,r}, r, color, rotation);
                    batch.addWireCylinder(position, {r+eps,r+eps}, r+eps, Color4f(1.0f,1.0f,1.0f), rotation);
                }
            }
        }
        main.finalize();
    };
}