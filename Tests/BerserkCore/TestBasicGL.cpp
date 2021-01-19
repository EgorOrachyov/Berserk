/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>

#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>

// This will be always private for other engine systems & users
// Required only for testing
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(BasicGlFixture)

TEST_F(BasicGlFixture, SetupGl) {
    if (Platform::WindowManager::GetBackendType() == Platform::WindowManager::Backend::Glfw) {
        volatile bool finish = false;

        auto exitCallback = [&](const Platform::Window::EventData& data) {
            BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

            if (data.eventType == Platform::Window::EventType::CloseRequested) {
                finish = true;
            }
        };

        Platform::Window::Desc desc;
        desc.name = BERSERK_TEXT("MAIN-WINDOW");
        desc.title = BERSERK_TEXT("Test rendering window");
        desc.size = Math::Size2i(1280, 720);

        auto window = Platform::WindowManager::CreateWindow(desc);
        auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

        Math::Vec3f a = Math::Vec3f(1.0f);
        Math::Vec3f b = Math::Vec3f(0.0f, 0.5f, 1.0f);
        float t = 0;
        float step = 0.01f;

        while (!finish) {
            FixedUpdate();

            {
                Color clearColor(Math::Vec3f::Lerp(t, a, b));
                t = t + step;

                if (t > 1.0f) {
                    std::swap(a, b);
                    t = 0.0f;
                }

                // This is not a moder and safe method, but it works at some circumstances
                GLFWwindow* wHND = (GLFWwindow*) window->GetNativeHnd();
                glfwMakeContextCurrent(wHND);

                glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
                glClear(GL_COLOR_BUFFER_BIT);

                glfwSwapBuffers(wHND);
            }
        }
    }
}

BERSERK_GTEST_MAIN