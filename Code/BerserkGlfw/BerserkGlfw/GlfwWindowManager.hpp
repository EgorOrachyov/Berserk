/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLFWWINDOWMANAGER_HPP
#define BERSERK_GLFWWINDOWMANAGER_HPP

#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Containers/Queue.hpp>
#include <BerserkGlfw/GlfwWindow.hpp>

namespace Berserk {
    namespace Platform {

        class GlfwWindowManager: public WindowManager {
        public:

            class GlfwImpl final: public WindowManager::Impl {
            public:
                explicit GlfwImpl(class GlfwContext& context);
                ~GlfwImpl() override;

                Ref<Window> CreateWindow(const Window::Desc &desc) override;
                Ref<Window> GetWindowInFocus() override;
                Ref<Window> GetWindowByName(const String &name) override;

                void GetWindows(Array<Ref<Window>> &windows) override;

            private:
                friend class GlfwContext;
                friend class GlfwWindow;

                void Update();
                void QueueWindowToRelease(Ref<GlfwWindow> window);
                void SetFocusWindow(const Ref<GlfwWindow> &window, bool inFocus);
                Ref<GlfwWindow> GetWindowByHandle(GLFWwindow* handle) const;

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
                Ref<GlfwWindow> mWindowInFocus;
                /** Active windows of the application */
                Array<Ref<GlfwWindow>> mWindows;
                /** Windows pending remove (nd hnd release) */
                Queue<Ref<GlfwWindow>> mPendingRelease;
                /** Glfw context */
                class GlfwContext& mContext;

                mutable RecursiveMutex mMutex;
            };

            /** @return Instance, required for windows safe release */
            static GlfwImpl& Get() {
                return (GlfwImpl&) GlfwImpl::Instance();
            }

        };

    }
}

#endif //BERSERK_GLFWWINDOWMANAGER_HPP