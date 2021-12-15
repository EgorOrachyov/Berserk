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
#include <core/event/EventDropInput.hpp>
#include <core/event/EventJoystick.hpp>
#include <core/event/EventKeyboard.hpp>
#include <core/event/EventMouse.hpp>
#include <core/string/Unicode.hpp>
#include <platform/glfw/GlfwInput.hpp>
#include <platform/glfw/GlfwInputDefs.hpp>

#include <algorithm>

BRK_NS_BEGIN

GlfwInput::GlfwInput() {
    mMouse = Ref<GlfwMouse>(new GlfwMouse);
    mKeyboard = Ref<GlfwKeyboard>(new GlfwKeyboard);
    CheckConnectedJoysticks();
}

Ref<Mouse> GlfwInput::GetMouse() {
    return mMouse.As<Mouse>();
}

Ref<Keyboard> GlfwInput::GetKeyboard() {
    return mKeyboard.As<Keyboard>();
}

Ref<Joystick> GlfwInput::GetJoystick() {
    return mJoysticks.empty() ? Ref<Joystick>() : mJoysticks.front().As<Joystick>();
}

void GlfwInput::SubscribeWindow(GLFWwindow *window) {
    glfwSetDropCallback(window, DropCallback);
    glfwSetCursorPosCallback(window, MousePositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonsCallback);
    glfwSetKeyCallback(window, KeyboardKeysCallback);
    glfwSetCharCallback(window, KeyboardTextCallback);
    glfwSetJoystickCallback(JoystickCallback);
}

void GlfwInput::PreUpdate() {
    // Pass
}

void GlfwInput::PostUpdate() {
    // Update joysticks state
    for (auto &joystick : mJoysticks)
        if (joystick->GetState() == InputDeviceState::Connected)
            joystick->Update();
}

void GlfwInput::CheckConnectedJoysticks() {
    for (int jid = 0; jid < GLFW_JOYSTICK_LAST; jid++) {
        if (glfwJoystickPresent(jid)) {
            mJoysticks.push_back(Ref<GlfwJoystick>(new GlfwJoystick(jid)));
        }
    }
}

Ref<GlfwJoystick> GlfwInput::GetJoystick(int32 jid) {
    auto query = std::find_if(mJoysticks.begin(), mJoysticks.end(), [=](Ref<GlfwJoystick> &joystick) {
        return joystick->GetHND() == jid;
    });
    return query == mJoysticks.end() ? Ref<GlfwJoystick>() : *query;
}

#define BRK_GLFW_INPUT_CALLBACK_SETUP  \
    auto &engine = Engine::Instance(); \
    auto &dispatcher = engine.GetEventDispatcher();

#define BRK_GLFW_INPUT_CALLBACK_SETUP_EXT \
    BRK_GLFW_INPUT_CALLBACK_SETUP         \
    auto &input = engine.GetInput();      \
    auto &glfwInput = *dynamic_cast<GlfwInput *>(&input);

void GlfwInput::DropCallback(GLFWwindow *window, int32 count, const char **paths) {
    if (count > 0) {
        std::vector<String> pathsArray;
        pathsArray.reserve(count);

        for (int32 i = 0; i < count; i++)
            pathsArray.push_back(std::move(String(paths[i])));

        BRK_GLFW_INPUT_CALLBACK_SETUP

        Ref<EventDropInput> event(new EventDropInput());
        event->SetPaths(std::move(pathsArray));
        dispatcher.Dispatch(event.As<Event>());
    }
}

void GlfwInput::MousePositionCallback(GLFWwindow *window, double x, double y) {
    BRK_GLFW_INPUT_CALLBACK_SETUP_EXT

    Point2f position(static_cast<float>(x), static_cast<float>(y));
    glfwInput.mMouse->UpdatePosition(position);
    Ref<EventMouse> event(new EventMouse);
    event->SetAction(InputAction::Move);
    event->SetPosition(glfwInput.mMouse->GetPosition());
    event->SetDelta(glfwInput.mMouse->GetDelta());
    dispatcher.Dispatch(event.As<Event>());
}

void GlfwInput::MouseButtonsCallback(GLFWwindow *window, int32 button, int32 action, int32 mods) {
    BRK_GLFW_INPUT_CALLBACK_SETUP_EXT

    auto mask = GlfwInputDefs::GetModsMask(mods);
    auto mouseButton = GlfwInputDefs::GetMouseButton(button);
    auto mouseAction = GlfwInputDefs::GetAction(action);

    if (mouseButton != InputMouseButton::Unknown && mouseAction != InputAction::Unknown) {
        glfwInput.mMouse->UpdateButton(mouseButton, mouseAction);
        Ref<EventMouse> event(new EventMouse);
        event->SetAction(mouseAction);
        event->SetButton(mouseButton);
        event->SetModifiers(mask);
        dispatcher.Dispatch(event.As<Event>());
    }
}

void GlfwInput::KeyboardKeysCallback(GLFWwindow *window, int32 key, int32 scancode, int32 action, int32 mods) {
    BRK_GLFW_INPUT_CALLBACK_SETUP_EXT

    auto mask = GlfwInputDefs::GetModsMask(mods);
    auto keyboardKey = GlfwInputDefs::GetKeyboardKey(key);
    auto keyboardAction = GlfwInputDefs::GetAction(action);

    if (keyboardKey != InputKeyboardKey::Unknown && keyboardAction != InputAction::Unknown) {
        glfwInput.mKeyboard->UpdateKey(keyboardKey, keyboardAction);
        Ref<EventKeyboard> event(new EventKeyboard);
        event->SetAction(keyboardAction);
        event->SetKey(keyboardKey);
        event->SetModifiers(mask);
        dispatcher.Dispatch(event.As<Event>());
    }
}

void GlfwInput::KeyboardTextCallback(GLFWwindow *window, unsigned int codePoint) {
    BRK_GLFW_INPUT_CALLBACK_SETUP

    uint32 length;
    char buffer[8];
    Unicode::Utf32toUtf8(codePoint, buffer, length);

    if (length > 0) {
        Ref<EventKeyboard> event(new EventKeyboard);
        event->SetText(std::move(String(buffer, length)));
        event->SetAction(InputAction::Text);
        dispatcher.Dispatch(event.As<Event>());
    }
}

void GlfwInput::JoystickCallback(int32 jid, int32 state) {
    BRK_GLFW_INPUT_CALLBACK_SETUP_EXT

    auto joystick = glfwInput.GetJoystick(jid);
    auto deviceState = state == GLFW_CONNECTED ? InputDeviceState::Connected : InputDeviceState::Disconnected;

    // If found (previously was connected, update state)
    if (joystick.IsNotNull())
        joystick->UpdateState(deviceState);
    // If connected and not found, add new one
    if (joystick.IsNull() && deviceState == InputDeviceState::Connected) {
        joystick = Ref<GlfwJoystick>(new GlfwJoystick(jid));
        glfwInput.mJoysticks.push_back(joystick);
    }

    if (joystick.IsNotNull()) {
        Ref<EventJoystick> event(new EventJoystick);
        event->SetJoystick(joystick.As<Joystick>());
        event->SetAction(InputAction::State);
        dispatcher.Dispatch(event.As<Event>());
    }
}

#undef BRK_GLFW_INPUT_CALLBACK_SETUP_EXT
#undef BRK_GLFW_INPUT_CALLBACK_SETUP

BRK_NS_END