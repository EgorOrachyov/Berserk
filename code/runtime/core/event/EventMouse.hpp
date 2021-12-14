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

#ifndef BERSERK_EVENTMOUSE_HPP
#define BERSERK_EVENTMOUSE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/event/Event.hpp>
#include <core/input/InputDefs.hpp>
#include <core/math/TVecN.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class EventMouse
 * @brief Mouse input event
 */
class EventMouse final : public Event {
public:
    BRK_API EventMouse() = default;
    BRK_API ~EventMouse() override = default;

    /** @copydoc Event::GetType() */
    BRK_API const EventType &GetEventType() const override;
    /** @copydoc Event::GetType() */
    BRK_API static const EventType &GetEventTypeStatic();

    BRK_API void SetPosition(Point2i position);
    BRK_API void SetDelta(Size2i delta);
    BRK_API void SetModifiers(InputModifiers modifiers);
    BRK_API void SetAction(InputAction action);
    BRK_API void SetButton(InputMouseButton button);

    BRK_API Point2i GetPosition() const;
    BRK_API Size2i GetDelta() const;
    BRK_API InputModifiers GetModifiers() const;
    BRK_API InputAction GetAction() const;
    BRK_API InputMouseButton GetButton() const;

private:
    Point2i mPosition;
    Size2i mDelta;
    InputModifiers mModifiers;
    InputAction mAction = InputAction::Unknown;
    InputMouseButton mMouseButton = InputMouseButton::Unknown;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_EVENTMOUSE_HPP
