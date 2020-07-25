/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TestMacro.h>

#include <Main.h>
#include <Platform/Window.h>
#include <Platform/WindowManager.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Engine)
{
    BERSERK_TEST_COND(EngineStartUp, true)
    {
        Main main;
        main.initialize(0, nullptr);

        auto& man = WindowManager::getSingleton();
        auto  window = man.find("MAIN_WINDOW");

        class WindowListener : public WindowStateListener,
                               public WindowResizeListener,
                               public WindowPositionListener {
        public:
            void onResized(Size2i oldSize, Size2i newSize) override {
                printf("Main window size: {%i,%i}\n", newSize[0], newSize[1]);
            }
            void onMoved(Point2i oldPosition, Point2i newPosition) override {
                printf("Main window position: {%i,%i}\n", newPosition[0], newPosition[1]);
            }
            void onStateChanged(EWindowState oldState, EWindowState newState) override {
                switch (newState) {
                    case EWindowState::Normal:
                        printf("Window state: Normal\n");
                        break;
                    case EWindowState::Minimised:
                        printf("Window state: Minimised\n");
                        break;
                    case EWindowState::Closed:
                        printf("Window state: Closed\n");
                        break;
                    default:
                        return;
                }
            }
        } listener;

        window->addPositionListener(listener);
        window->addResizeListener(listener);
        window->addStateListener(listener);

        main.enterMainLoop();

        window->removePositionListener(listener);
        window->removeResizeListener(listener);
        window->removeStateListener(listener);

        main.finalize();
    };
}
