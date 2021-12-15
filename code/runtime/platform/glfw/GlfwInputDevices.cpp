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
#include <core/event/EventJoystick.hpp>
#include <platform/glfw/GlfwInputDefs.hpp>
#include <platform/glfw/GlfwInputDevices.hpp>

#include <algorithm>

BRK_NS_BEGIN

GlfwMouse::GlfwMouse() {
    mName = std::move(StringName(BRK_TEXT("_brk_platform_glfw_mouse")));
    mState = InputDeviceState::Connected;
    mButtons.resize(MAX_BUTTONS, InputAction::Unknown);
}

void GlfwMouse::UpdatePosition(const Point2f &pos) {
    mDelta = pos - mPosition;
    mPosition = pos;
}

void GlfwMouse::UpdateButton(InputMouseButton button, InputAction action) {
    mButtons[static_cast<size_t>(button)] = action;
}

GlfwKeyboard::GlfwKeyboard() {
    mName = std::move(StringName(BRK_TEXT("_brk_platform_glfw_keyboard")));
    mState = InputDeviceState::Connected;
    mKeys.resize(MAX_KEYS, InputAction::Unknown);
}

void GlfwKeyboard::UpdateKey(InputKeyboardKey key, InputAction action) {
    mKeys[static_cast<size_t>(key)] = action;
}

GlfwJoystick::GlfwJoystick(int32 HND) {
    mHND = HND;

    mName = std::move(StringName(glfwGetJoystickName(mHND)));
    mGUID = std::move(StringName(glfwGetJoystickGUID(mHND)));
    mState = InputDeviceState::Connected;

    int32 axesCount;
    int32 buttonsCount;

    glfwGetJoystickAxes(mHND, &axesCount);
    glfwGetJoystickButtons(mHND, &buttonsCount);

    mAxes.resize(axesCount, 0.0f);
    mButtons.resize(buttonsCount, InputAction::Release);
}

void GlfwJoystick::Update() {
    auto &engine = Engine::Instance();
    auto &dispatcher = engine.GetEventDispatcher();

    int32 axesCount;
    auto pAxes = glfwGetJoystickAxes(mHND, &axesCount);
    assert(axesCount <= mAxes.size());
    for (int32 i = 0; i < axesCount; i++) {
        mAxes[i] = pAxes[i];
    }

    Ref<EventJoystick> axisUpdate(new EventJoystick);
    axisUpdate->SetJoystick(Ref<Joystick>(this));
    axisUpdate->SetAction(InputAction::Move);
    dispatcher.Dispatch(axisUpdate.As<Event>());

    int32 buttonsCount;
    auto pButtons = glfwGetJoystickButtons(mHND, &buttonsCount);
    assert(buttonsCount <= mButtons.size());
    for (int32 i = 0; i < buttonsCount; i++) {
        auto action = GlfwInputDefs::GetAction(pButtons[i]);

        // Something changed, dispatch event
        if (mButtons[i] != action) {
            Ref<EventJoystick> buttonUpdate(new EventJoystick);
            buttonUpdate->SetJoystick(Ref<Joystick>(this));
            buttonUpdate->SetAction(action);
            buttonUpdate->SetButton(static_cast<InputJoystickButton>(i));
            dispatcher.Dispatch(buttonUpdate.As<Event>());
        }

        mButtons[i] = action;
    }
}

void GlfwJoystick::UpdateState(InputDeviceState state) {
    mState = state;

    if (state == InputDeviceState::Connected)
        std::fill(mButtons.begin(), mButtons.end(), InputAction::Release);
}

BRK_NS_END
