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

#include <core/io/Logger.hpp>
#include <core/string/String16u.hpp>
#include <core/string/Unicode.hpp>
#include <platform/FileSystem.hpp>

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <dirent.h>
#include <unistd.h>
#include <whereami.h>

BRK_NS_BEGIN

std::FILE *FileSystem::OpenFile(const String &filepath, const String &mode) {
    return std::fopen(filepath.c_str(), mode.c_str());
}

bool FileSystem::IsAbsolutePath(const String &filename) {
    return !filename.empty() && filename[0] == '/';
}

std::vector<FileSystem::Entry> FileSystem::ListDir(const String &dir) {
    std::vector<Entry> entries;

    String fullDir = GetFullDirPath(dir);

    if (fullDir.empty()) {
        BRK_ERROR("Failed to get full path to list directory dir=" << dir);
        return entries;
    }

    DIR *directory = opendir(fullDir.c_str());

    if (!directory) {
        BRK_ERROR("Failed to open dir=" << fullDir);
        return entries;
    }

    dirent *ent;

    while ((ent = readdir(directory)) != nullptr) {
        Entry entry;
        entry.name = ent->d_name;

        switch (ent->d_type) {
            case DT_REG:
                entry.type = EntryType::File;
                break;
            case DT_DIR:
                entry.type = EntryType::Directory;
                break;
            case DT_LNK:
                entry.type = EntryType::Link;
                break;
            default:
                entry.type = EntryType::Unknown;
        }

        entries.push_back(std::move(entry));
    }

    closedir(directory);

    return std::move(entries);
}

void FileSystem::Init() {
    auto pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
    std::vector<char> path(pathLength, ' ');
    wai_getExecutablePath(path.data(), pathLength, nullptr);
    mExecutablePath = std::move(String(path.data(), pathLength));
}

bool FileSystem::IsFileExistsAbs(const String &filename) {
    return access(filename.c_str(), F_OK) == 0;
}

bool FileSystem::IsDirExistsAbs(const String &dirname) {
    return access(dirname.c_str(), F_OK) == 0;
}

String FileSystem::ResolveFilePath(const String &prefix, const String &file) {
    return GetPathForFile(prefix, file);
}

String FileSystem::ResolveDirPath(const String &prefix, const String &dir) {
    return prefix + dir;
}

BRK_NS_END