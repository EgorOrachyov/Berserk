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

#include <rhi/opengl/GLDevice.hpp>

#include <platform/Application.hpp>
#include <platform/glfw/GlfwInput.hpp>
#include <platform/glfw/GlfwWindowManager.hpp>

#include <chrono>

BRK_NS_BEGIN

int Application::Run(int argc, const char *const *argv) {
    // Parse args
    gArgs = std::make_shared<ArgumentParser>();
    gArgs->Parse(argc, argv);

    // Setup engine
    gEngine = std::unique_ptr<Engine>(new Engine());
    gEngine->Init();

    // Create platform window manager
    // NOTE: use glfw, it is sufficient for know
    auto gWindowManager = std::make_shared<GlfwWindowManager>(true, true);
    gEngine->SetWindowManager(gWindowManager);
    gEngine->SetInput(gWindowManager->mInput);

    // Create primary window
    OnWindowCreate();

    // Initialize rendering device and thread
    auto gRhiDevice = GLDevice::Make(gWindowManager->GetMakeContextCurrentFunc(), gWindowManager->GetSwapBuffersFunc());
    auto gRhiThread = std::make_shared<Thread>();
    gEngine->SetRHIDevice(gRhiDevice);
    gEngine->SetRHIThread(gRhiThread);

    // Post init call
    OnInitialize();

    using clock = std::chrono::steady_clock;
    using ns = std::chrono::nanoseconds;

    auto time = clock::now();

    // Main loop
    while (!gEngine->CloseRequested()) {
        auto newTime = clock::now();
        auto dt = static_cast<double>(std::chrono::duration_cast<ns>(newTime - time).count()) / 1.0e9;

        // Swap queued messages
        gRhiThread->Update();

        // Execute before logic (resources setup etc.)
        gRhiThread->ExecuteBefore();
        // Execute update logic (queued resource update etc.)
        gRhiThread->ExecuteUpdate();

        // Update engine, rendering and game
        gEngine->Update(static_cast<float>(dt));

        // todo: will be removed <------------
        static auto window = gWindowManager->GetPrimaryWindow();
        static auto makeCurrent = gWindowManager->GetMakeContextCurrentFunc();
        static auto swapBuffers = gWindowManager->GetSwapBuffersFunc();
        static float factor = 0.0f;

        makeCurrent(window);
        auto dtf = static_cast<float>(dt);
        factor = (factor + dtf) > 1.0f ? (factor + dtf) - 1.0f : factor + dtf;
        glClearColor(1.0f * factor, 1.0f * (1.0f - factor * factor), 0.4f * factor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        swapBuffers(window);
        // todo: will be removed <------------

        // Execute after logic (resources destruction etc.)
        gRhiThread->ExecuteAfter();

        // Poll platform events
        gWindowManager->PollEvents();

        time = newTime;
    }

    // Pre-finalize call
    OnFinalize();

    // Release RHI related stuff
    gRhiThread.reset();
    gRhiDevice.reset();

    // Release platform window manager
    gWindowManager.reset();

    // Release engine first
    gEngine.reset();

    return 0;
}

BRK_NS_END