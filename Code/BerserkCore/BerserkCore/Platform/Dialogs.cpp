/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/Dialogs.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <tfd/tinyfiledialogs.h>

#define BERSERK_TFD_SPLITTER "|"
#define BERSERK_TDF_ALLOW_MULTIPLE_SELECTS 1

namespace Berserk {
    namespace Platform {

        void Dialogs::Impl::OpenFileDialog(const String& title, const String &defaultPath, const Array<String> &patterns, Array<String> &paths) {
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

        bool Dialogs::Impl::OpenFolderDialog(const String& title, const String &defaultPath, String &folderPath) {
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

        bool Dialogs::Impl::OpenSaveDialog(const String& title, const String &defaultPath, const String &defaultName, const Array<String> &patterns, String& filePath) {
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