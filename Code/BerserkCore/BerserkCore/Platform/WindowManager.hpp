/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_WINDOWMANAGER_HPP
#define BERSERK_WINDOWMANAGER_HPP

#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Templates/SharedPointer.hpp>

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