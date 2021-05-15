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

#include <BerserkCore/Platform/Dialogs.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <tfd/tinyfiledialogs.h>

#define BERSERK_TFD_SPLITTER "|"
#define BERSERK_TDF_ALLOW_MULTIPLE_SELECTS 1

namespace Berserk {

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