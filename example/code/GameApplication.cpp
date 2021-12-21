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

#include <GameApplication.hpp>

void GameApplication::OnWindowCreate() {
    BRK_INFO("Initialize game window");

    auto &engine = berserk::Engine::Instance();
    auto &windowManager = engine.GetWindowManager();

    // Create primary window
    auto window = windowManager.CreateWindow(berserk::StringName("MAIN"), {1280, 720}, "Example window");
}

void GameApplication::OnInitialize() {
    BRK_INFO("Initialize game application");

    auto &engine = berserk::Engine::Instance();
    auto &scheduler = engine.GetScheduler();
    auto &dispatcher = engine.GetEventDispatcher();

    // Capture window events
    dispatcher.Subscribe(berserk::EventWindow::GetEventTypeStatic(), [](const berserk::Event &_event) {
        auto event = dynamic_cast<const berserk::EventWindow *>(&_event);
        BRK_INFO("Window event: window=" << event->GetWindow()->GetName() << " type=" << static_cast<int>(event->GetType()));
        return false;
    });

    // Capture mouse events
    dispatcher.Subscribe(berserk::EventMouse::GetEventTypeStatic(), [](const berserk::Event &_event) {
        auto event = dynamic_cast<const berserk::EventMouse *>(&_event);
        BRK_INFO("Mouse event:"
                 << " pos=" << event->GetPosition()
                 << " delta=" << event->GetDelta()
                 << " action=" << static_cast<int>(event->GetAction())
                 << " button=" << static_cast<int>(event->GetButton()));
        return false;
    });

    // Capture keyboard events
    dispatcher.Subscribe(berserk::EventKeyboard::GetEventTypeStatic(), [](const berserk::Event &_event) {
        auto event = dynamic_cast<const berserk::EventKeyboard *>(&_event);
        BRK_INFO("Keyboard event:"
                 << " key=" << static_cast<int>(event->GetKey())
                 << " action=" << static_cast<int>(event->GetAction())
                 << " text=" << event->GetText());
        return false;
    });

    // Capture joystick events
    dispatcher.Subscribe(berserk::EventJoystick::GetEventTypeStatic(), [](const berserk::Event &_event) {
        auto event = dynamic_cast<const berserk::EventJoystick *>(&_event);
        auto &joystick = event->GetJoystick();

        // State event
        if (event->GetAction() != berserk::InputAction::Move) {
            BRK_INFO("Joystick event:"
                     << " state=" << (joystick->GetState() == berserk::InputDeviceState::Connected ? "+" : "-")
                     << " button=" << static_cast<int>(event->GetButton())
                     << " action=" << static_cast<int>(event->GetAction()));
        }

        return false;
    });

    // Finish application when close requested
    dispatcher.Subscribe(berserk::EventWindow::GetEventTypeStatic(), [](const berserk::Event &_event) {
        auto event = dynamic_cast<const berserk::EventWindow *>(&_event);
        if (event->GetType() == berserk::EventWindow::Type::CloseRequested) {
            BRK_INFO("Request engine close");
            berserk::Engine::Instance().RequestClose();
        }
        return false;
    });
}

void GameApplication::OnFinalize() {
    BRK_INFO("Finalize game application");
}
