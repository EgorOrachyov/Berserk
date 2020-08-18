/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GlfwSystem/GlfwWindowManager.h>
#include <GlfwSystem/GlfwInput.h>

namespace Berserk {

    GlfwWindowManager::GlfwWindowManager() : WindowManager() {
        // Set as singleton in WindowManager() constructor
    }

    GlfwWindowManager::~GlfwWindowManager() {
        // nothing
    }

    TPtrShared<GlfwWindow> GlfwWindowManager::createInternal(const CString &name, const CString &caption, Size2i size, EWindowActionOnClose action) {
        auto window = TPtrShared<GlfwWindow>::make(name, caption, size, action);
        auto hnd = window->getWindowHandleGLFW();

        glfwSetFramebufferSizeCallback(hnd, framebufferSizeCallback);
        glfwSetWindowIconifyCallback(hnd, iconifyCallback);
        glfwSetWindowPosCallback(hnd, positionCallback);
        glfwSetWindowFocusCallback(hnd, focusCallback);

        mWindows.add(window);

        // Now input for this window will be received by users
        GlfwInput::getGlfwInput().subscribeWindow(hnd);

        return window;
    }
    
    TPtrShared<Window> GlfwWindowManager::create(const CString &name, const CString &caption, Size2i size) {
        return createInternal(name, caption, size, EWindowActionOnClose::Nothing).castTo<Window>();
    }

    TPtrShared<Window> GlfwWindowManager::find(const CString &name) {
        for (auto& window: mWindows) {
            if (window->getName() == name) {
                return window.castTo<Window>();
            }
        }

        return nullptr;
    }

    TPtrShared<Window> GlfwWindowManager::getFocusedWindow() {
        for (auto& window: mWindows) {
            if (window->isFocused()) {
                return window.castTo<Window>();
            }
        }

        return nullptr;
    }

    bool GlfwWindowManager::contains(const CString &name) const {
        for (auto& window: mWindows) {
            if (window->getName() == name) {
                return true;
            }
        }

        return false;
    }

    void GlfwWindowManager::setErrorFunction(Function<void(const char *)> function) {
        mOnError = std::move(function);
        glfwSetErrorCallback(errorCallback);
    }

    void GlfwWindowManager::initializeForMacOS() {
        auto result = glfwInit();
        BERSERK_COND_ERROR_FAIL(result, "Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    }

    void GlfwWindowManager::forceVSync() {
        glfwSwapInterval(1);
    }

    void GlfwWindowManager::pollEvents() {
        glfwPollEvents();

        for (auto& window: mWindows) {
            window->update();
        }
    }

    void GlfwWindowManager::finalize() {
        for (auto& window: mWindows) {
            window->close();
            window->markAsRemoved();
        }

        mWindows.clear();
        glfwTerminate();
    }

    TPtrShared<GlfwWindow> GlfwWindowManager::findWindowByHandle(GLFWwindow *hnd) {
        for (auto& window: mWindows) {
            if (window->getWindowHandleGLFW() == hnd)
                return window;
        }

        return nullptr;
    }

    void GlfwWindowManager::framebufferSizeCallback(GLFWwindow* hnd, int32 width, int32 height) {
        auto window = getGlfwManager().findWindowByHandle(hnd);

        if (window.isNotNull()) {
            window->processResize(Size2i(width,height));
        }
    }

    void GlfwWindowManager::iconifyCallback(GLFWwindow* hnd, int iconify) {
        auto window = getGlfwManager().findWindowByHandle(hnd);

        if (window.isNotNull()) {
            window->processIconification((iconify? EWindowState::Minimised : EWindowState::Normal));
        }
    }

    void GlfwWindowManager::positionCallback(GLFWwindow* hnd, int posX, int posY) {
        auto window = getGlfwManager().findWindowByHandle(hnd);

        if (window.isNotNull()) {
            window->processMovement(Point2i(posX, posY));
        }
    }

    void GlfwWindowManager::focusCallback(GLFWwindow *hnd, int focus) {
        auto window = getGlfwManager().findWindowByHandle(hnd);

        if (window.isNotNull()) {
            window->processFocus(focus == GLFW_TRUE);
        }
    }

    void GlfwWindowManager::errorCallback(int errorCode, const char *description) {
        getGlfwManager().getOnErrorFunc()(description);
    }

    GlfwWindowManager& GlfwWindowManager::getGlfwManager() {
        static auto& manager = (GlfwWindowManager&)getSingleton();
        return manager;
    }

}