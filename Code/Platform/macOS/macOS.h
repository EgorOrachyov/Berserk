/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MACOS_H
#define BERSERK_MACOS_H

#include <Platform/ISystem.h>
#include <Std/StdAtomic.h>
#include <Std/StdFile.h>
#include <Std/StdMutex.h>
#include <Unix/UnixDirectory.h>
#include <GlfwSystem/GlfwInput.h>
#include <GlfwSystem/GlfwWindow.h>

#include <IO/Logs.h>

#include <AllocPool.h>
#include <time.h>
#include <clocale>

namespace Berserk {

    class macOS final : public ISystem {
    public:

        macOS() noexcept
            : ISystem(),
              mAllocFile(sizeof(StdFile)),
              mAllocMutex(sizeof(StdMutex)),
              mAllocAtomic(sizeof(StdAtomic)),
              mAllocDirectory(sizeof(UnixDirectory)) {
            std::setlocale(LC_ALL, "");
        }

        ~macOS() override {
#ifdef BERSERK_DEBUG
            uint32 count;
            count = mAllocAtomic.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated atomic [%u]\n", count);
            }
            count = mAllocFile.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated file [%u]\n", count);
            }
            count = mAllocMutex.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated mutex [%u]\n", count);
            }
#endif
        }

        void initialize(CString windowName, const VideoMode &videoMode) override {
            glfwInit();

            auto monitor = glfwGetPrimaryMonitor();
            Vec2f scale;
            glfwGetMonitorContentScale(monitor, &scale[0], &scale[1]);

            GlfwWindows::create(windowName, videoMode, scale);
            mInput.initialize(GlfwWindows::get(MAIN_WINDOW).handle);
        }

        void update() override {
            GlfwWindows::update();
            glfwPollEvents();
            mInput.update(); // Capture input data after pool events call
        }

        void finalize() override {
            glfwDestroyWindow(GlfwWindows::get(ISystem::MAIN_WINDOW).handle);
            glfwTerminate();
        }

        void onError(const char *message, uint64 line, const char *function, const char *file) override {
            const uint32 size = 2048;
            char buffer[size];
            snprintf(buffer, size, "Line: %llu Function: %s File: %s\nMessage: %s",
                    line, function, file, message);
            mDefaultLog.log(ELogVerbosity::Error, buffer);
        }

        ILog &getLog() override {
            return mDefaultLog;
        }

        IOutputDevice &getOutputDevice() override {
            return mDefaultOutput;
        }

        bool shouldClose(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.shouldClose;
        }

        bool isResizeable(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.resizeable;
        }

        bool isMinimized(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isMinimized;
        }

        bool isRestored(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isRestored;
        }

        bool isMoved(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isMoved;
        }

        bool isResized(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isResized;
        }

        Size2i getWindowPos(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.pos;
        }

        Size2i getWindowSize(WindowID id) override {
            auto& window = GlfwWindows::get(id);
            return window.size;
        }

        const CString &getWindowCaption(WindowID id) const override {
            auto& window = GlfwWindows::get(id);
            return window.caption;
        }

        Time getTime() const override {
            using namespace std::chrono;
            auto time = system_clock::to_time_t(system_clock::now());
            tm timeM{};
            localtime_r(&time, &timeM);

            Time result{};
            result.year = 1900 + timeM.tm_year;
            result.month = timeM.tm_mon;
            result.dayWeek = timeM.tm_wday;
            result.dayMonth = timeM.tm_mday - 1;
            result.dayYear = timeM.tm_yday;
            result.hour = timeM.tm_hour;
            result.min = timeM.tm_min;
            result.sec = timeM.tm_sec;
            return result;
        }

        Time getTime(const TimeValue &t) const override {
            using namespace std::chrono;

            int64 v = t.getRawValue();
            microseconds dur(v);
            time_point<system_clock> tp(dur);

            auto time = system_clock::to_time_t(tp);
            tm timeM{};
            localtime_r(&time, &timeM);

            Time result{};
            result.year = 1900 + timeM.tm_year;
            result.month = timeM.tm_mon;
            result.dayWeek = timeM.tm_wday;
            result.dayMonth = timeM.tm_mday - 1;
            result.dayYear = timeM.tm_yday;
            result.hour = timeM.tm_hour;
            result.min = timeM.tm_min;
            result.sec = timeM.tm_sec;
            return result;
        }

        TimeValue getTimeValue(const Time &t) const override {
            using namespace std::chrono;

            tm timeM{};
            timeM.tm_year = t.year - 1900;
            timeM.tm_mon = t.month;
            timeM.tm_mday = t.dayMonth + 1;
            timeM.tm_hour = t.hour;
            timeM.tm_min = t.min;
            timeM.tm_sec = t.sec;

            auto time = std::mktime(&timeM);
            auto tp = system_clock::from_time_t(time);
            auto mc = time_point_cast<microseconds>(tp);
            auto dur = mc.time_since_epoch();

            TimeValue r;
            r.getRawValue() = dur.count();
            return r;
        }

        IMutex& getErrorSyncMutex() override {
            return mErrorMutex;
        }

        TPtrUnique<IFile> openFile(CString path, EFileMode mode) override {
            static Function<void(void*)> dealloc = [](void* a){ ((macOS&)ISystem::getSingleton()).deallocateFile(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocFile.allocate(0);
            IFile* file = new (memory) StdFile(path, mode);
            return TPtrUnique<IFile>(file, &dealloc);
        }

        TPtrUnique<IDirectory> openDirectory(CString path) override {
            static Function<void(void*)> dealloc = [](void* a){ ((macOS&)ISystem::getSingleton()).deallocateDirectory(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocDirectory.allocate(0);
            IDirectory* directory = new (memory) UnixDirectory(path);
            return TPtrUnique<IDirectory>(directory, &dealloc);
        }

        TPtrUnique<IMutex> createMutex() override {
            static Function<void(void*)> dealloc = [](void* a){ ((macOS&)ISystem::getSingleton()).deallocateMutex(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocMutex.allocate(0);
            IMutex* mutex = new (memory) StdMutex();
            return TPtrUnique<IMutex>(mutex, &dealloc);
        }

        TPtrUnique<IAtomic> createAtomic() override {
            static Function<void(void*)> dealloc = [](void* a){ ((macOS&)ISystem::getSingleton()).deallocateAtomic(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocAtomic.allocate(0);
            IAtomic* atomic = new (memory) StdAtomic();
            return TPtrUnique<IAtomic>(atomic, &dealloc);
        }

    private:

        void deallocateFile(void* file) {
            Guard guard(mAccessMutex);
            mAllocFile.free(file);
        }

        void deallocateMutex(void* mutex) {
            Guard guard(mAccessMutex);
            mAllocMutex.free(mutex);
        }

        void deallocateAtomic(void* atomic) {
            Guard guard(mAccessMutex);
            mAllocAtomic.free(atomic);
        }

        void deallocateDirectory(void* directory) {
            Guard guard(mAccessMutex);
            mAllocDirectory.free(directory);
        }

    private:
        StdMutex mErrorMutex;
        StdMutex mAccessMutex;
        AllocPool mAllocFile;
        AllocPool mAllocMutex;
        AllocPool mAllocAtomic;
        AllocPool mAllocDirectory;
        GlfwInput mInput;
        LogStdout mDefaultLog;
        OutputDeviceStd mDefaultOutput;
    };

}

#endif //BERSERK_MACOS_H