/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Window.h>

namespace Berserk {

    void Window::addResizeListener(Berserk::WindowResizeListener &listener) {
        auto ptr = &listener;

        if (mResizeListeners.contains(ptr))
            BERSERK_ERROR_RET("Window already contains resize listener");

        mResizeListeners.add(ptr);
    }

    void Window::removeResizeListener(Berserk::WindowResizeListener &listener) {
        auto ptr = &listener;
        mResizeListeners.removeElementUnordered(ptr);
    }

    void Window::addPositionListener(Berserk::WindowPositionListener &listener) {
        auto ptr = &listener;

        if (mPositionListeners.contains(ptr))
            BERSERK_ERROR_RET("Window already contains position listener");

        mPositionListeners.add(ptr);
    }

    void Window::removePositionListener(Berserk::WindowPositionListener &listener) {
        auto ptr = &listener;
        mPositionListeners.removeElementUnordered(ptr);
    }

    void Window::addStateListener(Berserk::WindowStateListener &listener) {
        auto ptr = &listener;

        if (mStateListeners.contains(ptr))
            BERSERK_ERROR_RET("Window already contains state listener");

        mStateListeners.add(ptr);
    }

    void Window::removeStateListener(Berserk::WindowStateListener &listener) {
        auto ptr = &listener;
        mStateListeners.removeElementUnordered(ptr);
    }

}