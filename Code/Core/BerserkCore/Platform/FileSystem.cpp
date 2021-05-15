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
