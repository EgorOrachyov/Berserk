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
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <BerserkCore/Platform/Dialogs.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Debug/Debug.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(GuiFixture)

TEST_F(GuiFixture, FileDialogSingleSelect) {
    String title = BERSERK_TEXT("Choose files");
    String defaultPath = BERSERK_TEXT("./");
    Array<String> paths;
    Array<String> patterns;

    Dialogs::OpenFileDialog(title, defaultPath, false, patterns, paths);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Files selected: {0}"), paths.GetSize());
    for (auto i = 0; i < paths.GetSize(); i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File[{0}]: \"{1}\""), i, paths[i]);
    }
}

TEST_F(GuiFixture, FileDialogMultipleSelect) {
    String title = BERSERK_TEXT("Choose files");
    String defaultPath = BERSERK_TEXT("./");
    Array<String> paths;
    Array<String> patterns;

    Dialogs::OpenFileDialog(title, defaultPath, true, patterns, paths);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Files selected: {0}"), paths.GetSize());
    for (auto i = 0; i < paths.GetSize(); i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File[{0}]: \"{1}\""), i, paths[i]);
    }
}

TEST_F(GuiFixture, FolderDialog) {
    String title = BERSERK_TEXT("Choose folder");
    String defaultPath = BERSERK_TEXT("./");
    String path;

    bool selected = Dialogs::OpenFolderDialog(title, defaultPath, path);

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

    bool selected = Dialogs::OpenSaveDialog(title, defaultPath, defaultName, patterns, filePath);

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

    Dialogs::OpenFileDialog(title, defaultPath, false, patterns, paths);

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

    bool selected = Dialogs::OpenSaveDialog(title, defaultPath, defaultName, patterns, filePath);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File selected: {0}"), selected);

    if (selected) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("File: \"{0}\""), filePath);
    }
}

TEST_F(GuiFixture, Notifications) {
    Dialogs::SendNotification("Test Info", "Berserk info notification.", Dialogs::NotificationType::Info);
    Dialogs::SendNotification("Test Warning", "Berserk warning notification.", Dialogs::NotificationType::Warning);
    Dialogs::SendNotification("Test Error", "Berserk error notification.", Dialogs::NotificationType::Error);
    Dialogs::SendNotification("Test Question", "Berserk question notification?", Dialogs::NotificationType::Question);
}

TEST_F(GuiFixture, BasicWindow) {
    volatile bool finish = false;

    auto exitCallback = [&](const Window::EventData& data) {
        BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

        if (data.eventType == Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    Window::Desc desc;
    desc.name = BERSERK_TEXT("MAIN-WINDOW");
    desc.title = BERSERK_TEXT("Test berserk window");
    desc.size = Math::Size2i(1280, 720);

    auto window = WindowManager::CreateWindow(desc);
    auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

    while (!finish) {
        FixedUpdate();
    }
}

TEST_F(GuiFixture, SeveralWindows) {
    volatile bool finish = false;

    auto exitCallback = [&](const Window::EventData& data) {
        BERSERK_CORE_LOG_INFO("Event type: {0}", data.eventType);

        if (data.eventType == Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    Window::Desc desc;
    desc.name = BERSERK_TEXT("MAIN-WINDOW");
    desc.title = BERSERK_TEXT("Test window");
    desc.size = Math::Size2i(1280, 720);

    auto mainWindow = WindowManager::CreateWindow(desc);
    auto eventHnd = mainWindow->OnWindowEvent.Subscribe(exitCallback);

    uint32 iterations = 8;
    Array<SharedPtr<Window>> windows;
    Array<EventHnd> windowsCloseEventHnds;

    for (auto i = 0; i < iterations; i++) {
        Window::Desc windowDesc;
        windowDesc.name = String{BERSERK_TEXT("WINDOW-")} + String::Fromi32(i);
        windowDesc.title = String{BERSERK_TEXT("Test window ")} + String::Fromi32(i);
        windowDesc.size = Math::Size2i(400 + i * 40, 300 + i * 20);

        auto window = windows.Add(WindowManager::CreateWindow(windowDesc));

        auto hnd = window->OnWindowEvent.Subscribe([=](const Window::EventData& data) {
            if (data.eventType == Window::EventType::CloseRequested) {
                window->Close();
            }
        });

        windowsCloseEventHnds.Add(hnd);
    }

    while (!finish) {
        FixedUpdate();
    }

    EXPECT_TRUE(WindowManager::GetWindowByName(BERSERK_TEXT("MAIN-WINDOW")).IsNotNull());
}

TEST_F(GuiFixture, WindowIcon) {
    volatile bool finish = false;

    auto exitCallback = [&](const Window::EventData& data) {
        if (data.eventType == Window::EventType::CloseRequested) {
            finish = true;
        }
    };

    auto icon = BERSERK_TEXT("../../Engine/Resources/Textures/icon.jpeg");

    Window::Desc desc;
    desc.name = BERSERK_TEXT("MAIN-WINDOW");
    desc.title = BERSERK_TEXT("Test berserk window");
    desc.size = Math::Size2i(1280, 720);
    desc.icon = Image::Load(icon, Image::Channels::RGBA);

    auto window = WindowManager::CreateWindow(desc);

    auto eventHnd = window->OnWindowEvent.Subscribe(exitCallback);

    while (!finish) {
        FixedUpdate();
    }
}

BERSERK_GTEST_MAIN