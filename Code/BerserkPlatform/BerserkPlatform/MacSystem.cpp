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

    void MacSystem::onWarning(const char *message) {

    }

    void MacSystem::onError(const char *message) {

    }

    void MacSystem::onFatalError(const char *message) {

    }

    PtrShared<File> MacSystem::openFile(String filePath, EFileMode fileMode) {
        return PtrShared<File>();
    }

    const String &MacSystem::getExecutablePath() const {
        return mExecutablePath;
    }
}