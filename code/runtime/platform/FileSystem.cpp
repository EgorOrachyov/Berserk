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
#include <platform/FileSystem.hpp>

#include <sys/stat.h>

BRK_NS_BEGIN

FileSystem::FileSystem() {
    Init();
}

void FileSystem::CloseFile(std::FILE *file) {
    assert(file);
    std::fclose(file);
}

Ref<Data> FileSystem::ReadFile(const String &filepath) {
    auto *file = OpenFile(filepath, "rb");

    if (!file) {
        BRK_ERROR("Failed to open file filepath=" << filepath);
        return Ref<Data>();
    }

    struct stat statBuf {};
    if (stat(filepath.c_str(), &statBuf) == -1) {
        BRK_ERROR("Failed to get file stat filepath=" << filepath);
        return Ref<Data>();
    }

    auto size = static_cast<size_t>(statBuf.st_size);
    auto data = Data::Make(size);

    if (std::fread(data->GetDataWrite(), 1, size, file) != size) {
        BRK_ERROR("Failed to read file filepath=" << filepath << " size=" << size);
        return Ref<Data>();
    }

    return data;
}

void FileSystem::AddSearchPath(String path) {
    if (path.empty()) {
        BRK_ERROR("Passed empty path as search path");
        return;
    }

    if (path.back() != '/')
        path += '/';

    std::lock_guard<std::recursive_mutex> guard(mMutex);
    mSearchPaths.push_back(std::move(path));
    ClearCache();
}

void FileSystem::SetSearchPaths(std::vector<String> searchPaths) {
    for (auto &path : searchPaths) {
        if (path.empty()) {
            BRK_ERROR("Passed empty search path");
            continue;
        }

        if (path.back() != '/')
            path += '/';
    }

    std::lock_guard<std::recursive_mutex> guard(mMutex);
    mSearchPaths = std::move(searchPaths);
    ClearCache();
}

String FileSystem::GetFullFilePath(const String &filename) {
    std::lock_guard<std::recursive_mutex> guard(mMutex);

    if (filename.empty())
        return String();

    if (IsAbsolutePath(filename))
        return filename;

    auto cached = mCachedFullFilePath.find(filename);
    if (cached != mCachedFullFilePath.end())
        return cached->second;

    String resolvedPath;

    for (const auto &prefix : mSearchPaths) {
        resolvedPath = ResolveFilePath(prefix, filename);

        if (!resolvedPath.empty()) {
            mCachedFullFilePath.emplace(filename, resolvedPath);
            return resolvedPath;
        }
    }

    return String();
}

String FileSystem::GetFullDirPath(const String &dirname) {
    std::lock_guard<std::recursive_mutex> guard(mMutex);

    if (dirname.empty())
        return String();

    if (IsAbsolutePath(dirname))
        return dirname;

    auto cached = mCachedFullDirPath.find(dirname);
    if (cached != mCachedFullDirPath.end())
        return cached->second;

    String dirnameCorrected = dirname;

    if (dirnameCorrected.back() != '/')
        dirnameCorrected += '/';

    String resolvedPath;

    for (const auto &prefix : mSearchPaths) {
        resolvedPath = ResolveDirPath(prefix, dirnameCorrected);

        if (!resolvedPath.empty() && IsDirExistsAbs(resolvedPath)) {
            mCachedFullDirPath.emplace(dirname, resolvedPath);
            return resolvedPath;
        }
    }

    return String();
}

String FileSystem::GetFileExtension(const String &filename) {
    auto pos = filename.find_last_of('.');
    if (pos != String::npos)
        return filename.substr(pos + 1);

    return String();
}

bool FileSystem::IsFileExists(const String &filename) {
    auto fullPath = GetFullFilePath(filename);
    return !fullPath.empty() && IsFileExistsAbs(fullPath);
}

bool FileSystem::IsDirExists(const String &dirname) {
    auto fullPath = GetFullFilePath(dirname);
    return !fullPath.empty() && IsDirExistsAbs(fullPath);
}

void FileSystem::ClearCache() {
    mCachedFullFilePath.clear();
    mCachedFullDirPath.clear();
}

String FileSystem::GetPathForFile(const String &path, const String &filename) {
    // Make path/filename
    String ret = path;
    if (!path.empty() && path.back() != '/')
        ret += '/';
    ret += filename;

    // Check if exists
    if (IsFileExists(ret))
        return ret;

    return String();
}

const String &FileSystem::GetExecutablePath() {
    return mExecutablePath;
}

String FileSystem::GetExecutableDir() const {
    auto pos = mExecutablePath.find_last_of('/');
    if (pos != String::npos)
        return mExecutablePath.substr(0, pos);

    return ".";
}

BRK_NS_END