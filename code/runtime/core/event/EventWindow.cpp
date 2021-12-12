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

#include <core/event/EventWindow.hpp>

#include <cassert>

BRK_NS_BEGIN

const EventType &EventWindow::GetEventType() const {
    return GetEventTypeStatic();
}

const EventType &EventWindow::GetEventTypeStatic() {
    static StringName eventType(BRK_TEXT("_brk_core_event_window"));
    return eventType;
}

void EventWindow::SetWindow(Ref<Window> window) {
    assert(window.IsNotNull());
    mWindow = std::move(window);
}

void EventWindow::SetWindowSize(const Size2i &size) {
    mWindowSize = size;
}

void EventWindow::SetFramebufferSize(const Size2i &size) {
    mFramebufferSize = size;
}

void EventWindow::SetPosition(const Point2i &position) {
    mPosition = position;
}

void EventWindow::SetPixelRatio(const Vec2f &ratio) {
    mPixelRatio = ratio;
}

void EventWindow::SetFocus(bool focus) {
    mFocus = focus;
}

void EventWindow::SetType(Type type) {
    mEventType = type;
}

const Ref<Window> &EventWindow::GetWindow() const {
    return mWindow;
}

const Size2i &EventWindow::GetWindowSize() const {
    return mWindowSize;
}

const Size2i &EventWindow::GetFramebufferSize() const {
    return mFramebufferSize;
}

const Point2i &EventWindow::GetPosition() const {
    return mPosition;
}

const Vec2f &EventWindow::GetPixelRatio() const {
    return mPixelRatio;
}

bool EventWindow::GetFocus() const {
    return mFocus;
}

EventWindow::Type EventWindow::GetType() const {
    return mEventType;
}

BRK_NS_END