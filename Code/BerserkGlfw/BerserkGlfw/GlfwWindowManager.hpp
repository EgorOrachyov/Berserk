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
#include <BerserkCore/Templates/Queue.hpp>
#include <BerserkGlfw/GlfwWindow.hpp>

namespace Berserk {

    class GlfwWindowManager: public WindowManager {
    public:

        class GlfwImpl final: public WindowManager::Impl {
        public:
            explicit GlfwImpl(class GlfwContext& context);
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
            void QueueWindowToRelease(GLFWwindow* window);
            void SetFocusWindow(const SharedPtr<GlfwWindow> &window, bool inFocus);
            void AdviseWindowNoClose(const SharedPtr<GlfwWindow> &window);
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
            Queue<GLFWwindow*> mPendingRelease;
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

#endif //BERSERK_GLFWWINDOWMANAGER_HPP