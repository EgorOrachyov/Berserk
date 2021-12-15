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

#include <core/event/EventWindow.hpp>
#include <platform/WindowManager.hpp>

#include <memory>
#include <unordered_map>

#include <GLFW/glfw3.h>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class GlfwWindowManager
 * @brief Glfw window manager implementation
 */
class GlfwWindowManager final : public WindowManager {
public:
    BRK_API GlfwWindowManager(bool vsync, bool clientApi);
    BRK_API ~GlfwWindowManager() override;
    BRK_API Ref<Window> CreateWindow(const StringName &name, const Size2i &size, const String &title) override;
    BRK_API Ref<Window> GetPrimaryWindow() override;

private:
    friend class Application;
    BRK_API void PollEvents();
    BRK_API void DispatchEvent(const Ref<Window> &window, EventWindow::Type type);
    BRK_API Ref<Window> GetWindow(GLFWwindow *HND);

private:
    // Glfw Specifics
    static void WindowCloseCallback(GLFWwindow *HND);
    static void WindowResizedCallback(GLFWwindow *HND, int32 width, int32 height);
    static void WindowContentScaleCallback(GLFWwindow *HND, float xScale, float yScale);
    static void FramebufferSizeCallback(GLFWwindow *HND, int32 width, int32 height);
    static void IconifyCallback(GLFWwindow *HND, int32 iconify);
    static void MaximizeCallback(GLFWwindow *HND, int32 maximize);
    static void PositionCallback(GLFWwindow *HND, int32 posX, int32 posY);
    static void FocusCallback(GLFWwindow *HND, int32 focus);
    static void ErrorCallback(int32 errorCode, const char *description);

private:
    /** Input manager */
    std::shared_ptr<class GlfwInput> mInput;
    /** Primary window of application */
    Ref<Window> mPrimaryWindow;
    /** All application windows */
    std::unordered_map<StringName, Ref<Window>> mWindows;
    /** Aux hnd map */
    std::unordered_map<GLFWwindow *, Ref<Window>> mWindowsByHND;
    /** Use client-api (gl); otherwise no - for vulkan and directx */
    bool mClientApi = true;
    bool mVsync = true;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLFWWINDOWMANAGER_HPP
