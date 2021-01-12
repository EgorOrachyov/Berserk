/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Debug/Debug.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(GuiFixture)

TEST_F(GuiFixture, FileDialog) {
    String title = BERSERK_TEXT("Choose files");
    String defaultPath = BERSERK_TEXT("./");
    Array<String> paths;
    Array<String> patterns;

    Platform::FileSystem::OpenFileDialog(title, defaultPath, patterns, paths);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Files selected: {0}"), paths.GetSize());
    for (auto i = 0; i < paths.GetSize(); i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File[{0}]: \"{1}\""), i, paths[i]);
    }
}

TEST_F(GuiFixture, FolderDialog) {
    String title = BERSERK_TEXT("Choose folder");
    String defaultPath = BERSERK_TEXT("./");
    String path;

    bool selected = Platform::FileSystem::OpenFolderDialog(title, defaultPath, path);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Folder selected: {0}"), selected);

    if (selected) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Folder: \"{0}\""), path);
    }
}

TEST_F(GuiFixture, SaveDialog) {
    String title = BERSERK_TEXT("Choose file to save");
    String defaultPath = BERSERK_TEXT("./");
    String defaultName = BERSERK_TEXT("your_file.txt");
    String filePath;
    Array<String> patterns;

    bool selected = Platform::FileSystem::OpenSaveDialog(title, defaultPath, defaultName, patterns, filePath);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File selected: {0}"), selected);

    if (selected) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File= \"{0}\""), filePath);
    }
}

TEST_F(GuiFixture, FileDialogWithPatterns) {
    String title = BERSERK_TEXT("Choose files");
    String defaultPath = BERSERK_TEXT("./");
    Array<String> paths;
    Array<String> patterns = { BERSERK_TEXT("*.txt") };

    Platform::FileSystem::OpenFileDialog(title, defaultPath, patterns, paths);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Files selected: {0}"), paths.GetSize());
    for (auto i = 0; i < paths.GetSize(); i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File[{0}]: \"{1}\""), i, paths[i]);
    }
}

TEST_F(GuiFixture, SaveDialogWithPatterns) {
    String title = BERSERK_TEXT("Choose file to save");
    String defaultPath = BERSERK_TEXT("./");
    String defaultName = BERSERK_TEXT("your_file.txt");
    String filePath;
    Array<String> patterns = { BERSERK_TEXT("*.txt") };

    bool selected = Platform::FileSystem::OpenSaveDialog(title, defaultPath, defaultName, patterns, filePath);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File selected: {0}"), selected);

    if (selected) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File: \"{0}\""), filePath);
    }
}

TEST_F(GuiFixture, BasicWindow) {
    volatile bool finish = false;

    auto exitCallback = [&](const Platform::Window::EventData& data) {
        BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

        if (data.eventType == Platform::Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    Platform::Window::Desc desc;
    desc.name = BERSERK_TEXT("MAIN-WINDOW");
    desc.title = BERSERK_TEXT("Test berserk window");
    desc.size = Math::Size2i(1280, 720);

    auto window = Platform::WindowManager::CreateWindow(desc);
    auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

    while (!finish) {
        FixedUpdate();
    }
}

TEST_F(GuiFixture, SeveralWindows) {
    volatile bool finish = false;

    auto exitCallback = [&](const Platform::Window::EventData& data) {
        BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

        if (data.eventType == Platform::Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    Platform::Window::Desc desc;
    desc.name = BERSERK_TEXT("MAIN-WINDOW");
    desc.title = BERSERK_TEXT("Test window");
    desc.size = Math::Size2i(1280, 720);

    auto mainWindow = Platform::WindowManager::CreateWindow(desc);
    auto eventHnd = mainWindow->OnWindowEvent.Subscribe(exitCallback);

    uint32 iterations = 8;
    Array<Ref<Platform::Window>> windows;
    Array<EventHnd> windowsCloseEventHnds;

    for (auto i = 0; i < iterations; i++) {
        Platform::Window::Desc windowDesc;
        windowDesc.name = String{BERSERK_TEXT("WINDOW-")} + String::From(i);
        windowDesc.title = String{BERSERK_TEXT("Test window ")} + String::From(i);
        windowDesc.size = Math::Size2i(400 + i * 40, 300 + i * 20);

        auto window = windows.Add(Platform::WindowManager::CreateWindow(windowDesc));

        auto hnd = window->OnWindowEvent.Subscribe([=](const Platform::Window::EventData& data) {
            if (data.eventType == Platform::Window::EventType::CloseRequested) {
                window->Close();
            }
        });

        windowsCloseEventHnds.Add(hnd);
    }

    while (!finish) {
        FixedUpdate();
    }

    EXPECT_TRUE(Platform::WindowManager::GetWindowByName(BERSERK_TEXT("MAIN-WINDOW")).IsNotNull());
}

TEST_F(GuiFixture, WindowIcon) {
    volatile bool finish = false;

    auto exitCallback = [&](const Platform::Window::EventData& data) {
        if (data.eventType == Platform::Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    auto icon = BERSERK_TEXT("icon.jpeg");

    Platform::Window::Desc desc;
    desc.name = BERSERK_TEXT("MAIN-WINDOW");
    desc.title = BERSERK_TEXT("Test berserk window");
    desc.size = Math::Size2i(1280, 720);
    desc.icon = Image::Load(icon, Image::Channels::RGBA);

    auto window = Platform::WindowManager::CreateWindow(desc);

    auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

    while (!finish) {
        FixedUpdate();
    }
}

TEST_F(GuiFixture, WindowUpdateThreaded) {
    volatile bool finish = false;

    auto exitCallback = [&](const Platform::Window::EventData& data) {
        BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

        if (data.eventType == Platform::Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    auto processThreadJob = [&](){
        auto icon = BERSERK_TEXT("icon.jpeg");

        Platform::Window::Desc desc;
        desc.name = BERSERK_TEXT("MAIN-WINDOW");
        desc.title = BERSERK_TEXT("Test berserk window");
        desc.size = Math::Size2i(1280, 720);
        desc.icon = Image::Load(icon, Image::Channels::RGBA);

        auto window = Platform::WindowManager::CreateWindow(desc);
        auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

        while (!finish) {
            FixedUpdate();
        }
    };

    auto processThread = Platform::ThreadManager::CreateThread(BERSERK_TEXT("PROCESS-THREAD"), processThreadJob);
    processThread->Join();
}

BERSERK_GTEST_MAIN