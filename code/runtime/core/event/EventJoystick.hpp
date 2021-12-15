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

#ifndef BERSERK_EVENTJOYSTICK_HPP
#define BERSERK_EVENTJOYSTICK_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/event/Event.hpp>
#include <platform/InputDefs.hpp>
#include <platform/InputDevices.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class EventJoystick
 * @brief Joystick input event
 */
class EventJoystick final : public Event {
public:
    BRK_API EventJoystick() = default;
    BRK_API ~EventJoystick() override = default;

    /** @copydoc Event::GetType() */
    BRK_API const EventType &GetEventType() const override;
    /** @copydoc Event::GetType() */
    BRK_API static const EventType &GetEventTypeStatic();

    BRK_API void SetJoystick(Ref<Joystick> joystick);
    BRK_API void SetAction(InputAction action);
    BRK_API void SetButton(InputJoystickButton button);

    BRK_API const Ref<Joystick> &GetJoystick() const;
    BRK_API InputAction GetAction() const;
    BRK_API InputJoystickButton GetButton() const;

private:
    Ref<Joystick> mJoystick;
    InputAction mAction = InputAction::Unknown;
    InputJoystickButton mButton = InputJoystickButton::Unknown;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_EVENTJOYSTICK_HPP
