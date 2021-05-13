/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/FileSystem.hpp>

namespace Berserk {

    String FileSystem::GetFileNameFromPath(const String& path) {
    #if BERSERK_PATH_TYPE == BERSERK_PATH_TYPE_WINDOWS
        return GetFileNameFromPathWindows(path);
    #else
        return GetFileNameFromPathUnix(path);
    #endif
    }

    String FileSystem::GetFileNameFromPathUnix(const String &path) {
        String::CharType sep[] = { UnixFileSeparator, String::END };
        auto result = path.FindLast(sep);

        if (result) {
            auto length = path.GetLength() - result.index - 1;
            return path.SubString(result.index + 1, length);
        }

        // Whole path is file name ???
        return path;
    }

    String FileSystem::GetFileNameFromPathWindows(const String &path) {
        String::CharType sep[] = { WindowsFileSeparator, String::END };
        auto result = path.FindLast(sep);

        if (result) {
            auto length = path.GetLength() - result.index - 1;
            return path.SubString(result.index + 1, length);
        }

        // Whole path is file name ???
        return path;
    }

    String FileSystem::GetFileExtension(const String &filename) {
        auto result = filename.FindLast(BERSERK_TEXT("."));

        if (result) {
            auto length = filename.GetLength() - result.index - 1;
            return filename.SubString(result.index + 1, length);
        }

        return "";
    }

}
