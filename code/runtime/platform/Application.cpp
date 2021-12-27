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
    gEngine->InitCore();

    // Create platform window manager
    // NOTE: use glfw, it is sufficient for know
    auto gWindowManager = std::make_shared<GlfwWindowManager>(true, true);
    gEngine->SetWindowManager(gWindowManager);
    gEngine->SetInput(gWindowManager->mInput);

    // Create primary window
    OnWindowCreate();
    // If app does not create window, create default
    gEngine->ConfigureWindow();

    // Initialize rendering thread first and set it
    auto gRhiThread = std::make_shared<Thread>();
    gEngine->SetRHIThread(gRhiThread);

    // Then it is safe to create device
    auto gRhiDevice = GLDevice::Make(gWindowManager->GetMakeContextCurrentFunc(), gWindowManager->GetSwapBuffersFunc());
    gEngine->SetRHIDevice(gRhiDevice);

    // After RHI is created
    gEngine->InitEngine();

    // Post init call
    OnInitialize();

    using clock = std::chrono::steady_clock;
    using ns = std::chrono::nanoseconds;

    auto start = clock::now();
    auto time = start;

    // Main loop
    while (!gEngine->CloseRequested()) {
        auto newTime = clock::now();
        auto dt = static_cast<double>(std::chrono::duration_cast<ns>(newTime - time).count()) / 1.0e9;
        auto t = static_cast<double>(std::chrono::duration_cast<ns>(newTime - start).count()) / 1.0e9;

        // Swap queued messages
        gRhiThread->Update();

        // Execute before logic (resources setup etc.)
        gRhiThread->ExecuteBefore();
        // Execute update logic (queued resource update etc.)
        gRhiThread->ExecuteUpdate();

        // Custom pre-update
        OnPreUpdate();

        // Update engine, rendering and game
        gEngine->Update(static_cast<float>(t), static_cast<float>(dt));

        // Custom post-update
        OnPostUpdate();

        // Execute after logic (resources destruction etc.)
        gRhiThread->ExecuteAfter();

        // Poll platform events
        gWindowManager->PollEvents();

        time = newTime;
    }

    // Pre-finalize call
    OnFinalize();

    // Release RHI related stuff
    gRhiDevice.reset();
    gRhiThread.reset();

    // Release platform window manager
    gWindowManager.reset();

    // Release engine first
    gEngine.reset();

    return 0;
}

BRK_NS_END