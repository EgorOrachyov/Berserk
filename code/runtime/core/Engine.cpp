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
    mResourceManager.reset();
    mRenderEngine.reset();
    mEventDispatcher.reset();
    mScheduler.reset();
    mRHIDevice.reset();
    mRHIThread.reset();
    mInput.reset();
    mWindowManager.reset();
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

const Config &Engine::GetConfig() {
    return mConfig;
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

Input &Engine::GetInput() {
    return *mInput;
}

RHIDevice &Engine::GetRHIDevice() {
    return *mRHIDevice;
}

Thread &Engine::GetRHIThread() {
    return *mRHIThread;
}

RenderEngine &Engine::GetRenderEngine() {
    return *mRenderEngine;
}

ResourceManager &Engine::GetResourceManager() {
    return *mResourceManager;
}

std::thread::id Engine::GetGameThreadId() const {
    return mGameThreadID;
}

float Engine::GetDeltaTime() const {
    return mDt;
}

float Engine::GetTime() const {
    return mT;
}

Engine &Engine::Instance() {
    return *gEngine;
}

void Engine::InitCore() {
    // Setup logger
    LoggerListenerOutput listener;
    listener.SetName("Engine");
    listener.SetLevel(Logger::Level::Error);

#ifdef BERSERK_DEBUG
    listener.SetLevel(Logger::Level::Info);
#endif

    // Logger setup first
    Logger::Instance().AddListener([=](const Logger::Entry &entry) { listener.OnEntry(entry); });

    // In order from lower level foundation to high-level system
    mGameThreadID = std::this_thread::get_id();
    mOutput = std::unique_ptr<Output>(new Output());
    mFileSystem = std::unique_ptr<FileSystem>(new FileSystem());
    mScheduler = std::unique_ptr<Scheduler>(new Scheduler());
    mEventDispatcher = std::unique_ptr<EventDispatcher>(new EventDispatcher());
    mRenderEngine = std::unique_ptr<RenderEngine>(new RenderEngine());
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager());

    // Provide singleton
    gEngine = this;

    // Set default search path
    mFileSystem->AddSearchPath(mFileSystem->GetExecutableDir());

    // Create config file
    mConfig.Open("config/engine.config.xml");
}

void Engine::InitEngine() {
    // Init call
    mRenderEngine->Init();
}

void Engine::ConfigureWindow() {
    // Create primary window if is not created
    if (mWindowManager->GetPrimaryWindow().IsNull()) {
        StringName sectionApp("application");
        auto w = mConfig.GetProperty(sectionApp, StringName("window.width"), 1280);
        auto h = mConfig.GetProperty(sectionApp, StringName("window.height"), 720);
        auto caption = mConfig.GetProperty(sectionApp, StringName("window.caption"), "Default window");
        auto name = mConfig.GetProperty(sectionApp, StringName("window.name"), "MAIN");
        mWindowManager->CreateWindow(StringName(name), Size2i{w, h}, caption);
    }
}

void Engine::SetWindowManager(std::shared_ptr<WindowManager> windowManager) {
    mWindowManager = std::move(windowManager);
}

void Engine::SetInput(std::shared_ptr<Input> input) {
    mInput = std::move(input);
}

void Engine::SetRHIDevice(std::shared_ptr<RHIDevice> device) {
    mRHIDevice = std::move(device);
}

void Engine::SetRHIThread(std::shared_ptr<Thread> thread) {
    mRHIThread = std::move(thread);
}

void Engine::Update(float t, float dt) {
    mT = t;
    mDt = dt;
    mScheduler->Update(dt);
    mEventDispatcher->Update();
    mRenderEngine->PreUpdate();
    mRenderEngine->PostUpdate();
}

Engine *Engine::gEngine = nullptr;

BRK_NS_END