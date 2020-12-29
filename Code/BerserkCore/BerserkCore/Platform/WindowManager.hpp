/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_WINDOWMANAGER_HPP
#define BERSERK_WINDOWMANAGER_HPP

#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Containers/Array.hpp>

namespace Berserk {
    namespace Platform {

        /**
         * @brief Window manager
         *
         * Provides operations for managing OS application windows.
         */
        class WindowManager {
        public:

            /**
             * Create window with specified descriptor
             * @param desc Window descriptor
             *
             * @return Window handle
             */
            static Ref<Window> CreateWindow(const Window::Desc &desc) {
                return Impl::Instance().CreateWindow(desc);
            }

            /**
             * Returns current window, which receives focus (input)
             *
             * @return Window handle
             */
            static Ref<Window> GetWindowInFocus() {
                return Impl::Instance().GetWindowInFocus();
            }

            /**
             * Finds window by its name
             * @param name Name of the window to find
             *
             * @return Window handle
             */
            static Ref<Window> GetWindowByName(const String& name) {
                return Impl::Instance().GetWindowByName(name);
            }

            /**
             * Get all active windows of the application
             * @param[out] windows
             */
            static void GetWindows(Array<Ref<Window>> &windows) {
                Impl::Instance().GetWindows(windows);
            }

        protected:

            class Impl: public Singleton<Impl> {
            public:
                virtual ~Impl() = default;
                virtual Ref<Window> CreateWindow(const Window::Desc &desc) = 0;
                virtual Ref<Window> GetWindowInFocus() = 0;
                virtual Ref<Window> GetWindowByName(const String& name) = 0;
                virtual void GetWindows(Array<Ref<Window>> &windows) = 0;
            };

        };

    }
}

#endif //BERSERK_WINDOWMANAGER_HPP