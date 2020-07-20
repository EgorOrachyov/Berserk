/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MACOS_H
#define BERSERK_MACOS_H

#include <Platform/System.h>
#include <Std/StdFile.h>
#include <Unix/UnixDirectory.h>
#include <GlfwSystem/GlfwInput.h>
#include <GlfwSystem/GlfwWindowManager.h>
#include <AllocPool.h>
#include <IO/Logs.h>
#include <BuildOptions.h>

#include <clocale>
#include <time.h>

#ifdef BERSERK_WITH_OPENGL
    #include <GLDevice.h>
#endif // BERSERK_WITH_OPENGL

namespace Berserk {

    class macOS final : public System {
    public:

        macOS() noexcept;
        ~macOS() override;

        void initialize(const CString &name, const CString &caption, Size2i size, bool forceVSync, ERenderDeviceType deviceType) override;
        void update() override;
        void finalize() override;

        void onError(const char *message, uint64 line, const char *function, const char *file) override;
        void onWarning(const char *message, uint64 line, const char *function, const char *file) override;

        Log &getLog() override { return mDefaultLog; }
        OutputDevice &getOutputDevice() override { return mDefaultOutput; }

        Time getTime() const override;
        Time getTime(const TimeValue &t) const override;
        TimeValue getTimeValue(const Time &t) const override;

        TPtrUnique<File> openFile(CString path, EFileMode mode) override;
        TPtrUnique<Directory> openDirectory(CString path) override;

    private:

        void deallocateFile(void* file);
        void deallocateDirectory(void* directory);

        bool mInitialized = false;
        bool mFinalized = false;

        Mutex mAccessMutex;
        AllocPool mAllocFile;
        AllocPool mAllocDirectory;
        GlfwInput mInput;
        GlfwWindowManager mManager;
        LogStdout mDefaultLog;
        OutputDeviceStd mDefaultOutput;

        static macOS gMacOS;

#ifdef BERSERK_WITH_OPENGL
        TPtrUnique<GLDevice> mDevice;
#endif // BERSERK_WITH_OPENGL
    };

}

#endif //BERSERK_MACOS_H