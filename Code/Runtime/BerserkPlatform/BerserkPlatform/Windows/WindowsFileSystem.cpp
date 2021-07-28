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

#include <BerserkPlatform/Windows/WindowsFileSystem.hpp>
#include <BerserkPlatform/PlatformFile.hpp>
#include <whereami.h>
#include <cstdlib>

namespace Berserk {

    WindowsFileSystem::WinImpl::WinImpl() {
        // Get absolute path to executable
        {
            int32 pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
            String path(pathLength + 1);
            wai_getExecutablePath(path.GetStr_C(), pathLength, nullptr);
            path.GetStr_C()[pathLength] = '\0';
            mExecutablePath = std::move(path);
        }

        // Get home dir
        {
            mHomeDirectory = getenv("USERPROFILE");
        }

        Provide(this);
    }

    WindowsFileSystem::WinImpl::~WinImpl() {
        Remove(this);
    }

    const String &WindowsFileSystem::WinImpl::GetExecutablePath() {
        return mExecutablePath;
    }

    const String &WindowsFileSystem::WinImpl::GetHomeDirectory() {
        return mHomeDirectory;
    }

    String WindowsFileSystem::WinImpl::GetCurrentDirectory() {
        return String();
    }

    String WindowsFileSystem::WinImpl::GetAbsolutePath(const String &path) {
        return String();
    }

    SharedPtr<File> WindowsFileSystem::WinImpl::OpenFile(const String &filepath, File::Mode mode) {
        PlatformFile unixFile(filepath, mode);

        if (unixFile.IsOpen())
            return  (SharedPtr<File>) SharedPtr<PlatformFile>::MakeMove(std::move(unixFile));

        return {};
    }

    void WindowsFileSystem::WinImpl::ListDirectory(const String &path, Array<Entry> &entries) {

    }

    bool WindowsFileSystem::WinImpl::SetCurrentDirectory(const String &path) {
        return false;
    }

    bool WindowsFileSystem::WinImpl::Exists(const String &path) {
        return false;
    }

    bool WindowsFileSystem::WinImpl::CreateDirectory(const String &path) {
        return false;
    }

    bool WindowsFileSystem::WinImpl::RemoveEntry(const String &path) {
        return false;
    }

}