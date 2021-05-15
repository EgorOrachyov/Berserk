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

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/Function.hpp>

namespace Berserk {

    enum class EWindowVisibility {
        Windowed,
        Minimized
    };

    enum class EWindowEventType {
        Resize = 0,
        Move,
        Visibility,
        Framebuffer,
        Focus,
        PixelRatio,
        Close,
        // Do not touch
        MarkerCount,
        MarkerUnknown
    };

    struct WindowEvent {
        EWindowEventType eventType;
        EWindowVisibility visibility;
        Size2i size;
        Size2i framebufferSize;
        Point2i position;
        Vec2f pixelRatio;
        bool focus;
    };

    class WindowEventListener {
    public:
        virtual ~WindowEventListener() = default;
        virtual void OnWindowEvent(const WindowEvent& event) = 0;
    };

    class Window {
    public:
        virtual ~Window() = default;
        virtual void RequestFocus() = 0;
        virtual void AddListener(WindowEventListener& listener) = 0;
        virtual void RemoveListener(WindowEventListener& listener) = 0;
        virtual EWindowVisibility GetVisibility() const = 0;
        virtual Point2i GetPosition() const = 0;
        virtual Size2i GetSize() const = 0;
        virtual Vec2f GetPixelRatio() const = 0;
        virtual bool IsInFocus() const = 0;
        virtual const String& GetName() const = 0;
        virtual const String& GetTitle() const = 0;
    };

}

#endif //BERSERK_WINDOW_HPP