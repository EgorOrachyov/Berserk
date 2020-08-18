/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWWINDOW_H
#define BERSERK_GLFWWINDOW_H

#include <Platform/Window.h>
#include <GLFW/glfw3.h>

namespace Berserk {

    class GlfwWindow final: public Window {
    public:
        GlfwWindow(const CString& name, const CString& caption, Size2i size, EWindowActionOnClose actionOnClose = EWindowActionOnClose::Nothing);
        ~GlfwWindow() override;

        void makeRenderContextCurrent() const override;
        void setLimits(Size2i min, Size2i max) override;
        void requestFocus() override;
        void requestClose() override;
        bool shouldClose() const override;
        bool isFocused() const override;

        void processResize(Size2i newSize);
        void processMovement(Point2i newPos);
        void processIconification(EWindowState newState);
        void processFocus(bool focus);
        void update();
        void close();
        void removeMyselfFromWindowManager();
        void markAsRemoved() { mInManager = false; }

        float getScaleX() const { return mScaleX; }
        float getScaleY() const { return mScaleY; }

        GLFWmonitor* getMonitorGLFW() const { return mMonitor; }
        GLFWwindow* getWindowHandleGLFW() const { return mWindowHandle; }

    private:

        GLFWwindow* mWindowHandle = nullptr;
        GLFWmonitor* mMonitor = nullptr;
        float mScaleX = 1.0f;
        float mScaleY = 1.0f;
        bool mShouldClose = false;
        bool mInManager = true;
        bool mIsFocused = false;
    };

}

#endif //BERSERK_GLFWWINDOW_H