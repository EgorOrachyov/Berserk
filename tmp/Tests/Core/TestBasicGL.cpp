/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(BasicGlFixture)

TEST_F(BasicGlFixture, SetupGl) {
    if (WindowManager::GetBackendType() == WindowManager::Backend::Glfw) {
        volatile bool finish = false;

        auto exitCallback = [&](const Window::EventData& data) {
            BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

            if (data.eventType == Window::EventType::CloseRequested) {
                finish = true;
            }
        };

        Window::Desc desc;
        desc.name = BERSERK_TEXT("MAIN-WINDOW");
        desc.title = BERSERK_TEXT("Test rendering window");
        desc.size = Math::Size2i(1280, 720);

        auto window = WindowManager::CreateWindow(desc);
        auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

        // Primary window is created. Now we are able to initialize RHI device.
        // In the user applications, this function will be automatically called by GuiApplication class.
        InitializeRHI();
        // At this moment we are able to make RHI calls

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

                // This is not a modern and safe method, but it works under some circumstances
                glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }
    }
}

BERSERK_GTEST_MAIN