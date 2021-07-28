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

#ifndef BERSERK_GLFWWINDOWMANAGER_HPP
#define BERSERK_GLFWWINDOWMANAGER_HPP

#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Templates/Queue.hpp>
#include <BerserkPlatform/Glfw/GlfwWindow.hpp>

namespace Berserk {

    class GlfwWindowManager: public WindowManager {
    public:

        class GlfwImpl final: public WindowManager::Impl {
        public:
            explicit GlfwImpl(class GlfwContext& context, bool useVsync, bool noClientApi);
            ~GlfwImpl() override;

            SharedPtr<Window> CreateWindow(const Window::Desc &desc) override;
            SharedPtr<Window> GetWindowInFocus() override;
            SharedPtr<Window> GetWindowByName(const StringName &name) override;

            Backend GetBackendType() const override;
            void GetWindows(Array<SharedPtr<Window>> &windows) override;

        private:
            friend class GlfwContext;
            friend class GlfwWindow;

            void PreUpdate();
            void PostUpdate();
            void QueueWindowToRelease(GlfwWindow* window);
            void SetFocusWindow(const SharedPtr<GlfwWindow> &window, bool inFocus);
            void AdviseWindowNoClose(const SharedPtr<GlfwWindow> &window);
            void RequestContext(GLFWwindow* window);
            SharedPtr<Window> GetPrimaryWindow() const;
            SharedPtr<GlfwWindow> GetWindowByHandle(GLFWwindow* handle) const;

            // Glfw Specifics
            static void WindowCloseCallback(GLFWwindow* handle);
            static void WindowResizedCallback(GLFWwindow* handle, int32 width, int32 height);
            static void WindowContentScaleCallback(GLFWwindow* handle, float xscale, float yscale);
            static void FramebufferSizeCallback(GLFWwindow* handle, int32 width, int32 height);
            static void IconifyCallback(GLFWwindow* handle, int32 iconify);
            static void MaximizeCallback(GLFWwindow* handle, int32 maximize);
            static void PositionCallback(GLFWwindow* handle, int32 posX, int32 posY);
            static void FocusCallback(GLFWwindow* handle, int32 focus);

            /** Current window in the focus */
            SharedPtr<GlfwWindow> mWindowInFocus;
            /** Active windows of the application */
            Array<SharedPtr<GlfwWindow>> mWindows;
            /** Windows pending remove (nd hnd release) */
            Array<GlfwWindow*> mPendingRelease;
            /** Glfw context */
            class GlfwContext& mContext;
            /** True if we need enable vsync for windows swap */
            bool mUseVsync;
            /** No graphics context if we use vulkan backend */
            bool mNoClientApi;

            mutable RecursiveMutex mMutex;
        };

        /** @return Instance, required for windows safe release */
        static GlfwImpl& Get() {
            return (GlfwImpl&) GlfwImpl::Instance();
        }

    };
}

#endif //BERSERK_GLFWWINDOWMANAGER_HPP