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

#include <BerserkPlatform/Unix/UnixFileSystem.hpp>
#include <BerserkPlatform/PlatformFile.hpp>
#include <whereami.h>

#include <climits>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <pwd.h>

namespace Berserk {

    UnixFileSystem::UnixImpl::UnixImpl() {
        // Get absolute path to executable
        {
            int32 pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
            String path(pathLength + 1);
            wai_getExecutablePath(path.GetStr_C(), pathLength, nullptr);
            path.GetStr_C()[pathLength] = '\0';
            mExecutablePath = std::move(path);
        }

        // Home directory for linux based os
        {
            const char* home;
            if ((home = getenv("HOME")) == nullptr)
                home = getpwuid(getuid())->pw_dir;
            mHomeDirectory = home;
        }

        Provide(this);
    }

    UnixFileSystem::UnixImpl::~UnixImpl() {
        Remove(this);
    }

    const String &UnixFileSystem::UnixImpl::GetExecutablePath() {
        return mExecutablePath;
    }

    const String &UnixFileSystem::UnixImpl::GetHomeDirectory() {
        return mHomeDirectory;
    }

    String UnixFileSystem::UnixImpl::GetCurrentDirectory() {
        Guard<Mutex> guard(mMutex);

        const uint64 size = PATH_MAX * 8;
        char buffer[size];
        auto result = getcwd(buffer, size);

        return result == buffer? buffer: String();
    }

    SharedPtr<File> UnixFileSystem::UnixImpl::OpenFile(const String &filepath, File::Mode mode) {
        PlatformFile unixFile(filepath, mode);

        if (unixFile.IsOpen())
            return  (SharedPtr<File>) SharedPtr<PlatformFile>::MakeMove(std::move(unixFile));

        return {};
    }

    void UnixFileSystem::UnixImpl::ListDirectory(const String &path, Array<Entry> &entries) {
        Guard<Mutex> guard(mMutex);

        DIR* dir = opendir(path.GetStr_C());

        if (dir) {
            dirent* entry;

            while ((entry = readdir(dir)) != nullptr) {
                auto& result = entries.Emplace();
                result.name = entry->d_name;

                switch (entry->d_type) {
                    case DT_REG:
                        result.type = EntryType::File;
                        break;
                    case DT_DIR:
                        result.type = EntryType::Directory;
                        break;
                    case DT_LNK:
                        result.type = EntryType::Link;
                        break;
                    default:
                        result.type = EntryType::Unknown;
                }
            }

            closedir(dir);
        }
    }

    String UnixFileSystem::UnixImpl::GetAbsolutePath(const String &path) {
        char resolved[PATH_MAX * 8];
        auto result = realpath(path.GetStr_C(), resolved);

        return result == resolved? resolved: String();
    }

    bool UnixFileSystem::UnixImpl::SetCurrentDirectory(const String &path) {
        Guard<Mutex> guard(mMutex);
        return chdir(path.GetStr_C()) == 0;
    }

    bool UnixFileSystem::UnixImpl::Exists(const String &path) {
        Guard<Mutex> guard(mMutex);
        return access(path.GetStr_C(), F_OK) == 0;
    }

    bool UnixFileSystem::UnixImpl::CreateDirectory(const String &path) {
        Guard<Mutex> guard(mMutex);
        auto mask = 0777;
        return mkdir(path.GetStr_C(), mask) == 0;
    }

    bool UnixFileSystem::UnixImpl::RemoveEntry(const String &path) {
        Guard<Mutex> guard(mMutex);
        return remove(path.GetStr_C()) == 0;
    }

}