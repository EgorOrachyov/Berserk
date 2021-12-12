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

#include <core/Engine.hpp>
#include <core/io/Logger.hpp>
#include <platform/glfw/GlfwWindow.hpp>
#include <platform/glfw/GlfwWindowManager.hpp>

#include <cstdlib>

BRK_NS_BEGIN

GlfwWindowManager::GlfwWindowManager(bool vsync, bool clientApi) {
    mVsync = vsync;
    mClientApi = clientApi;

    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit()) {
        BRK_ERROR("Failed to initialize glfw");
        std::abort();
    }

    if (!clientApi && !glfwVulkanSupported()) {
        BRK_ERROR("Vulkan is not supported");
        std::abort();
    }

#if defined(BERSERK_TARGET_MACOS)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          // Required on Mac
#elif defined(BERSERK_TARGET_LINUX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#elif defined(BERSERK_TARGET_WINDOWS)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (!clientApi) {
        // For Vulkan based renderer
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
}

GlfwWindowManager::~GlfwWindowManager() {
#ifdef BERSERK_DEBUG
    for (const auto &entry : mWindows)
        BRK_INFO("Release window name=" << entry.first);
#endif//BERSERK_DEBUG
}

Ref<Window> GlfwWindowManager::CreateWindow(const StringName &name, const Size2i &size, const String &title) {
    if (mWindows.find(name) != mWindows.end()) {
        BRK_ERROR("An attempt to create new window with the same name=" << name << ". Choose another id");
        return Ref<Window>{};
    }

    // Create and register
    Ref<GlfwWindow> window{new GlfwWindow(name, size, title, mClientApi)};
    mWindows.emplace(name, window.As<Window>());
    mWindowsByHND.emplace(window->mHND, window.As<Window>());

    // Subscribe notification callbacks
    glfwSetWindowCloseCallback(window->mHND, WindowCloseCallback);
    glfwSetWindowSizeCallback(window->mHND, WindowResizedCallback);
    glfwSetWindowContentScaleCallback(window->mHND, WindowContentScaleCallback);
    glfwSetFramebufferSizeCallback(window->mHND, FramebufferSizeCallback);
    glfwSetWindowIconifyCallback(window->mHND, IconifyCallback);
    glfwSetWindowMaximizeCallback(window->mHND, MaximizeCallback);
    glfwSetWindowPosCallback(window->mHND, PositionCallback);
    glfwSetWindowFocusCallback(window->mHND, FocusCallback);

    // If first, make primary
    if (mPrimaryWindow.IsNull())
        mPrimaryWindow = window.As<Window>();

    if (mClientApi) {
        // First of all, make context current to enable following GL code in renderer
        glfwMakeContextCurrent(window->mHND);

        // Set interval for swap in glfw
        // https://www.glfw.org/docs/3.3/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
        int vsync = mVsync ? 1 : 0;
        glfwSwapInterval(vsync);
    }

    return window.As<Window>();
}

Ref<Window> GlfwWindowManager::GetPrimaryWindow() {
    return mPrimaryWindow;
}

void GlfwWindowManager::PollEvents() {
    glfwPollEvents();
}

void GlfwWindowManager::DispatchEvent(const Ref<Window> &window, EventWindow::Type type) {
    auto &engine = Engine::Instance();
    auto &dispatcher = engine.GetEventDispatcher();
    auto glfwWindow = dynamic_cast<const GlfwWindow *>(window.Get());

    Ref<EventWindow> event{new EventWindow()};
    event->SetWindow(window);
    event->SetWindowSize(glfwWindow->mSize);
    event->SetFramebufferSize(glfwWindow->mFramebufferSize);
    event->SetPosition(glfwWindow->mPosition);
    event->SetPixelRatio(glfwWindow->mPixelRatio);
    event->SetFocus(glfwWindow->mInFocus);
    event->SetType(type);

    dispatcher.Dispatch(event.As<Event>());
}

Ref<Window> GlfwWindowManager::GetWindow(GLFWwindow *HND) {
    auto query = mWindowsByHND.find(HND);
    return query != mWindowsByHND.end() ? query->second : Ref<Window>{};
}

void GlfwWindowManager::WindowCloseCallback(GLFWwindow *HND) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<const GlfwWindow *>(window.Get());

    assert(glfwWindow);
    windowManager.DispatchEvent(window, EventWindow::Type::CloseRequested);
}

void GlfwWindowManager::WindowResizedCallback(GLFWwindow *HND, int32 width, int32 height) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    glfwWindow->mSize = Size2i(width, height);
    windowManager.DispatchEvent(window, EventWindow::Type::Resized);
}

void GlfwWindowManager::WindowContentScaleCallback(GLFWwindow *HND, float xScale, float yScale) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    glfwWindow->mPixelRatio = Vec2f(xScale, yScale);
    windowManager.DispatchEvent(window, EventWindow::Type::PixelRatioChanged);
}

void GlfwWindowManager::FramebufferSizeCallback(GLFWwindow *HND, int32 width, int32 height) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    glfwWindow->mFramebufferSize = Size2i(width, height);
    windowManager.DispatchEvent(window, EventWindow::Type::FramebufferResized);
}

void GlfwWindowManager::IconifyCallback(GLFWwindow *HND, int32 iconify) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    windowManager.DispatchEvent(window, iconify ? EventWindow::Type::Minimized : EventWindow::Type::Restored);
}

void GlfwWindowManager::MaximizeCallback(GLFWwindow *HND, int32 maximize) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    windowManager.DispatchEvent(window, maximize ? EventWindow::Type::Maximized : EventWindow::Type::Restored);
}

void GlfwWindowManager::PositionCallback(GLFWwindow *HND, int32 posX, int32 posY) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    glfwWindow->mPosition = Point2i(posX, posY);
    windowManager.DispatchEvent(window, EventWindow::Type::Moved);
}

void GlfwWindowManager::FocusCallback(GLFWwindow *HND, int32 focus) {
    auto &engine = Engine::Instance();
    auto &windowManager = *dynamic_cast<GlfwWindowManager *>(&engine.GetWindowManager());
    auto window = windowManager.GetWindow(HND);
    auto glfwWindow = dynamic_cast<GlfwWindow *>(window.Get());

    assert(glfwWindow);
    glfwWindow->mInFocus = focus;
    windowManager.DispatchEvent(window, focus ? EventWindow::Type::FocusReceived : EventWindow::Type::FocusLost);
}

void GlfwWindowManager::ErrorCallback(int32 errorCode, const char *description) {
    BRK_ERROR("Glfw error: code=" << errorCode << " desc=\"" << description << "\"");
}

BRK_NS_END