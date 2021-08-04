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

#include <BerserkPlatform/Glfw/GlfwWindowManager.hpp>
#include <BerserkPlatform/Glfw/GlfwContext.hpp>

namespace Berserk {

    GlfwWindowManager::GlfwImpl::GlfwImpl(class GlfwContext &context, bool useVsync, bool noClientApi)
        : mContext(context), mUseVsync(useVsync), mNoClientApi(noClientApi) {

        Provide(this);
    }

    GlfwWindowManager::GlfwImpl::~GlfwImpl() {
        mPendingRelease.Clear();

        for (auto& window: mWindows) {
            window->ReleaseNativeHandler();
        }

        Remove(this);
    }

    SharedPtr<Window> GlfwWindowManager::GlfwImpl::CreateWindow(const Window::Desc &desc) {
        Guard<RecursiveMutex> guardGlfw(mContext.GetMutex());
        Guard<RecursiveMutex> guard(mMutex);

        auto window = SharedPtr<GlfwWindow>::Make(desc);
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

        if (!mNoClientApi) {
            // First of all, make context current to enable following GL code in renderer
            glfwMakeContextCurrent(window->mHandle);

            // Set interval for swap in glfw
            // https://www.glfw.org/docs/3.3/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
            int vsync = mUseVsync? 1: 0;
            glfwSwapInterval(vsync);
        }

        // todo: Now input for this window will be received by users
        // GlfwInput::SubscribeWindow(hnd);

        return (SharedPtr<Window>) window;
    }

    SharedPtr<Window> GlfwWindowManager::GlfwImpl::GetWindowInFocus() {
        Guard<RecursiveMutex> guard(mMutex);
        return (SharedPtr<Window>) mWindowInFocus;
    }

    SharedPtr<Window> GlfwWindowManager::GlfwImpl::GetWindowByName(const StringName &name) {
        Guard<RecursiveMutex> guard(mMutex);

        for (auto& window: mWindows) {
            if (window->GetName() == name)
                return (SharedPtr<Window>) window;
        }

        return {};
    }

    WindowManager::Backend GlfwWindowManager::GlfwImpl::GetBackendType() const {
        return Backend::Glfw;
    }

    void GlfwWindowManager::GlfwImpl::GetWindows(Array<SharedPtr<Window>> &windows) {
        Guard<RecursiveMutex> guard(mMutex);

        windows.EnsureToAdd(mWindows.GetSize());
        for (auto& window: mWindows) {
            auto ptr = (SharedPtr<Window>) window;
            windows.Move(std::move(ptr));
        }
    }

    void GlfwWindowManager::GlfwImpl::PreUpdate() {
        Guard<RecursiveMutex> guard(mMutex);

        auto iter = mPendingRelease.begin();
        while (iter != mPendingRelease.end()) {
            GlfwWindow* window = *iter;

            if (window->CanReleaseNativeHandle()) {
                window->ReleaseNativeHandler();

                uint64 index;
                mWindows.FindFirst(index, [window](const SharedPtr<GlfwWindow>& w){
                   return w == window;
                });

                mWindows.Remove(index);
                iter = mPendingRelease.Remove(iter);
            }
            else
                ++iter;
        }
    }

    void GlfwWindowManager::GlfwImpl::PostUpdate() {
        if (!mNoClientApi) {
            for (auto& window: mWindows) {
                // todo: check, if swap was requested
                if (window->CanSwapBuffers())
                    window->SwapBuffers();
            }
        }
    }

    void GlfwWindowManager::GlfwImpl::QueueWindowToRelease(GlfwWindow* window) {
        Guard<RecursiveMutex> guard(mMutex);
        mPendingRelease.Add(window);
    }

    void GlfwWindowManager::GlfwImpl::SetFocusWindow(const SharedPtr<GlfwWindow> &window, bool inFocus) {
        Guard<RecursiveMutex> guard(mMutex);

        if (inFocus)
            mWindowInFocus = window;
        else if (mWindowInFocus == window)
            mWindowInFocus = {};
    }

