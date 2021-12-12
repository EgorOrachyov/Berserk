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

#include <core/Engine.hpp>
#include <core/io/Logger.hpp>
#include <core/io/LoggerListenerOutput.hpp>

BRK_NS_BEGIN

Engine::~Engine() {
    // Release in reverse order
    mWindowManager.reset();
    mEventDispatcher.reset();
    mScheduler.reset();
    mFileSystem.reset();
    mOutput.reset();

    // Remove global instance
    gEngine = nullptr;
}

void Engine::RequestClose() {
    mCloseRequested.store(true);
}

bool Engine::CloseRequested() {
    return mCloseRequested.load();
}

Output &Engine::GetOutput() {
    return *mOutput;
}

FileSystem &Engine::GetFileSystem() {
    return *mFileSystem;
}

Scheduler &Engine::GetScheduler() {
    return *mScheduler;
}

EventDispatcher &Engine::GetEventDispatcher() {
    return *mEventDispatcher;
}

WindowManager &Engine::GetWindowManager() {
    return *mWindowManager;
}

std::thread::id Engine::GetGameThreadId() const {
    return mGameThreadID;
}

Engine &Engine::Instance() {
    return *gEngine;
}

void Engine::Init() {
    // Setup logger
    LoggerListenerOutput listener;
    listener.SetName("Engine");
    listener.SetLevel(Logger::Level::Error);

#ifdef BERSERK_DEBUG
    listener.SetLevel(Logger::Level::Info);
#endif

    Logger::Instance().AddListener([=](const Logger::Entry &entry) { listener.OnEntry(entry); });

    mGameThreadID = std::this_thread::get_id();
    mOutput = std::unique_ptr<Output>(new Output());
    mFileSystem = std::unique_ptr<FileSystem>(new FileSystem());
    mScheduler = std::unique_ptr<Scheduler>(new Scheduler());
    mEventDispatcher = std::unique_ptr<EventDispatcher>(new EventDispatcher());

    // Provide singleton
    gEngine = this;
}

void Engine::Configure() {
}

void Engine::SetWindowManager(std::shared_ptr<WindowManager> windowManager) {
    mWindowManager = std::move(windowManager);
}

void Engine::Update(float dt) {
    mScheduler->Update(dt);
    mEventDispatcher->Update();
}

Engine *Engine::gEngine = nullptr;

BRK_NS_END