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

        TWeakPtr<IWindow> window = manager.createWindow(640, 480, "Test window");
        TSharedPtr<IWindow> safe = window.lock();

        int32 i = 0;

        while (!safe->shouldClose())
        {
            manager.update();

            if (i++ == 100000) {
                //safe->minimize();
                //OutputDevice::printf("min\n");
            }

            if (safe->isPositionChanged()) OutputDevice::printf("%i %i \n", safe->getPosX(), safe->getPosY());

            if (i > 1000000) {
                safe->close();
            }
        }

    }

    static void run()
    {
        SystemInitTest1();
    }

};

#endif //BERSERK_GLFWWINDOWSYSTEMTEST_H
