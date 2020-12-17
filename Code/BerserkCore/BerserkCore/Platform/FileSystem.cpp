/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/FileSystem.hpp>

namespace Berserk {
    namespace Platform {

        String FileSystem::GetFileNameFromPath(const String& path) {
        #ifdef BERSERK_TARGET_WINDOWS
            return GetFileNameFromPathWindows(path);
        #elif defined(BERSERK_TARGET_LINUX) || defined(BERSERK_TARGET_MACOS)
            return GetFileNameFromPathUnix(path);
        #else
            #error Unsupported file system path type
        #endif
        }

        String FileSystem::GetFileNameFromPathUnix(const String &path) {
            char sep[] = { UnixFileSeparator, '\0' };
            auto result = path.FindLast(sep);

            if (result && result.index + 1 < path.GetLength()) {
                auto length = path.GetLength() - result.index - 1;
                return path.SubString(result.index + 1, length);
            }

            // Whole path is file name ???
            return path;
        }

        String FileSystem::GetFileNameFromPathWindows(const String &path) {
            char sep[] = { WindowsFileSeparator, '\0' };
            auto result = path.FindLast(sep);

            if (result && result.index + 1 < path.GetLength()) {
                auto length = path.GetLength() - result.index - 1;
                return path.SubString(result.index + 1, length);
            }

            // Whole path is file name ???
            return path;
        }

    }
}
