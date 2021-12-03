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

#include <shlwapi.h>
#include <windows.h>

BRK_NS_BEGIN

static inline String PathToUnixStyle(const String &path) {
    String ret = path;
    for (std::size_t i = 0; i < path.length(); i++)
        if (ret[i] == '\\')
            ret[i] = '/';
    return ret;
}

bool FileSystem::IsAbsolutePath(const String &filename) {
    String16u filename16u;

    if (!filename.empty() && Unicode::ConvertUtf8ToUtf16(filename, filename16u))
        return !PathIsRelativeW(reinterpret_cast<LPCWSTR>(filename16u.c_str()));

    return false;
}

std::vector<FileSystem::Entry> FileSystem::ListDirectory(const String &dir) {
    std::vector<Entry> entries;

    String fullDir = GetFullDirPath(dir);
    String16u fullDirU16;

    if (!fullDir.empty() && Unicode::ConvertUtf8ToUtf16(fullDir, fullDirU16)) {
        WIN32_FIND_DATA fileData;
        HANDLE hFind;

        // Append search pattern '/*'
        if (!fullDirU16.empty() && fullDirU16.back() != L'/')
            fullDirU16 += L'/';
        fullDirU16 += L'*';

        hFind = FindFirstFileW(reinterpret_cast<LPCWSTR>(fullDirU16.c_str()), &fileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            return entries;
        }

        do {
            Entry entry;
            entry.type = EntryType::File;

            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                entry.type = EntryType::Directory;

            String16u entryName16u(reinterpret_cast<const char16_t *>(fileData.cFileName));
            String entryName;

            if (Unicode::ConvertUtf16ToUtf8(entryName16u, entryName)) {
                entry.name = std::move(PathToUnixStyle(entryName));
                entries.push_back(std::move(entry));
            }

        } while (FindNextFileW(hFind, &fileData));

        FindClose(hFind);
    }

    return std::move(entries);
}

bool FileSystem::IsFileExistsAbs(const String &filename) {
    String16u fullPath16u;

    if (!filename.empty() && Unicode::ConvertUtf8ToUtf16(filename, fullPath16u)) {
        auto attributes = GetFileAttributesW(reinterpret_cast<LPCWSTR>(fullPath16u.c_str()));
        return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    return false;
}

bool FileSystem::IsDirExistsAbs(const String &dirname) {
    String16u fullPath16u;

    if (!dirname.empty() && Unicode::ConvertUtf8ToUtf16(dirname, fullPath16u)) {
        auto attributes = GetFileAttributesW(reinterpret_cast<LPCWSTR>(fullPath16u.c_str()));
        return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    return false;
}

String FileSystem::ResolvePath(const String &prefix, const String &file) {
    auto unixPrefix = PathToUnixStyle(prefix);
    auto unixFile = PathToUnixStyle(file);
    return GetPathForFile(unixPrefix, unixFile);
}

BRK_NS_END