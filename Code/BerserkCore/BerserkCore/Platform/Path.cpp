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

#include <BerserkCore/Platform/Path.hpp>
#include <BerserkCore/Debug/Debug.hpp>

namespace Berserk {

    Path::Path(const char *path, FileSystem::PathType pathType) {
        auto pathTypeIdx = (uint32) pathType;

        if (pathTypeIdx == BERSERK_PATH_TYPE_UNIX)
            ParseUnix(path);
        else if (pathTypeIdx == BERSERK_PATH_TYPE_WINDOWS)
            ParseWindows(path);
        else
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Unsupported path type"));
    }

    Path::Path(const String &path, FileSystem::PathType pathType)
        : Path(path.GetStr_C(), pathType) {

    }

    Path Path::Parent() const {
        Path path;

        if (HasFilename()) {
            path.mDirectories = mDirectories;
        }
        else if (mDirectories.IsNotEmpty()) {
            path.mDirectories.Add(mDirectories.GetData(), mDirectories.GetSize() - 1);
        }

        path.mDevice = mDevice;
        path.mAbsolute = mAbsolute;

        return path;
    }

    void Path::AppendDir(String directory) {
        mDirectories.Move(std::move(directory));
    }

    void Path::SetFilename(String filename) {
        mFilename = std::move(filename);
    }

    void Path::SetDevice(String device) {
        mDevice = std::move(device);
    }

    void Path::SetFilenameAsDirectory() {
        if (HasFilename()) {
            AppendDir(std::move(mFilename));
            mFilename = "";
        }
    }

    String Path::MakePath() const {
#if BERSERK_PATH_TYPE == BERSERK_PATH_TYPE_WINDOWS
        return MakePathWindows();
#else
        return MakePathUnix();
#endif
    }

    String Path::MakePathUnix() const {
        String path;
        size_t i = 0;

        if (mAbsolute) {
            if (mDirectories.IsNotEmpty() && mDirectories[0] == "~") {
                i += 1;
                path.Add("~");
            }

            path.Add(FileSystem::UnixFileSeparator);
        }

        for (; i < mDirectories.GetSize(); i++) {
            path.Add(mDirectories[i]);
            path.Add(FileSystem::UnixFileSeparator);
        }

        if (HasFilename()) {
            path.Add(mFilename);
        }

        return path;
    }

    String Path::MakePathWindows() const {
        String path;

        if (HasDevice()) {
            path.Add(mDevice);
            path.Add(":");
            path.Add(FileSystem::WindowsFileSeparator);
        }
        else if (mAbsolute) {
            path.Add(FileSystem::WindowsFileSeparator);
        }

        for (size_t i = 0; i < mDirectories.GetSize(); i++) {
            path.Add(mDirectories[i]);
            path.Add(FileSystem::WindowsFileSeparator);
        }

        if (HasFilename()) {
            path.Add(mFilename);
        }

        return path;
    }

    void Path::ParseUnix(const char *path) {
        auto length = StringUtils::Length(path);
        auto sep = FileSystem::UnixFileSeparator;
        uint32 i = 0;

        if (i < length) {
            if (path[i] == sep) {
                mAbsolute = true;
                i += 1;
            }
            else if (path[i] == '~') {
                mAbsolute = true;
                AppendDir("~");
                i += 1;
            }

            while (i < length && path[i] == sep)
                i++;

            while (i < length) {
                uint32 current = i;

                while (i < length && path[i] != sep)
                    i++;

                if (i < length) {
                    uint32 dirLength = i - current;

                    if (dirLength > 0) {
                        String dir(path + current, dirLength);
                        AppendDir(std::move(dir));
                    }

                    i++;
                }
                else {
                    uint32 fileLength = i - current;

                    if (fileLength > 0) {
                        String file(path + current, fileLength);
                        SetFilename(std::move(file));
                    }
                }
            }
        }
    }

    void Path::ParseWindows(const char *path) {
        auto length = StringUtils::Length(path);
        auto sep = FileSystem::WindowsFileSeparator;
        uint32 i = 0;

        if (i < length) {
            if (path[i] == sep) {
                mAbsolute = true;
                i += 1;
            } else {
                auto pos = StringUtils::FindFirst(path, ":");

                if (pos != nullptr) {
                    auto location = (uint64)pos - (uint64)path;
                    uint32 nameLength = location - i;

                    assert(nameLength == 1);
                    assert(location == 1);

                    if (nameLength > 0) {
                        String deviceName(path, nameLength);
                        SetDevice(std::move(deviceName));
                        mAbsolute = true;
                    }

                    i = location + 1;
                }
            }

            while (i < length && path[i] == sep)
                i++;

            while (i < length) {
                uint32 current = i;

                while (i < length && path[i] != sep)
                    i++;

                if (i < length) {
                    uint32 dirLength = i - current;

                    if (dirLength > 0) {
                        String dir(path + current, dirLength);
                        AppendDir(std::move(dir));
                    }

                    i++;
                }
                else {
                    uint32 fileLength = i - current;

                    if (fileLength > 0) {
                        String file(path + current, fileLength);
                        SetFilename(std::move(file));
                    }
                }
            }
        }
    }

}