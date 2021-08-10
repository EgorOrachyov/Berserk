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
#include <BerserkCore/Strings/String16u.hpp>
#include <whereami.h>
#include <cstdlib>
#include <windows.h>

#ifdef GetCurrentDirectory
 #undef GetCurrentDirectory
#endif

#ifdef SetCurrentDirectory
 #undef SetCurrentDirectory
#endif

#ifdef CreateDirectory
 #undef CreateDirectory
#endif

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
        Guard<Mutex> guard(mMutex);

        DWORD bufferLength = GetCurrentDirectoryW(0, nullptr);
        String16u buffer(bufferLength);

        GetCurrentDirectoryW(bufferLength, reinterpret_cast<LPWSTR>(buffer.GetStr_C()));

        String dirName;
        return buffer.ToUtf8(dirName)? dirName: "";
    }

    String WindowsFileSystem::WinImpl::GetAbsolutePath(const String &path) {
        String16u path16u;

        if (path.ToUtf16(path16u)) {
            DWORD bufferSize = GetFullPathNameW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C()), 0, nullptr, nullptr);
            String16u buffer(bufferSize);

            GetFullPathNameW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C()), bufferSize,
                             reinterpret_cast<LPWSTR>(buffer.GetStr_C()), nullptr);

            String fullPath;
            return buffer.ToUtf8(fullPath)? fullPath: "";
        }

        return "";
    }

    SharedPtr<File> WindowsFileSystem::WinImpl::OpenFile(const String &filepath, File::Mode mode) {
        PlatformFile unixFile(filepath, mode);

        if (unixFile.IsOpen())
            return  (SharedPtr<File>) SharedPtr<PlatformFile>::MakeMove(std::move(unixFile));

        return {};
    }

    void WindowsFileSystem::WinImpl::ListDirectory(const String &path, Array<Entry> &entries) {
        WIN32_FIND_DATA fileData;
        HANDLE hFind;
        String16u path16u;

        if (path.ToUtf16(path16u)) {
            Guard<Mutex> guard(mMutex);

            // Append search pattern '\*'
            if (!path.EndsWith("/") && !path.EndsWith("\\"))
                path16u.AddChar(L'\\');
            path16u.AddChar(L'*');

            hFind = FindFirstFileW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C()), &fileData);

            if (hFind == INVALID_HANDLE_VALUE)
                return;

            do {
                Entry entry{};
                entry.type = EntryType::File;

                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    entry.type = EntryType::Directory;

                String16u entryNameu16(reinterpret_cast<const String16u::CharType*>(fileData.cFileName));
                entryNameu16.ToUtf8(entry.name);

                entries.Move(std::move(entry));
            } while (FindNextFileW(hFind, &fileData));

            FindClose(hFind);
        }
    }

    bool WindowsFileSystem::WinImpl::SetCurrentDirectory(const String &path) {
        String16u path16u;

        if (!path.ToUtf16(path16u))
            return false;

        Guard<Mutex> guard(mMutex);
        return SetCurrentDirectoryW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C()));
    }

    bool WindowsFileSystem::WinImpl::Exists(const String &path) {
        String16u path16u;

        if (!path.ToUtf16(path16u))
            return false;

        Guard<Mutex> guard(mMutex);
        return GetFileAttributesW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C())) != INVALID_FILE_ATTRIBUTES;
    }

    bool WindowsFileSystem::WinImpl::CreateDirectory(const String &path) {
        String16u path16u;

        if (!path.ToUtf16(path16u))
            return false;

        Guard<Mutex> guard(mMutex);
        return CreateDirectoryW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C()), nullptr);
    }

    bool WindowsFileSystem::WinImpl::RemoveEntry(const String &path) {
        String16u path16u;

        if (!path.ToUtf16(path16u))
            return false;

        Guard<Mutex> guard(mMutex);
        return DeleteFileW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C())) ||
               RemoveDirectoryW(reinterpret_cast<LPCWSTR>(path16u.GetStr_C()));
    }

}