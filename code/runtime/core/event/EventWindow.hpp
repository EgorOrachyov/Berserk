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

#ifndef BERSERK_EVENTWINDOW_HPP
#define BERSERK_EVENTWINDOW_HPP

#include <core/Config.hpp>
#include <core/event/Event.hpp>
#include <core/math/TVecN.hpp>
#include <core/templates/Ref.hpp>
#include <platform/Window.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class EventWindow
 * @brief Event dispatched when window state changed
 */
class EventWindow final : public Event {
public:
    /** Type of window event */
    enum class Type {
        /** Triggered when window size changes */
        Resized = 0,
        /** Triggered when window framebuffer size changes */
        FramebufferResized,
        /** Triggered when window position changes */
        Moved,
        /** Triggered when window receives input focus */
        FocusReceived,
        /** Triggered when window loses input focus */
        FocusLost,
        /** Triggered when the window is minimized (iconified) */
        Minimized,
        /** Triggered when the window is expanded to cover the current screen */
        Maximized,
        /** Triggered when the window leaves minimized or maximized state */
        Restored,
        /** Triggered when the pixel ration of the window is changed (usually happens when the window is moved to another monitor) */
        PixelRatioChanged,
        /** Triggered when the user wants to close the window */
        CloseRequested,
        /** Unknown event */
        Unknown
    };

    BRK_API EventWindow() = default;
    BRK_API ~EventWindow() override = default;

    /** @copydoc Event::GetType() */
    BRK_API const EventType &GetEventType() const override;
    /** @copydoc Event::GetType() */
    BRK_API static const EventType &GetEventTypeStatic();

    BRK_API void SetWindow(Ref<Window> window);
    BRK_API void SetWindowSize(const Size2i &size);
    BRK_API void SetFramebufferSize(const Size2i &size);
    BRK_API void SetPosition(const Point2i &position);
    BRK_API void SetPixelRatio(const Vec2f &ratio);
    BRK_API void SetFocus(bool focus);
    BRK_API void SetType(Type type);

    BRK_API const Ref<Window> &GetWindow() const;
    BRK_API const Size2i &GetWindowSize() const;
    BRK_API const Size2i &GetFramebufferSize() const;
    BRK_API const Point2i &GetPosition() const;
    BRK_API const Vec2f &GetPixelRatio() const;
    BRK_API bool GetFocus() const;
    BRK_API Type GetType() const;

private:
    Ref<Window> mWindow;
    Size2i mWindowSize{};
    Size2i mFramebufferSize{};
    Point2i mPosition{};
    Vec2f mPixelRatio{};
    bool mFocus = false;
    Type mEventType = Type::Unknown;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_EVENTWINDOW_HPP
