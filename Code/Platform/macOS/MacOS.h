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

    class MacOS final : public System {
    public:

        MacOS() noexcept;
        ~MacOS() override;

        void initialize(const CString &name, const CString &caption, Size2i size, bool forceVSync,
                        ERenderDeviceType deviceType,
                        CString logPath, bool logToFile) override;

        void update() override;
        void finalize() override;

        void requestClose() override;
        bool isCloseRequested() override;

        void onError(const char *message, uint64 line, const char *function, const char *file) override;
        void onWarning(const char *message, uint64 line, const char *function, const char *file) override;

        Log &getLog() override { return mDefaultLog; }
        OutputDevice &getOutputDevice() override { return mDefaultOutput; }
        const CString &getExecutablePath() const override { return mExecutablePath; }
        const CString &getRootPath() const override { return mEnginePath; }
        const CString &getCachePath() const override { return mCachePath; }
        const CString &getConfigPath() const override { return mConfigPath; }

        Time getTime() const override;
        Time getTime(const TimeValue &t) const override;
        TimeValue getTimeValue(const Time &t) const override;

        TPtrUnique<File> openFile(CString path, EFileMode mode) override;
        TPtrUnique<Directory> openDirectory(CString path) override;
        TPtrUnique<Library> openLibrary(CString path) override;

    private:

        void extractExecutablePath();
        void extractRootPath();
        void deallocateFile(void* file);
        void deallocateDirectory(void* directory);

        bool mInitialized = false;
        bool mFinalized = false;

        Mutex mAccessMutex;
        AtomicBool mRequestClose;
        AllocPool mAllocFile;
        AllocPool mAllocDirectory;
        GlfwInput mInput;
        GlfwWindowManager mManager;
        LogStdout mDefaultLog;
        OutputDeviceStd mDefaultOutput;
        CString mExecutablePath;
        CString mEnginePath;
        CString mCachePath;
        CString mConfigPath;
        CString mExecutableName;

        static MacOS gMacOS;

#ifdef BERSERK_WITH_OPENGL
        TPtrUnique<GLDevice> mDevice;
#endif // BERSERK_WITH_OPENGL
    };

}

#endif //BERSERK_MACOS_H