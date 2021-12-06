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

#include <platform/Application.hpp>

#include <chrono>

BRK_NS_BEGIN

int Application::Run(int argc, const char *const *argv) {
    // Parse args
    mArgs = std::make_shared<ArgumentParser>();
    mArgs->Parse(argc, argv);

    // Setup engine
    mEngine = std::unique_ptr<Engine>(new Engine());
    mEngine->Init();

    // Post init call
    OnInitialize();

    using clock = std::chrono::steady_clock;
    using ns = std::chrono::nanoseconds;

    auto time = clock::now();

    // Main loop
    while (!mEngine->CloseRequested()) {
        auto newTime = clock::now();
        auto dt = static_cast<double>(std::chrono::duration_cast<ns>(newTime - time).count()) / 1.0e9;

        mEngine->Update(static_cast<float>(dt));

        time = newTime;
    }

    // Pre-finalize call
    OnFinalize();

    // Release engine
    mEngine.reset();

    return 0;
}

BRK_NS_END