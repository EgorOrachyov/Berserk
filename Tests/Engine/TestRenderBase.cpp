/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>
#include <Main.h>

#include <Platform/WindowManager.h>
#include <ShaderManager.h>
#include <RenderResources/VertexArrayBuilder.h>

using namespace Berserk;
using namespace Render;

BERSERK_TEST_SECTION(TestRenderBase)
{
    BERSERK_TEST_COND(Main, false)
    {
        Main main;
        main.initialize(0, nullptr);
        main.enterMainLoop();
        main.finalize();
    };

    BERSERK_TEST_COND(RenderBase, true)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& system = System::getSingleton();
        auto& device = RHIDevice::getSingleton();
        auto& shaderManager = ShaderManager::getSingleton();
        auto  window = WindowManager::getSingleton().find("MAIN_WINDOW");

        auto shader = shaderManager.load("TestGeometry");

        auto drawList = device.createDrawList();
        {

        }

        while (!system.isCloseRequested()) {
            main.execSingleIteration();

            device.beginRenderFrame();
            device.submitDrawList(drawList);
            device.endRenderFrame();
        }

        main.finalize();
    };
}