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

#ifdef BERSERK_WITH_TFD
#include <BerserkCore/Platform/FileSystem.hpp>
#include <tfd/tinyfiledialogs.h>
#define BERSERK_TFD_SPLITTER "|"
#define BERSERK_TDF_ALLOW_MULTIPLE_SELECTS 1
#define BERSERK_TDF_SINGLE_SELECTS 0
#define BERSERK_TDF_WIN_UTF8 1
#endif

#include <portable-file-dialogs.h>

namespace Berserk {

    Dialogs::Impl::Impl() {
#if defined(BERSERK_WITH_TFD) && defined(BERSERK_TARGET_WINDOWS)
        // On all platforms string in utf-8, so force utf-8 in tfd
        tinyfd_winUtf8 = BERSERK_TDF_WIN_UTF8;
#endif
    }

    void Dialogs::Impl::OpenFileDialog(const String& title, const String &defaultPath, bool multipleSelect, const Array<String> &patterns, Array<String> &paths) {
#ifdef BERSERK_WITH_TFD
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
                    multipleSelect? BERSERK_TDF_ALLOW_MULTIPLE_SELECTS: BERSERK_TDF_SINGLE_SELECTS
            );

            if (tinyfdResults != nullptr) {
                result = std::move(String(tinyfdResults));
            }
        }

        result.Split(BERSERK_TFD_SPLITTER, paths);
#else
        std::string pfdTitle = title.GetStr_C();
        std::string pfdInitialPath = defaultPath.GetStr_C();
        std::vector<std::string> pfdFilters;
        pfd::opt pfdSelectOption = multipleSelect ? pfd::opt::multiselect : pfd::opt::none;

        if (patterns.IsNotEmpty()) {
            pfdFilters.emplace_back(BERSERK_TEXT("All Files"));

            for (const auto& filter: patterns)
                pfdFilters.emplace_back(filter.GetStr_C());
        }

        std::vector<std::string> result;

        {
            Guard<Mutex> guard(mMutex);
            result = std::move(pfd::open_file(pfdTitle, pfdInitialPath, pfdFilters, pfdSelectOption).result());
        }

        paths.EnsureToAdd(result.size());
        for (auto& path: result) {
            paths.Emplace(path.c_str());
        }
#endif
    }

    bool Dialogs::Impl::OpenFolderDialog(const String& title, const String &defaultPath, String &folderPath) {
#ifdef BERSERK_WITH_TFD
        Guard<Mutex> guard(mMutex);

        const char *tinyfdResult = tinyfd_selectFolderDialog(
                title.GetStr_C(),
                defaultPath.GetStr_C()
        );

        if (tinyfdResult != nullptr) {
            folderPath = std::move(String(tinyfdResult));
        }

        return tinyfdResult != nullptr;
#else
        std::string pfdTitle = title.GetStr_C();
        std::string pfdInitialPath = defaultPath.GetStr_C();
        pfd::opt pfdSelectOption = pfd::opt::none;

        std::string result;

        {
            Guard<Mutex> guard(mMutex);
            result = std::move(pfd::select_folder(pfdTitle, pfdInitialPath, pfdSelectOption).result());
        }

        folderPath = result.c_str();
        return !result.empty();
#endif
    }

    bool Dialogs::Impl::OpenSaveDialog(const String& title, const String &defaultPath, const String &defaultName, const Array<String> &patterns, String& filePath) {
#ifdef BERSERK_WITH_TFD
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
#else
        std::string pfdTitle = title.GetStr_C();
        std::string pfdInitialPath = defaultPath.GetStr_C();
        std::vector<std::string> pfdFilters;
        pfd::opt pfdSelectOption = pfd::opt::none;

        if (patterns.IsNotEmpty()) {
            pfdFilters.emplace_back(BERSERK_TEXT("All Files"));

            for (const auto& filter: patterns)
                pfdFilters.emplace_back(filter.GetStr_C());
        }

        std::string result;

        {
            Guard<Mutex> guard(mMutex);
            result = std::move(pfd::save_file(pfdTitle, pfdInitialPath, pfdFilters, pfdSelectOption).result());
        }

        filePath = result.c_str();
        return !result.empty();
#endif
    }

    void Dialogs::Impl::SendNotification(const String &title, const String &text, NotificationType notificationType) {
        std::string pfdTitle = title.GetStr_C();
        std::string pfdText = text.GetStr_C();
        pfd::icon pfdIcon = pfd::icon::info;

        switch (notificationType) {
            case NotificationType::Info:
                pfdIcon = pfd::icon::info;
                break;
            case NotificationType::Warning:
                pfdIcon = pfd::icon::warning;
                break;
            case NotificationType::Error:
                pfdIcon = pfd::icon::error;
                break;
            case NotificationType::Question:
                pfdIcon = pfd::icon::question;
                break;
            default:
                break;
        }

        {
            Guard<Mutex> guard(mMutex);
            auto notification = pfd::notify(pfdTitle, pfdText, pfdIcon);

            while (!notification.ready());
        }
    }

}