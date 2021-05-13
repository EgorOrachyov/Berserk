/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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

        Math::Point2i GetPosition() const override;
        Math::Size2i GetSize() const override;
        Math::Size2i GetFramebufferSize() const override;
        Math::Vec2f GetPixelRatio() const override;

        bool IsInFocus() const override;
        bool IsClosed() const override;

        StringName GetName() const override;
        String GetTitle() const override;
        void *GetNativeHnd() const override;

        static Ref<GlfwWindow> Create(const Desc& desc);

    protected:
        void OnReleased() const override;

    private:
        friend class GlfwWindowManager;

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
        GLFWwindow* mHandle = nullptr;

        mutable SpinMutex mMutex;
    };
}

#endif //BERSERK_GLFWWINDOW_HPP