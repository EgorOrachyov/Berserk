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

#ifndef BERSERK_GLFWWINDOW_HPP
#define BERSERK_GLFWWINDOW_HPP

#include <BerserkCore/Platform/Window.hpp>
#include <GLFW/glfw3.h>

namespace Berserk {

    class GlfwWindow final: public Window {
    public:
        using Window::OnWindowEvent;
        using Window::EventData;
        using Window::EventType;
        using Window::Desc;

        explicit GlfwWindow(const Desc& desc);
        ~GlfwWindow() override;

        void Close() override;
        void MakeContextCurrent() override;

        Math::Point2i GetPosition() const override;
        Math::Size2i GetSize() const override;
        Math::Size2i GetFramebufferSize() const override;
        Math::Vec2f GetPixelRatio() const override;

        bool IsInFocus() const override;
        bool IsClosed() const override;

        StringName GetName() const override;
        String GetTitle() const override;
        void *GetNativeHnd() const override;
        void AddUnsafeUsage() override;
        void ReleaseUnsafeUsage() override;

    private:
        friend class GlfwWindowManager;

        bool CanReleaseNativeHandle() const;
        bool CanSwapBuffers() const;
        void SwapBuffers();
        void ReleaseNativeHandler();
        GLFWwindow* GetNativeHandle() const;

        void OnFramebufferSizeChanged(const Math::Size2i &newFramebufferSize);
        void OnWindowMoved(const Math::Point2i &newPosition);
        void OnWindowResized(const Math::Size2i &newSize);
        void OnContentScaleChanged(const Math::Vec2f &newScale);
        void OnFocusChanged(bool isInFocus);
        void OnMinimized(bool minimized);
        void OnMaximized(bool maximized);
        void OnClose();

        EventData GetEventBaseData(EventType eventType) const;

    private:
        EventPublisher<const EventData&> mEvent;
        StringName mName;
        String mTitle;
        Math::Point2i mPosition;
        Math::Size2i mSize;
        Math::Size2i mFramebufferSize;
        Math::Vec2f mPixelRatio;
        bool mIsInFocus = false;
        bool mIsPendingRelease = false;
        bool mSwapBuffers = true;
        GLFWwindow* mHandle = nullptr;
        AtomicInt32 mUnsafeUsage{0};

        mutable SpinMutex mMutex;
    };
}

#endif //BERSERK_GLFWWINDOW_HPP