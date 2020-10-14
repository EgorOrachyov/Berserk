/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PLATFORMAPPLICATION_HPP
#define BERSERK_PLATFORMAPPLICATION_HPP

#include <BerserkCore/Application.hpp>
#include <BerserkCore/BuildOptions.hpp>
#include <BerserkPlatform/PlatformLog.hpp>

#if defined(BERSERK_TARGET_LINUX)
#include <BerserkPlatform/Linux.hpp>
#endif

namespace Berserk {

    class PlatformApplication: public Application {
    public:
        PlatformApplication(uint32 argc, const char** argv);
        ~PlatformApplication() override;

        Log &GetLog() override;
        System &GetSystem() override;

        const TArray<String> &GetCmdLineArgs() override;
        const String &GetAppName() override;
        const String &GetAuthorName() override;
        const String &GetCredits() override;
        const String &GetExecDir() override;
        const String &GetBaseDir() override;
        const String &GetResourcesDir() override;
        const String &GetEngineDir() override;
        const String &GetEditorDir() override;

        void Abort(const char *message, uint32 length) override;
        void RequestExit() override;
        void EnterMainLoop() override;

    private:
#if defined(BERSERK_TARGET_LINUX)
        Linux mSystem;
#endif
        TArray<String> mCmdLineArgs;
        String mAppName;
        String mAuthor;
        String mCredits;
        String mExecDir;
        String mBaseDir;
        String mResDir;
        String mEngineDir;
        String mEditorDir;
        PlatformLog mLog;
    };

}

#endif //BERSERK_PLATFORMAPPLICATION_HPP