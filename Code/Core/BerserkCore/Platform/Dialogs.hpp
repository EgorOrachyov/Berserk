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

#ifndef BERSERK_DIALOGS_HPP
#define BERSERK_DIALOGS_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    /**
     * @brief Gui Dialogs.
     *
     * Provides access to native platform-specific gui dialogs.
     * Supports file selection, folder selection, file save dialogs.
     */
    class Dialogs {
    public:

        /**
         * Open native OS file select dialog.
         * Blocks current thread until user selects files and confirms his/her choice.
         *
         * @param title Title of the dialog to set
         * @param defaultPath Path of directory, where open dialog
         * @param patterns Patterns of files to show user in form { "*.jpg", "*.png" } etc.
         * @param paths Returned selected files by the user
         */
        static void OpenFileDialog(const String& title, const String& defaultPath, const Array<String> &patterns, Array<String> &paths) {
            Impl::Instance().OpenFileDialog(title, defaultPath, patterns, paths);
        }

        /**
         * Open native OS folder select dialog.
         * Blocks current thread until user selects folder and confirms his/her choice.
         *
         * @param title Title of the dialog to set
         * @param defaultPath Path of directory, where open dialog
         * @param folderPath Selected folder path
         *
         * @return True if user selected something
         */
        static bool OpenFolderDialog(const String& title, const String& defaultPath, String &folderPath) {
            return Impl::Instance().OpenFolderDialog(title, defaultPath, folderPath);
        }

        /**
         * Open native OS file save dialog.
         * Blocks current thread until user selects folder and confirms his/her choice.
         *
         * @param title Title of the dialog to set
         * @param defaultPath Path of directory, where open dialog
         * @param defaultName Default save file name
         * @param patterns Patterns of files to show user in form { "*.jpg", "*.png" } etc.
         * @param filePath Selected file path
         *
         * @return True if user selected something
         */
        static bool OpenSaveDialog(const String& title, const String& defaultPath, const String& defaultName, const Array<String> &patterns, String& filePath) {
            return Impl::Instance().OpenSaveDialog(title, defaultPath, defaultName, patterns, filePath);
        }

    protected:

        class Impl: public Singleton<Impl> {
        public:
            virtual ~Impl() = default;
            virtual void OpenFileDialog(const String& title, const String& defaultPath, const Array<String> &patterns, Array<String> &paths);
            virtual bool OpenFolderDialog(const String& title, const String& defaultPath, String &folderPath);
            virtual bool OpenSaveDialog(const String& title, const String& defaultPath, const String& defaultName, const Array<String> &patterns, String& filePath);
        protected:
            Mutex mMutex;
        };

    };

}




#endif //BERSERK_DIALOGS_HPP
