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

        float32 color = 0.0f;
        float32 step = 1.0f / 300.0f;

        auto t = TimeValue::now();
        auto d = TimeValue().setMilliseconds(30.0f);

        while (!ISystem::getSingleton().shouldClose(ISystem::MAIN_WINDOW)) {
            ISystem::getSingleton().update();

            auto next = color + step;
            color = (next > 1.0f ? 0.0f : next);

            auto p = glClearColor;

            glClearColor(color, 0.3f, 1.0f - color, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            t = TimeValue::wait(t + d);
        }
    };
}

