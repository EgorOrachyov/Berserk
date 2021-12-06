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

#ifndef BERSERK_FILESYSTEM_HPP
#define BERSERK_FILESYSTEM_HPP

#include <core/Config.hpp>
#include <core/Data.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>
#include <core/templates/Ref.hpp>

#include <cstdio>
#include <mutex>
#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/**
 * @class FileSystem
 * @brief Target platform file system access and file utils
 */
class FileSystem final {
public:
    /** Type of directory entry */
    enum class EntryType {
        /** Simple file */
        File,
        /** Directory */
        Directory,
        /** Link or hyper-link for a file/directory */
        Link,
        /** Entry has unknown type */
        Unknown
    };

    /** Directory entry */
    struct Entry {
        String name;
        EntryType type;
    };

    /** Init file system */
    BRK_API FileSystem();

    /**
     * @brief Open file by file path and mode
     *
     * Opens file using provided full absolute file path.
     * Automatically converts string path to required encoding of platform.
     *
     * @param filepath Absolute (full) path to file
     * @param mode Mode to open file
     *
     * @return File handle or null if failed open
     */
    BRK_API std::FILE *OpenFile(const String &filepath, const String &mode);

    /**
     * @brief Read file by file path
     *
     * Attempts to open and read content of the file, specified by file path.
     * File content is read into allocated data container.
     * If fails to open or read file, returns null.
     *
     * @param filepath Absolute (full) path to file
     *
     * @return Data or null if failed read file
     */
    BRK_API Ref<Data> ReadFile(const String &filepath);

    /**
     * @brief Add search path
     *
     * Adds search path used to search engine files.
     * New path is added as last path in the list of search paths (with lower priority).
     * Adding new path invalidates previously cached path lookups.
     *
     * @param path Search path to add; must be not empty
     */
    BRK_API void AddSearchPath(String path);

    /**
     * @brief Set search paths for paths resolution
     *
     * Uses passed search paths to resolve relative
     * file and directory paths to full paths.
     *
     * @param searchPaths List of search paths
     */
    BRK_API void SetSearchPaths(std::vector<String> searchPaths);

    /**
     * @brief Get full path for specified file
     *
     * Attempts to resolve passed relative file path
     * using configured search paths of the file system class.
     *
     * Uses cache of resolved entries to speed-up look ups.
     *
     * @param filename Relative or absolute path
     * @return Full path if resolved
     */
    BRK_API String GetFullFilePath(const String &filename);

    /**
     * @brief Get full path for specified directory
     *
     * Attempts to resolve passed relative directory path
     * using configured search paths of the file system class.
     *
     * Uses cache of resolved entries to speed-up look ups.
     *
     * @param dirname Relative or absolute path
     * @return Full path if resolved
     */
    BRK_API String GetFullDirPath(const String &dirname);

    /**
     * @brief Retrieve file extension
     *
     * @param filename File name to get extension
     * @return Extension if present
     */
    BRK_API String GetFileExtension(const String &filename);

    /**
     * @brief Get file name from file path
     *
     * Get file name from relative or absolute path to the file.
     * This function preserves file extension.
     *
     * @param filename Relative or absolute path
     * @return File name
     */
    BRK_API String GetFileName(const String &filename);

    /**
     * @brief Check is passed path is absolute
     *
     * @param filename Path to check
     * @return True if absolute
     */
    BRK_API bool IsAbsolutePath(const String &filename);

    /**
     * @brief Check if specified file exists
     * @note Uses search path to resolve relative path if passed
     *
     * @param filename Relative or absolute path of the file
     * @return True if file exists
     */
    BRK_API bool IsFileExists(const String &filename);

    /**
     * @brief Check if specified directory exists
     * @note Uses search path to resolve relative path if passed
     *
     * @param dirname Relative or absolute path of the directory
     * @return True if directory exists
     */
    BRK_API bool IsDirExists(const String &dirname);

    /**
     * @brief List entries of the specified directory
     * @note Uses search path to resolve relative path if passed
     *
     * @param dir Relative or absolute path of the directory
     * @return List of directory entries
     */
    BRK_API std::vector<Entry> ListDir(const String &dir);

    /**
     * @brief Path to the executable file of the application
     * @return Path
     */
    BRK_API const String &GetExecutablePath();

private:
    void Init();
    void ClearCache();
    bool IsFileExistsAbs(const String &filename);
    bool IsDirExistsAbs(const String &dirname);
    String ResolveFilePath(const String &prefix, const String &file);
    String ResolveDirPath(const String &prefix, const String &dir);
    String GetPathForFile(const String &path, const String &filename);

    /** List of search paths for files; store in descending priority order */
    std::vector<String> mSearchPaths;

    /** Cached full file path look-ups */
    std::unordered_map<String, String> mCachedFullFilePath;

    /** Cached full dir path look-ups */
    std::unordered_map<String, String> mCachedFullDirPath;

    /** Path to the executable file of the application */
    String mExecutablePath;

    mutable std::recursive_mutex mMutex;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_FILESYSTEM_HPP
