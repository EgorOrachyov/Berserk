/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GlfwSystem/GlfwWindowManager.h>

namespace Berserk {

    GlfwWindowManager::GlfwWindowManager() : WindowManager() {
        // Set as singleton in WindowManager() constructor
    }

    GlfwWindowManager::~GlfwWindowManager() {
        // nothing
    }

    TPtrShared<GlfwWindow> GlfwWindowManager::createInternal(const CString &name, const CString &caption, Size2i size) {
        auto w = TPtrShared<GlfwWindow>::make(name, caption, size);
        auto hnd = w->getWindowHandleGLFW();

        glfwSetFramebufferSizeCallback(hnd, framebufferSizeCallback);
        glfwSetWindowIconifyCallback(hnd, iconifyCallback);
        glfwSetWindowPosCallback(hnd, positionCallback);
        glfwSetWindowFocusCallback(hnd, focusCallback);

        mWindows.add(w);
        return w;
    }
    
    TPtrShared<Window> GlfwWindowManager::create(const CString &name, const CString &caption, Size2i size) {
        return (TPtrShared<Window>) createInternal(name, caption, size);
    }

    TPtrShared<Window> GlfwWindowManager::find(const CString &name) {
        for (auto& w: mWindows) {
            if (w->getName() == name) {
                return (TPtrShared<Window>) w;
            }
        }

        return nullptr;
    }

    bool GlfwWindowManager::contains(const CString &name) const {
        for (auto& w: mWindows) {
            if (w->getName() == name) {
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

        for (auto& w: mWindows) {
            w->update();
        }
    }

    void GlfwWindowManager::finalize() {
        for (auto& w: mWindows) {
            w->close();
            w->markAsRemoved();
        }

        mWindows.clear();
        glfwTerminate();
    }

    TPtrShared<GlfwWindow> GlfwWindowManager::findWindowByHandle(GLFWwindow *hnd) {
        for (auto& w: mWindows) {
            if (w->getWindowHandleGLFW() == hnd)
                return w;
        }

        return nullptr;
    }

    void GlfwWindowManager::framebufferSizeCallback(GLFWwindow* handle, int32 width, int32 height) {
        auto& windows = getGlfwManager().getWindows();

        for (auto& w: windows) {
            if (w->getWindowHandleGLFW() == handle) {
                w->processResize(Size2i(width,height));
                return;
            }
        }
    }

    void GlfwWindowManager::iconifyCallback(GLFWwindow* handle, int iconify) {
        auto& windows = getGlfwManager().getWindows();

        for (auto& w: windows) {
            if (w->getWindowHandleGLFW() == handle) {
                w->processIconification((iconify? EWindowState::Minimised : EWindowState::Normal));
                return;
            }
        }
    }

    void GlfwWindowManager::positionCallback(GLFWwindow* handle, int posX, int posY) {
        auto& windows = getGlfwManager().getWindows();

        for (auto& w: windows) {
            if (w->getWindowHandleGLFW() == handle) {
                w->processMovement(Point2i(posX, posY));
                return;
            }
        }
    }

    void GlfwWindowManager::focusCallback(GLFWwindow *handle, int focus) {
        auto& windows = getGlfwManager().getWindows();

        for (auto& w: windows) {
            if (w->getWindowHandleGLFW() == handle) {
                w->processFocus(focus == GLFW_TRUE);
                return;
            }
        }
    }

    void GlfwWindowManager::errorCallback(int errorCode, const char *description) {
        getGlfwManager().getOnErrorFunc()(description);
    }

    GlfwWindowManager& GlfwWindowManager::getGlfwManager() {
        static auto manager = (GlfwWindowManager&)getSingleton();
        return manager;
    }

}