/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <TestFixture.hpp>
#include <BerserkCore/Logging.hpp>
#include <BerserkPlatform/GlfwWindow.hpp>
#include <BerserkPlatform/GlfwWindowManager.hpp>

#include <GLFW/glfw3.h>

TEST_F(TestFixture,Glfw) {
    glfwInit();
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const char* name = glfwGetMonitorName(monitor);
    printf("Monitor name: %s\n", name);

    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    printf("Monitor Content Scale: (x: %f, y: %f)\n", xscale, yscale);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Test Window", nullptr, nullptr);

    float wxscale, wyscale;
    glfwGetWindowContentScale(window, &wxscale, &wyscale);
    printf("Window Content Scale: (x: %f, y: %f)\n", wxscale, wyscale);

    int32 widtppx, heightpx;
    glfwGetFramebufferSize(window, &widtppx, &heightpx);
    printf("Window Rendering Framebuffer: (w: %i, h: %i)\n", widtppx, heightpx);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

TEST_F(TestFixture, GlfwWindow) {
    glfwInit();

    bool shouldClose = false;

    struct Listener: public WindowEventListener {
        bool &shouldClose;
        Window &window;

        explicit Listener(bool& shouldClose, Window& window) : shouldClose(shouldClose), window(window) {}
        ~Listener() override = default;

        void OnWindowEvent(const WindowEvent &event) override {
            switch (event.eventType) {
                case EWindowEventType::Focus:
                    BERSERK_LOG_INFO("Focus ", event.focus, " on ", window.GetName());
                    return;
                case EWindowEventType::Move:
                    BERSERK_LOG_INFO("Window ", window.GetName(), " moved: ", event.position);
                    return;
                case EWindowEventType::Resize:
                    BERSERK_LOG_INFO("Window ", window.GetName(), " resized: ", event.size);
                    return;
                case EWindowEventType::Framebuffer:
                    BERSERK_LOG_INFO("Window ", window.GetName(), " fbo resized: ", event.framebufferSize);
                    return;
                case EWindowEventType::Close:
                    BERSERK_LOG_INFO("Request close on ", window.GetName());
                    shouldClose = true;
                    return;
                default:
                    return;
            }
        }
    };

    {
        GlfwWindowManager windowManager;
        WindowDesc windowDesc;
        windowDesc.name = "MAIN_WINDOW";
        windowDesc.title = "Test Window Glfw";
        windowDesc.size = Size2i(1280, 720);
        auto window = windowManager.CreateWindow(std::move(windowDesc));

        Listener listener(shouldClose, *window);
        window->AddListener(listener);

        while (!shouldClose) {
            glfwPollEvents();
        }
    }

    glfwTerminate();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}