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

#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

namespace Berserk {

    /**
     * @brief Window manager
     *
     * Provides operations for managing OS application windows.
     */
    class WindowManager {
    public:

        enum class Backend {
            Glfw,
            Unknown
        };

        /**
         * Create window with specified descriptor
         * @param desc Window descriptor
         *
         * @return Window handle
         */
        static SharedPtr<Window> CreateWindow(const Window::Desc &desc) {
            return Impl::Instance().CreateWindow(desc);
        }

        /**
         * Returns current window, which receives focus (input)
         *
         * @return Window handle
         */
        static SharedPtr<Window> GetWindowInFocus() {
            return Impl::Instance().GetWindowInFocus();
        }

        /**
         * Finds window by its name
         * @param name Name of the window to find
         *
         * @return Window handle
         */
        static SharedPtr<Window> GetWindowByName(const StringName& name) {
            return Impl::Instance().GetWindowByName(name);
        }

        /**
         * Get all active windows of the application
         * @param[out] windows
         */
        static void GetWindows(Array<SharedPtr<Window>> &windows) {
            Impl::Instance().GetWindows(windows);
        }

        /**
         * Backend type of this window manager.
         *
         * @return Backend type enum
         */
        static Backend GetBackendType() {
            return Impl::Instance().GetBackendType();
        }

    protected:

        class Impl: public Singleton<Impl> {
        public:
            virtual ~Impl() = default;
            virtual SharedPtr<Window> CreateWindow(const Window::Desc &desc) = 0;
            virtual SharedPtr<Window> GetWindowInFocus() = 0;
            virtual SharedPtr<Window> GetWindowByName(const StringName& name) = 0;
            virtual Backend GetBackendType() const = 0;
            virtual void GetWindows(Array<SharedPtr<Window>> &windows) = 0;
        };

    };
}

#endif //BERSERK_WINDOWMANAGER_HPP