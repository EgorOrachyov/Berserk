/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/GlfwWindowManager.hpp>
#include <BerserkCore/Logging.hpp>

namespace Berserk {

    GlfwWindowManager* GlfwWindowManager::gManager = nullptr;

    GlfwWindowManager::GlfwWindowManager() {
        if (gManager) {
            return;
        }

        gManager = this;
        glfwSetErrorCallback(ErrorCallback);
    }

    GlfwWindowManager::~GlfwWindowManager() {
        if (gManager == this) {
            gManager = nullptr;
        }
    }

    TPtrShared<Window> GlfwWindowManager::CreateWindow(WindowDesc windowDesc) {
        auto window = TPtrShared<GlfwWindow>::MakeFromArgs(this, std::move(windowDesc));
        auto hnd = window->GetHND();

        glfwSetWindowCloseCallback(hnd, WindowCloseCallback);
        glfwSetWindowSizeCallback(hnd, WindowResizedCallback);
        glfwSetWindowContentScaleCallback(hnd, WindowContentScaleCallback);
        glfwSetFramebufferSizeCallback(hnd, FramebufferSizeCallback);
        glfwSetWindowIconifyCallback(hnd, IconifyCallback);
        glfwSetWindowPosCallback(hnd, PositionCallback);
        glfwSetWindowFocusCallback(hnd, FocusCallback);

        mWindows.Add(window);

        // todo: Now input for this window will be received by users
        // GlfwInput::SubscribeWindow(hnd);

        return (TPtrShared<Window>) window;
    }

    TPtrShared<Window> GlfwWindowManager::GetWindow(const String &name) const {
        for (auto& window: mWindows) {
            if (window->GetName() == name)
                return (TPtrShared<Window>) window;
        }

        return nullptr;
    }

    TPtrShared<Window> GlfwWindowManager::GetWindowInFocus() const {
        return (TPtrShared<Window>) mWindowInFocus;
    }

    bool GlfwWindowManager::HasWindow(const String &name) const {
        for (auto& window: mWindows) {
            if (window->GetName() == name)
                return true;
        }

        return false;
    }

    void GlfwWindowManager::GetWindows(TArray<TPtrShared<Window>> &windows) const {
        windows.EnsureToAdd(mWindows.GetSize());
        for (auto& window: mWindows) {
            auto cast = (TPtrShared<Window>) window;
            windows.Emplace(std::move(cast));
        }
    }

    void GlfwWindowManager::WindowCloseCallback(GLFWwindow *hnd) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);

            if (window) {
                window->OnClose();
            }
        }
    }

    void GlfwWindowManager::WindowResizedCallback(GLFWwindow *hnd, int32 width, int32 height) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);

            if (window) {
                window->OnWindowResized(Size2i(width, height));
            }
        }
    }

    void GlfwWindowManager::WindowContentScaleCallback(GLFWwindow *hnd, float xscale, float yscale) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);

            if (window) {
                window->OnContentScaleChanged(Vec2f(xscale, yscale));
            }
        }
    }

    void GlfwWindowManager::FramebufferSizeCallback(GLFWwindow* hnd, int32 width, int32 height) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);

            if (window) {
                window->OnFramebufferSizeChanged(Size2i(width, height));
            }
        }
    }

    void GlfwWindowManager::IconifyCallback(GLFWwindow* hnd, int32 iconify) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);

            if (window) {
                window->OnMinimized(iconify);
            }
        }
    }

    void GlfwWindowManager::PositionCallback(GLFWwindow* hnd, int32 posX, int32 posY) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);

            if (window) {
                window->OnWindowMoved(Point2i(posX, posY));
            }
        }
    }

    void GlfwWindowManager::FocusCallback(GLFWwindow *hnd, int32 focus) {
        if (gManager) {
            auto window = gManager->GetWindowByHND(hnd);
            auto focusFlag = focus == GLFW_TRUE;

            if (focusFlag)
                gManager->mWindowInFocus = window;
            else if (gManager->mWindowInFocus == window)
                gManager->mWindowInFocus = nullptr;

            window->OnFocusChanged(focusFlag);
        }
    }

    void GlfwWindowManager::ErrorCallback(int32 errorCode, const char *description) {
        BERSERK_LOG_CAT_ERROR("Glfw", "Error description: ", description);
    }

    TPtrShared<GlfwWindow> GlfwWindowManager::GetWindowByHND(GLFWwindow *hnd) const {
        for (auto& window: mWindows) {
            if (window->GetHND() == hnd)
                return window;
        }

        return nullptr;
    }

}