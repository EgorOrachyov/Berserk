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

#ifndef BERSERK_WINDOW_HPP
#define BERSERK_WINDOW_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Event.hpp>
#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Strings/StringName.hpp>
#include <BerserkCore/Image/Image.hpp>

namespace Berserk {

    /**
     * @brief Window
     *
     * Represents platform specific OS window, suitable for rendering.
     */
    class Window {
    public:

        struct Desc {
            StringName name;
            String title;
            Math::Size2i size;
            Image icon;
        };

        enum class EventType {
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
            CloseRequested
        };

        static const size_t EventTypeMax = 10;

        struct EventData {
            EventType eventType;
            Math::Size2i windowSize;
            Math::Size2i framebufferSize;
            Math::Point2i position;
            Math::Vec2f pixelRatio;
            bool focus;
        };

        virtual ~Window() = default;

        /** Close this window */
        virtual void Close() = 0;

        /** @return Current window position */
        virtual Math::Point2i GetPosition() const = 0;

        /** @return Window size */
        virtual Math::Size2i GetSize() const = 0;

        /** @return Window framebuffer size */
        virtual Math::Size2i GetFramebufferSize() const = 0;

        /** @return Ratio of pixels per window size units */
        virtual Math::Vec2f GetPixelRatio() const = 0;

        /** @return Aspect ratio width / height */
        virtual float GetAspectRatio() const { auto s = GetSize(); return s[1] > 0? (float)s[0] / (float)s[1]: 1.0f; }

        /** @return True if window receives focus */
        virtual bool IsInFocus() const = 0;

        /** @return True if window is closed */
        virtual bool IsClosed() const = 0;

        /** @return Window unique name for look-up */
        virtual StringName GetName() const = 0;

        /** @return Window title */
        virtual String GetTitle() const = 0;

        /** @return Native window hnd (might be null for some API backends) */
        virtual void* GetNativeHnd() const = 0;

        /** @return Event that allows to subscribe to window events */
        Event<const EventData&> OnWindowEvent;

    };

    template<>
    class TextPrint<Window::EventType> {
    public:
        template<typename Stream>
        void operator()(Stream &stream, Window::EventType eventType) {
            static const char *EventTypeText[] = {
                    BERSERK_TEXT("Resized"),
                    BERSERK_TEXT("FramebufferResized"),
                    BERSERK_TEXT("Moved"),
                    BERSERK_TEXT("FocusReceived"),
                    BERSERK_TEXT("FocusLost"),
                    BERSERK_TEXT("Minimized"),
                    BERSERK_TEXT("Maximized"),
                    BERSERK_TEXT("Restored"),
                    BERSERK_TEXT("PixelRatioChanged"),
                    BERSERK_TEXT("CloseRequested")
            };

            auto id = (size_t) eventType;
            if (id < Window::EventTypeMax) {
                stream.Add(EventTypeText[id]);
            } else {
                stream.Add(BERSERK_TEXT("Unknown"));
            }
        }
    };
}

#endif //BERSERK_WINDOW_HPP