    void GlfwWindowManager::GlfwImpl::AdviseWindowNoClose(const SharedPtr<GlfwWindow> &window) {
        Guard<RecursiveMutex> guard(mMutex);

        GLFWwindow* handler = window->GetNativeHandle();
        glfwSetWindowShouldClose(handler, GLFW_FALSE);
    }

    void GlfwWindowManager::GlfwImpl::RequestContext(GLFWwindow *window) {
        Guard<RecursiveMutex> guardGlfw(mContext.GetMutex());
        Guard<RecursiveMutex> guard(mMutex);

        if (!mNoClientApi) {
            // Only if context is managed by glfw (for OpenGL-based renderer)
            glfwMakeContextCurrent(window);
        }
    }

    SharedPtr<Window> GlfwWindowManager::GlfwImpl::GetPrimaryWindow() const {
        Guard<RecursiveMutex> guard(mMutex);

        if (mWindows.IsNotEmpty()) {
            return (SharedPtr<Window>) mWindows[0];
        }

        return nullptr;
    }

    SharedPtr<GlfwWindow> GlfwWindowManager::GlfwImpl::GetWindowByHandle(GLFWwindow *handle) const {
        Guard<RecursiveMutex> guard(mMutex);

        for (auto& window: mWindows) {
            if (window->GetNativeHandle() == handle)
                return window;
        }

        return nullptr;
    }

    void GlfwWindowManager::GlfwImpl::WindowCloseCallback(GLFWwindow *handle) {
        auto& manager = Get();
        auto window = manager.GetWindowByHandle(handle);
        assert(window);

        manager.AdviseWindowNoClose(window);

        if (window) {
            window->OnClose();
        }
    }

    void GlfwWindowManager::GlfwImpl::WindowResizedCallback(GLFWwindow *handle, int32 width, int32 height) {
        auto window = Get().GetWindowByHandle(handle);
        assert(window);

        if (window) {
            window->OnWindowResized(Math::Size2i(width, height));
        }
    }

    void GlfwWindowManager::GlfwImpl::WindowContentScaleCallback(GLFWwindow *handle, float xscale, float yscale) {
        auto window = Get().GetWindowByHandle(handle);
        assert(window);

        if (window) {
            window->OnContentScaleChanged(Math::Vec2f(xscale, yscale));
        }
    }

    void GlfwWindowManager::GlfwImpl::FramebufferSizeCallback(GLFWwindow* handle, int32 width, int32 height) {
        auto window = Get().GetWindowByHandle(handle);
        assert(window);

        if (window) {
            window->OnFramebufferSizeChanged(Math::Size2i(width, height));
        }
    }

    void GlfwWindowManager::GlfwImpl::IconifyCallback(GLFWwindow *handle, int32 iconify) {
        auto window = Get().GetWindowByHandle(handle);
        assert(window);

        if (window) {
            window->OnMinimized(iconify == GLFW_TRUE);
        }
    }

    void GlfwWindowManager::GlfwImpl::MaximizeCallback(GLFWwindow *handle, int32 maximize) {
        auto window = Get().GetWindowByHandle(handle);
        assert(window);

        if (window) {
            window->OnMaximized(maximize == GLFW_TRUE);
        }
    }

    void GlfwWindowManager::GlfwImpl::PositionCallback(GLFWwindow *handle, int32 posX, int32 posY) {
        auto window = Get().GetWindowByHandle(handle);
        assert(window);

        if (window) {
            window->OnWindowMoved(Math::Point2i(posX, posY));
        }
    }

    void GlfwWindowManager::GlfwImpl::FocusCallback(GLFWwindow *handle, int32 focus) {
        auto& manager = Get();
        auto window = manager.GetWindowByHandle(handle);
        auto focusFlag = focus == GLFW_TRUE;
        assert(window);

        manager.SetFocusWindow(window, focusFlag);

        if (window) {
            window->OnFocusChanged(focusFlag);
        }
    }
}