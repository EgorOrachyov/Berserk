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

#include <core/event/EventJoystick.hpp>
#include <utility>

BRK_NS_BEGIN

const EventType &EventJoystick::GetEventType() const {
    return GetEventTypeStatic();
}

const EventType &EventJoystick::GetEventTypeStatic() {
    static EventType eventType(BRK_TEXT("_brk_core_event_joystick"));
    return eventType;
}

void EventJoystick::SetJoystick(Ref<Joystick> joystick) {
    mJoystick = std::move(joystick);
}

void EventJoystick::SetModifiers(InputModifiers modifiers) {
    mModifiers = modifiers;
}

void EventJoystick::SetAction(InputAction action) {
    mAction = action;
}

void EventJoystick::SetAxis(InputJoystickAxis axis) {
    mAxis = axis;
}

void EventJoystick::SetButton(InputJoystickButton button) {
    mButton = button;
}

const Ref<Joystick> &EventJoystick::GetJoystick() const {
    return mJoystick;
}

InputModifiers EventJoystick::GetModifiers() const {
    return mModifiers;
}

InputAction EventJoystick::GetAction() const {
    return mAction;
}

InputJoystickAxis EventJoystick::GetAxis() const {
    return mAxis;
}

InputJoystickButton EventJoystick::GetButton() const {
    return mButton;
}

BRK_NS_END