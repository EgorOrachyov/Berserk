/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/MacSystem.hpp>
#include <BerserkPlatform/PlatformFile.hpp>

namespace Berserk {

    MacSystem::~MacSystem() {

    }

    void MacSystem::OnWarning(const char *message) {

    }

    void MacSystem::OnError(const char *message) {

    }

    void MacSystem::OnFatalError(const char *message) {

    }

    TPtrShared<File> MacSystem::OpenFile(String filePath, EFileMode fileMode) {
        return TPtrShared<File>();
    }

    const String &MacSystem::GetExecutablePath() const {
        return mExecutablePath;
    }
}