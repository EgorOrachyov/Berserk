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

#ifndef BERSERK_WINDOWSFILESYSTEM_HPP
#define BERSERK_WINDOWSFILESYSTEM_HPP

#include <BerserkCore/Platform/FileSystem.hpp>

namespace Berserk {

    class WindowsFileSystem final: public FileSystem {
    public:

        class WinImpl final: public FileSystem::Impl {
        public:
            WinImpl();
            ~WinImpl() override;
            const String &GetExecutablePath() override;
            const String &GetHomeDirectory() override;
            String GetCurrentDirectory() override;
            String GetAbsolutePath(const String &path) override;
            SharedPtr<File> OpenFile(const String &filepath, File::Mode mode) override;
            void ListDirectory(const String &path, Array<Entry> &entries) override;
            bool SetCurrentDirectory(const String &path) override;
            bool Exists(const String &path) override;
            bool CreateDirectory(const String &path) override;
            bool RemoveEntry(const String &path) override;

        private:
            String mExecutablePath;
            String mHomeDirectory;

            mutable Mutex mMutex;
        };

    };

}

#endif //BERSERK_WINDOWSFILESYSTEM_HPP