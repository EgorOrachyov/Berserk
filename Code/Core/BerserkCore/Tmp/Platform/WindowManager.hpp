/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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