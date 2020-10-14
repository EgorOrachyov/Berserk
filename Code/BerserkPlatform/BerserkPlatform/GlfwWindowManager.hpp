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
#include <BerserkPlatform/GlfwWindow.hpp>

namespace Berserk {

    class GlfwWindowManager final: public WindowManager {
    public:
        GlfwWindowManager();
        ~GlfwWindowManager() override;

        TPtrShared<Window> CreateWindow(WindowDesc windowDesc) override;
        TPtrShared<Window> GetWindow(const String &name) const override;
        TPtrShared<Window> GetWindowInFocus() const override;

        bool HasWindow(const String &name) const override;
        void GetWindows(TArray<TPtrShared<Window>> &windows) const override;

        // Glfw Specifics
        static void WindowCloseCallback(GLFWwindow* hnd);
        static void WindowResizedCallback(GLFWwindow* hnd, int32 width, int32 height);
        static void WindowContentScaleCallback(GLFWwindow* hnd, float xscale, float yscale);
        static void FramebufferSizeCallback(GLFWwindow* hnd, int32 width, int32 height);
        static void IconifyCallback(GLFWwindow* hnd, int32 iconify);
        static void PositionCallback(GLFWwindow* hnd, int32 posX, int32 posY);
        static void FocusCallback(GLFWwindow* hnd, int32 focus);
        static void ErrorCallback(int32 errorCode, const char *description);

        TPtrShared<GlfwWindow> GetWindowByHND(GLFWwindow* hnd) const;

    private:
        TPtrShared<GlfwWindow> mWindowInFocus;
        TArray<TPtrShared<GlfwWindow>> mWindows;

        static GlfwWindowManager* gManager;
    };

}

#endif //BERSERK_GLFWWINDOWMANAGER_HPP