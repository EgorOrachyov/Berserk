/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWWINDOW_H
#define BERSERK_GLFWWINDOW_H

#include <Platform/ISystem.h>
#include <GLFW/glfw3.h>
#include <Math/Vec2f.h>
#include <TArray.h>
#include <BuildOptions.h>

namespace Berserk {

    struct GlfwWindow {
        CString caption;
        GLFWwindow* handle;
        Size2i pos;
        Size2i size;
        float32 scaleX;
        float32 scaleY;
        bool resizeable;
        bool shouldClose;

        bool isMinimized;
        bool isRestored;
        bool isResized;
        bool isMoved;

        void reset() {
            isMinimized = false;
            isRestored = false;
            isMoved = false;
            isResized = false;
            shouldClose = glfwWindowShouldClose(handle);
        }

        void update() {
            glfwSwapBuffers(handle);
        }
    };

    class GlfwWindows {
    public:

        static void create(CString& caption, const ISystem::VideoMode& videoMode, const Vec2f& scale) {
            auto& window = mWindows.emplace();
            window.caption = std::move(caption);
            window.scaleX = scale[0];
            window.scaleY = scale[1];
            window.size[0] = (int32)((float32)videoMode.width / scale[0]);
            window.size[1] = (int32)((float32)videoMode.height / scale[1]);
            window.resizeable = videoMode.resizeable;

            auto w = window.size.x();
            auto h = window.size.y();
            auto handle = glfwCreateWindow(w, h, window.caption.data(), nullptr, nullptr);

            if (handle == nullptr)
                BERSERK_ERROR_RET("Failed to create GLFW window [%s]", window.caption.data());
            if (!window.resizeable)
                glfwSetWindowSizeLimits(handle, w, h, w, h);
            if (videoMode.maximised)
                glfwMaximizeWindow(handle);
            if (videoMode.forceVSync)

            glfwSetWindowIconifyCallback(handle, iconifyCallback);
            glfwSetWindowPosCallback(handle, positionCallback);
            glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
            glfwMakeContextCurrent(handle);

            window.handle = handle;
            window.reset();
        }

        static void destroy() {
            for (auto& w: mWindows) {
                glfwDestroyWindow(w.handle);
            }

            mWindows.clear();
        }

        static void reset() {
            for (auto& w: mWindows) {
                w.reset();
            }
        }

        static void update() {
            for (auto& w: mWindows) {
                w.update();
            }
        }

        static void framebufferSizeCallback(GLFWwindow* handle, int32 width, int32 height) {
            for (auto& w: mWindows) {
                if (w.handle == handle) {
                    w.size = Size2i(width,height);
                    w.isResized = true;
                    break;
                }
            }
        }

        static void iconifyCallback(GLFWwindow* handle, int iconify) {
            for (auto& w: mWindows) {
                if (w.handle == handle) {
                    if (iconify)
                        w.isMinimized = true;
                    else
                        w.isRestored = true;
                    break;
                }
            }
        }

        static void positionCallback(GLFWwindow* handle, int posX, int posY) {
            for (auto& w: mWindows) {
                if (w.handle == handle) {
                    w.pos[0] = (int32)(w.scaleX * posX);
                    w.pos[1] = (int32)(w.scaleY * posY);
                    w.isMoved = true;
                    break;
                }
            }
        }

        static GlfwWindow& getByHandle(GLFWwindow* handle) {
            for (auto& w: mWindows) {
                if (w.handle == handle) {
                    return w;
                }
            }

            BERSERK_ERROR_FAIL("No such window");
        }

        static GlfwWindow& get(uint32 index) {
            return mWindows[index];
        }

    private:
        /** All the application windows are stored as static container to access it in callbacks */
        static TArray<GlfwWindow> mWindows;
    };

    TArray<GlfwWindow> GlfwWindows::mWindows;
}

#endif //BERSERK_GLFWWINDOW_H