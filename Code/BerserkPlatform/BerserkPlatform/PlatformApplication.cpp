/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/PlatformApplication.hpp>

namespace Berserk {

    PlatformApplication::PlatformApplication(uint32 argc, const char **argv): Application() {
        for (uint32 i = 0; i < argc; i++) {
            mCmdLineArgs.Emplace(argv[i]);
        }
    }

    PlatformApplication::~PlatformApplication() {

    }

    Log &PlatformApplication::GetLog() {
        return mLog;
    }

    System &PlatformApplication::GetSystem() {
        return mSystem;
    }

    const TArray<String> &PlatformApplication::GetCmdLineArgs() {
        return mCmdLineArgs;
    }

    const String &PlatformApplication::GetAppName() {
        return mAppName;
    }

    const String &PlatformApplication::GetAuthorName() {
        return mAuthor;
    }

    const String &PlatformApplication::GetCredits() {
        return mCredits;
    }

    const String &PlatformApplication::GetExecDir() {
        return mExecDir;
    }

    const String &PlatformApplication::GetBaseDir() {
        return mBaseDir;
    }

    const String &PlatformApplication::GetResourcesDir() {
        return mResDir;
    }

    const String &PlatformApplication::GetEngineDir() {
        return mEngineDir;
    }

    const String &PlatformApplication::GetEditorDir() {
        return mEditorDir;
    }

    void PlatformApplication::Abort(const char *message, uint32 length) {
        // Perform log dump
        // Save state
        // Save configs
        // Abort
    }

    void PlatformApplication::RequestExit() {
        // Mark as need to exit after this iteration
    }

    void PlatformApplication::EnterMainLoop() {
        // Infinite loop
        // Update state
        // Update system input and windows
    }
}