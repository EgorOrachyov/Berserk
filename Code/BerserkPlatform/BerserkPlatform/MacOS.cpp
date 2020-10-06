/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/MacOS.hpp>
#include <BerserkPlatform/PlatformFile.hpp>
#include <BerserkCore/BuildOptions.hpp>

// To get the executable engine location
#ifdef BERSERK_WITH_WHEREAMI
#include <whereami.h>
#endif

namespace Berserk {

    MacOS::MacOS() {
        ExtractExePath();
    }

    MacOS::~MacOS() {

    }

    void MacOS::OnWarning(const char *message) {

    }

    void MacOS::OnError(const char *message) {

    }

    void MacOS::OnFatalError(const char *message) {

    }

    TPtrShared<File> MacOS::OpenFile(String filePath, EFileMode fileMode) {
        PlatformFile platformFile(std::move(filePath), fileMode);
        return platformFile.isSuccessfullyOpened()? (TPtrShared<File>) TPtrShared<PlatformFile>::MakeMove(platformFile): TPtrShared<File>{};
    }

    const String &MacOS::GetExecutablePath() const {
        return mExecutablePath;
    }

    void MacOS::ExtractExePath() {
#ifdef BERSERK_WITH_WHEREAMI
        int32 pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
        String path(pathLength + 1);
        wai_getExecutablePath(path.GetStr(), pathLength, nullptr);
        path.GetStr()[pathLength] = '\0';
        mExecutablePath = std::move(path);
#endif
    }

}