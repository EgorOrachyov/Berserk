/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkGlfw/GlfwWindowManager.hpp>
#include <BerserkGlfw/GlfwContext.hpp>

namespace Berserk {
    namespace Platform {

        GlfwWindowManager::GlfwImpl::GlfwImpl(class GlfwContext &context)
            : mContext(context) {

            Provide(this);
        }

        GlfwWindowManager::GlfwImpl::~GlfwImpl() {
            mPendingRelease.Clear();
            for (auto& window: mWindows) {
                window->ReleaseNativeHandler();
            }

            Remove(this);
        }

        Ref<Window> GlfwWindowManager::GlfwImpl::CreateWindow(const Window::Desc &desc) {
            Guard<RecursiveMutex> guardGlfw(mContext.GetMutex());
            Guard<RecursiveMutex> guard(mMutex);

            auto window = GlfwWindow::Create(desc);
            auto handle = window->GetNativeHandle();

            glfwSetWindowCloseCallback(handle, WindowCloseCallback);
            glfwSetWindowSizeCallback(handle, WindowResizedCallback);
            glfwSetWindowContentScaleCallback(handle, WindowContentScaleCallback);
            glfwSetFramebufferSizeCallback(handle, FramebufferSizeCallback);
            glfwSetWindowIconifyCallback(handle, IconifyCallback);
            glfwSetWindowMaximizeCallback(handle, MaximizeCallback);
            glfwSetWindowPosCallback(handle, PositionCallback);
            glfwSetWindowFocusCallback(handle, FocusCallback);

            mWindows.Add(window);

            // todo: Now input for this window will be received by users
            // GlfwInput::SubscribeWindow(hnd);

            return (Ref<Window>) window;
        }

        Ref<Window> GlfwWindowManager::GlfwImpl::GetWindowInFocus() {
            Guard<RecursiveMutex> guard(mMutex);
            return (Ref<Window>) mWindowInFocus;
        }

        Ref<Window> GlfwWindowManager::GlfwImpl::GetWindowByName(const StringName &name) {
            Guard<RecursiveMutex> guard(mMutex);

            for (auto& window: mWindows) {
                if (window->GetName() == name)
                    return (Ref<Window>) window;
            }

            return Ref<Window>();
        }

        WindowManager::Backend GlfwWindowManager::GlfwImpl::GetBackendType() const {
            return Backend::Glfw;
        }

        void GlfwWindowManager::GlfwImpl::GetWindows(Array<Ref<Window>> &windows) {
            Guard<RecursiveMutex> guard(mMutex);

            windows.EnsureToAdd(mWindows.GetSize());
            for (auto& window: mWindows) {
                auto ptr = (Ref<Window>) window;
                windows.Move(ptr);
            }
        }

        void GlfwWindowManager::GlfwImpl::PreUpdate() {
            Guard<RecursiveMutex> guard(mMutex);

            Ref<GlfwWindow> window;
            while (mPendingRelease.Pop(window)) {
                window->ReleaseNativeHandler();
                mWindows.RemoveElement(window);
            }
        }

        void GlfwWindowManager::GlfwImpl::PostUpdate() {
            // Nothing
        }

        void GlfwWindowManager::GlfwImpl::QueueWindowToRelease(Ref<GlfwWindow> window) {
            Guard<RecursiveMutex> guard(mMutex);
            mPendingRelease.PushMove(window);
        }

        void GlfwWindowManager::GlfwImpl::SetFocusWindow(const Ref<GlfwWindow> &window, bool inFocus) {
            Guard<RecursiveMutex> guard(mMutex);

            auto old = mWindowInFocus;

            if (inFocus)
                mWindowInFocus = window;
            else if (mWindowInFocus == window)
                mWindowInFocus = nullptr;

#if 0
            if (old != mWindowInFocus)
                BERSERK_LOG_INFO(BERSERK_TEXT("Glfw"), BERSERK_TEXT("Focus window changed: old={0} new={1}"), old.GetPtrOrNull(), mWindowInFocus.GetPtrOrNull());
#endif
        }

        void GlfwWindowManager::GlfwImpl::AdviseWindowNoClose(const Ref<GlfwWindow> &window) {
            Guard<RecursiveMutex> guard(mMutex);

            GLFWwindow* handler = window->GetNativeHandle();
            glfwSetWindowShouldClose(handler, GLFW_FALSE);
        }

        Ref<GlfwWindow> GlfwWindowManager::GlfwImpl::GetWindowByHandle(GLFWwindow *handle) const {
            Guard<RecursiveMutex> guard(mMutex);

            for (auto& window: mWindows) {
                if (window->GetNativeHandle() == handle)
                    return window;
            }

            return Ref<GlfwWindow>();
        }

        void GlfwWindowManager::GlfwImpl::WindowCloseCallback(GLFWwindow *handle) {
            auto& manager = Get();
            auto window = manager.GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            manager.AdviseWindowNoClose(window);

            if (window) {
                window->OnClose();
            }
        }

        void GlfwWindowManager::GlfwImpl::WindowResizedCallback(GLFWwindow *handle, int32 width, int32 height) {
            auto window = Get().GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            if (window) {
                window->OnWindowResized(Math::Size2i(width, height));
            }
        }

        void GlfwWindowManager::GlfwImpl::WindowContentScaleCallback(GLFWwindow *handle, float xscale, float yscale) {
            auto window = Get().GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            if (window) {
                window->OnContentScaleChanged(Math::Vec2f(xscale, yscale));
            }
        }

        void GlfwWindowManager::GlfwImpl::FramebufferSizeCallback(GLFWwindow* handle, int32 width, int32 height) {
            auto window = Get().GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            if (window) {
                window->OnFramebufferSizeChanged(Math::Size2i(width, height));
            }
        }

        void GlfwWindowManager::GlfwImpl::IconifyCallback(GLFWwindow *handle, int32 iconify) {
            auto window = Get().GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            if (window) {
                window->OnMinimized(iconify == GLFW_TRUE);
            }
        }

        void GlfwWindowManager::GlfwImpl::MaximizeCallback(GLFWwindow *handle, int32 maximize) {
            auto window = Get().GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            if (window) {
                window->OnMaximized(maximize == GLFW_TRUE);
            }
        }

        void GlfwWindowManager::GlfwImpl::PositionCallback(GLFWwindow *handle, int32 posX, int32 posY) {
            auto window = Get().GetWindowByHandle(handle);
            BERSERK_ASSERT(window);

            if (window) {
                window->OnWindowMoved(Math::Point2i(posX, posY));
            }
        }

        void GlfwWindowManager::GlfwImpl::FocusCallback(GLFWwindow *handle, int32 focus) {
            auto& manager = Get();
            auto window = manager.GetWindowByHandle(handle);
            auto focusFlag = focus == GLFW_TRUE;
            BERSERK_ASSERT(window);

            manager.SetFocusWindow(window, focusFlag);

            if (window) {
                window->OnFocusChanged(focusFlag);
            }
        }
    }
}