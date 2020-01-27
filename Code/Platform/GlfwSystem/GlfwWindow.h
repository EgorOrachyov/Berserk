/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWWINDOW_H
#define BERSERK_GLFWWINDOW_H

#include <Platform/System.h>
#include <GLFW/glfw3.h>
#include <TArray.h>
#include <BuildOptions.h>

namespace Berserk {
    struct GlfwWindow {
        CString caption;
        GLFWwindow* handle;
        Size2i pos;
        Size2i size;
        Size2i sizeFbo;
        float32 scaleX;
        float32 scaleY;
        bool maximised;
        bool resizeable;
        bool shouldClose;

        static void create(GlfwWindow& window) {
            auto w = window.size.x();
            auto h = window.size.y();
            auto handle = glfwCreateWindow(w, h, window.caption.data(), nullptr, nullptr);

            if (handle == nullptr) BERSERK_ERROR_RET("Failed to create GLFW window [%s]", window.caption.data());
            if (!window.resizeable) glfwSetWindowSizeLimits(handle, w, h, w, h);
            if (window.maximised) glfwMaximizeWindow(handle);

            window.handle = handle;
            update(window);
        }

        static void update(GlfwWindow& window) {
            glfwGetWindowPos(window.handle, &window.pos[0], &window.pos[1]);
            glfwGetWindowSize(window.handle, &window.pos[0], &window.pos[1]);
            glfwGetFramebufferSize(window.handle, &window.sizeFbo[0], &window.sizeFbo[1]);
            window.shouldClose = glfwWindowShouldClose(window.handle);
        }
    };
}

#endif //BERSERK_GLFWWINDOW_H