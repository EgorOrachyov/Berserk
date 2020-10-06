/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/Linux.hpp>
#include <BerserkPlatform/PlatformFile.hpp>
#include <BerserkCore/BuildOptions.hpp>

// To get the executable engine location
#ifdef BERSERK_WITH_WHEREAMI
#include <whereami.h>
#endif

namespace Berserk {

    Linux::Linux() {
        ExtractExePath();
    }

    void Linux::OnWarning(const char *message) {

    }

    void Linux::OnError(const char *message) {

    }

    void Linux::OnFatalError(const char *message) {

    }

    TPtrShared<File> Linux::OpenFile(String filePath, EFileMode fileMode) {
        PlatformFile platformFile(std::move(filePath), fileMode);
        return platformFile.isSuccessfullyOpened()? (TPtrShared<File>) TPtrShared<PlatformFile>::MakeMove(platformFile): TPtrShared<File>{};
    }

    const String &Linux::GetExecutablePath() const {
        return mExecutablePath;
    }

    void Linux::ExtractExePath() {
#ifdef BERSERK_WITH_WHEREAMI
        int32 pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
        String path(pathLength + 1);
        wai_getExecutablePath(path.GetStr(), pathLength, nullptr);
        path.GetStr()[pathLength] = '\0';
        mExecutablePath = std::move(path);
#endif
    }

}