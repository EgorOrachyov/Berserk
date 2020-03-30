/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <glad/glad.h>

#include <Platform/ISystem.h>
#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestOpenGL)
{
    BERSERK_TEST(OpenGL)
    {
        ISystem::VideoMode videoMode{};
        ISystem::getSingleton().initialize("Test OpenGL", videoMode, ERenderDeviceType::OpenGL);

        BERSERK_COND_ERROR_FAIL(gladLoadGL(), "Failed to load glad");

        while (!ISystem::getSingleton().shouldClose(ISystem::MAIN_WINDOW)) {
            ISystem::getSingleton().update();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    };
}

