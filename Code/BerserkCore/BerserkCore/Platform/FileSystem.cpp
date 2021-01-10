/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/FileSystem.hpp>
#include <tfd/tinyfiledialogs.h>

#define BERSERK_TFD_SPLITTER "|"
#define BERSERK_TDF_ALLOW_MULTIPLE_SELECTS 1

namespace Berserk {
    namespace Platform {

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

        void FileSystem::Impl::OpenFileDialog(const String& title, const String &defaultPath, const Array<String> &patterns, Array<String> &paths) {
            String result;

            Array<const String::CharType*> rawPatterns;

            if (patterns.IsNotEmpty()) {
                rawPatterns.EnsureToAdd(patterns.GetSize());
                for (const auto& p: patterns) {
                    rawPatterns.Add(p.GetStr_C());
                }
            }

            {
                Guard<Mutex> guard(mMutex);

                const char *tinyfdResults = tinyfd_openFileDialog(
                    title.GetStr_C(),
                    defaultPath.GetStr_C(),
                    rawPatterns.GetSize(),
                    rawPatterns.GetData(),
                    nullptr,
                    BERSERK_TDF_ALLOW_MULTIPLE_SELECTS
                );

                if (tinyfdResults != nullptr) {
                    result = std::move(String(tinyfdResults));
                }
            }

            result.Split(BERSERK_TFD_SPLITTER, paths);
        }

        bool FileSystem::Impl::OpenFolderDialog(const String& title, const String &defaultPath, String &folderPath) {
            Guard<Mutex> guard(mMutex);

            const char *tinyfdResult = tinyfd_selectFolderDialog(
                title.GetStr_C(),
                defaultPath.GetStr_C()
            );

            if (tinyfdResult != nullptr) {
                folderPath = std::move(String(tinyfdResult));
            }

            return tinyfdResult != nullptr;
        }

        bool FileSystem::Impl::OpenSaveDialog(const String& title, const String &defaultPath, const String &defaultName, const Array<String> &patterns, String& filePath) {
            Array<const String::CharType*> rawPatterns;

            if (patterns.IsNotEmpty()) {
                rawPatterns.EnsureToAdd(patterns.GetSize());
                for (const auto& p: patterns) {
                    rawPatterns.Add(p.GetStr_C());
                }
            }

            {
                Guard<Mutex> guard(mMutex);

                const char * tinyfdResult = tinyfd_saveFileDialog(
                    title.GetStr_C(),
                    (FileSystem::MakePath(defaultPath, defaultName)).GetStr_C(),
                    rawPatterns.GetSize(),
                    rawPatterns.GetData(),
                    nullptr
                );

                if (tinyfdResult != nullptr) {
                    filePath = std::move(String(tinyfdResult));
                }

                return tinyfdResult != nullptr;
            }
        }

    }
}
