//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_GLFWWINDOWSYSTEMTEST_H
#define BERSERK_GLFWWINDOWSYSTEMTEST_H

#include <GlfwWindowManager.h>

using namespace Berserk;

class GlfwWindowSystemTest
{
public:

    static void SystemInitTest1()
    {
        GlfwWindowManager _manager;

        IWindowManager& manager = _manager;
        IWindowRef window = manager.createWindow(640, 480, "Test window");

        window->makeActiveRenderingTarget();

        while (!window->shouldClose())
        {
            manager.update();
        }
    }

    static void run()
    {
        SystemInitTest1();
    }

};

#endif //BERSERK_GLFWWINDOWSYSTEMTEST_H
