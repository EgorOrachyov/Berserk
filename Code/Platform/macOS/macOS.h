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
#include <Std/StdAtomic.h>
#include <Std/StdFile.h>
#include <Std/StdMutex.h>
#include <GlfwSystem/GlfwInput.h>
#include <GlfwSystem/GlfwWindow.h>

#include <AllocPool.h>
#include <chrono>
#include <time.h>

namespace Berserk {

    class macOS final : public System {
    public:

        macOS() noexcept
            : System(),
              mAllocFile(sizeof(StdFile)),
              mAllocMutex(sizeof(StdMutex)),
              mAllocAtomic(sizeof(StdAtomic)) {
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
            float32 scaleX = 0, scaleY = 0;
            glfwGetMonitorContentScale(monitor, &scaleX, &scaleY);

            auto& window = mWindows.emplace();
            window.caption = windowName;
            window.scaleX = scaleX;
            window.scaleY = scaleY;
            window.size[0] = (int)((float32)videoMode.width / scaleX);
            window.size[1] = (int)((float32)videoMode.height / scaleX);
            window.resizeable = videoMode.resizeable;
            window.maximised = videoMode.maximised;

            GlfwWindow::create(window);
            mInput.initialize(window.handle);
        }

        void update() override {
            glfwPollEvents();
            for (auto& window: mWindows) {
                GlfwWindow::update(window);
            }
            // Capture input data after pool events call
            mInput.update();
        }

        void finalize() override {
            glfwDestroyWindow(mWindows[0].handle);
            glfwTerminate();
        }

        void onError(const char *message, uint64 line, const char *function, const char *file) override {
            const uint32 size = 2048;
            char buffer[size];
            snprintf(buffer, size, "Line: %llu Function: %s File: %s\nMessage: %s\n",
                    line, function, file, message);
            mDefaultLog.log(ELogVerbosity::Error, buffer);
        }

        Log &getLog() override {
            return mDefaultLog;
        }

        bool shouldClose(WindowID id) override {
            auto& window = mWindows[id];
            return window.shouldClose;
        }

        bool isResizeable(WindowID id) override {
            auto& window = mWindows[id];
            return window.resizeable;
        }

        bool isFullscreen(WindowID id) override {
            return false;
        }

        Size2i getWindowPos(WindowID id) override {
            auto& window = mWindows[id];
            return window.pos;
        }

        Size2i getWindowSize(WindowID id) override {
            auto& window = mWindows[id];
            return window.sizeFbo;
        }

        CString getWindowCaption(WindowID id) const override {
            auto& window = mWindows[id];
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
            result.day = timeM.tm_wday;
            result.dayYear = timeM.tm_yday;
            result.hour = timeM.tm_hour;
            result.min = timeM.tm_min;
            result.sec = timeM.tm_sec;
            return result;
        }

        Mutex& getErrorSyncMutex() override {
            return mErrorMutex;
        }

        TPtrUnique<File> openFile(CString path, EFileMode mode) override {
            static Function<void(File*)> dealloc = [](File* a){ ((macOS&)System::getSingleton()).deallocateFile(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocFile.allocate(0);
            File* file = new (memory) StdFile(path, mode);
            return TPtrUnique<File>(file, &dealloc);
        }

        TPtrUnique<Mutex> createMutex() override {
            static Function<void(Mutex*)> dealloc = [](Mutex* a){ ((macOS&)System::getSingleton()).deallocateMutex(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocMutex.allocate(0);
            Mutex* mutex = new (memory) StdMutex();
            return TPtrUnique<Mutex>(mutex, &dealloc);
        }

        TPtrUnique<Atomic> createAtomic() override {
            static Function<void(Atomic*)> dealloc = [](Atomic* a){ ((macOS&)System::getSingleton()).deallocateAtomic(a); };

            Guard guard(mAccessMutex);
            void* memory = mAllocAtomic.allocate(0);
            Atomic* atomic = new (memory) StdAtomic();
            return TPtrUnique<Atomic>(atomic, &dealloc);
        }

    private:

        void deallocateFile(File* file) {
            Guard guard(mAccessMutex);
            mAllocFile.free(file);
        }

        void deallocateMutex(Mutex* mutex) {
            Guard guard(mAccessMutex);
            mAllocMutex.free(mutex);
        }

        void deallocateAtomic(Atomic* atomic) {
            Guard guard(mAccessMutex);
            mAllocAtomic.free(atomic);
        }

    private:
        StdMutex mErrorMutex;
        StdMutex mAccessMutex;
        AllocPool mAllocFile;
        AllocPool mAllocMutex;
        AllocPool mAllocAtomic;
        LogStdout mDefaultLog;
        GlfwInput mInput;
        TArray<GlfwWindow> mWindows;
    };

}

#endif //BERSERK_MACOS_H