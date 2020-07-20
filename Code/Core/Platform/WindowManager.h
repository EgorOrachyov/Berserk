/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_WINDOWMANAGER_H
#define BERSERK_WINDOWMANAGER_H

#include <TPtrShared.h>
#include <Platform/Window.h>

namespace Berserk {

    /**
     * @brief Window manager
     *
     * Manager for platform specific window objects. Allows to create and destroy window,
     * manipulate them and pass around as objects.
     *
     * @note Main thread only
     */
    class WindowManager {
    public:

        WindowManager();
        virtual ~WindowManager() = default;

        /**
         * Creates arbitrary window with specified properties.
         * @param name Window unique name for look-up
         * @param caption Window caption shown to the user
         * @param size Window rendering area size
         * @return Window or null if error occurs
         */
        virtual TPtrShared<Window> create(const CString& name, const CString& caption, Size2i size) = 0;

        /**
         * Attempts to find window with specified name
         * @param name Unique window name to find
         * @return Window or null if not found
         */
        virtual TPtrShared<Window> find(const CString& name) = 0;

        /** @return True if contains window with specified name */
        virtual bool contains(const CString& name) const = 0;

        /** @return Application window manager instance */
        static WindowManager& getSingleton();

    private:

        static WindowManager* gWindowManager;
    };

}

#endif //BERSERK_WINDOWMANAGER_H