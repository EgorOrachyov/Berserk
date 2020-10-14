/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_WINDOWMANAGER_HPP
#define BERSERK_WINDOWMANAGER_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/TPtrShared.hpp>
#include <BerserkCore/Containers/TArray.hpp>

namespace Berserk {

    struct WindowDesc {
        String name;
        String title;
        Size2i size;
    };

    /** Handel platform specific window functionality */
    class WindowManager {
    public:
        virtual ~WindowManager() = default;
        virtual TPtrShared<Window> CreateWindow(WindowDesc windowDesc) = 0;
        virtual TPtrShared<Window> GetWindow(const String& name) const = 0;
        virtual TPtrShared<Window> GetWindowInFocus() const = 0;
        virtual bool HasWindow(const String& name) const = 0;
        virtual void GetWindows(TArray<TPtrShared<Window>> &windows) const = 0;
    };

}

#endif //BERSERK_WINDOWMANAGER_HPP