/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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