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
#include <Std/StdFile.h>
#include <Unix/UnixDirectory.h>
#include <GlfwSystem/GlfwInput.h>
#include <GlfwSystem/GlfwWindow.h>
#include <AllocPool.h>
#include <IO/Logs.h>

#include <clocale>
#include <time.h>

namespace Berserk {

    class macOS final : public ISystem {
    public:

        macOS() noexcept
            : ISystem(),
              mAllocFile(sizeof(StdFile)),
              mAllocDirectory(sizeof(UnixDirectory)) {
            std::setlocale(LC_ALL, "");
        }

        ~macOS() override {
#ifdef BERSERK_DEBUG
            uint32 count;
            count = mAllocFile.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost allocated file [%u]\n", count);
            }
#endif
        }

        void vinitialize(CString windowName, const VideoMode &videoMode, ERenderDeviceType deviceType) override {
            BERSERK_COND_ERROR_FAIL(!mInitialized, "System already initialized");

            glfwSetErrorCallback(glfwErrorCallback);
            auto result = glfwInit();
            BERSERK_COND_ERROR_FAIL(result, "Failed to initialize GLFW");

            if (deviceType == ERenderDeviceType::OpenGL) {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
            }
            else {
                BERSERK_ERROR_FAIL("Unsupported system rendering device");
            }

            Vec2f scale;
            auto monitor = glfwGetPrimaryMonitor();
            glfwGetMonitorContentScale(monitor, &scale[0], &scale[1]);

            GlfwWindows::create(windowName, videoMode, scale);

            if (videoMode.forceVSync)
                glfwSwapInterval(1);

            mInput.initialize(GlfwWindows::get(MAIN_WINDOW).handle);
        }

        void vupdate() override {
            // Reset windows states
            GlfwWindows::reset();
            // Reset input
            mInput.reset();
            // Capture input data after pool events call
            glfwPollEvents();
            // Swap buffers handle
            GlfwWindows::update();
            // Dispatch events
            mInput.update();
        }

        void vfinalize() override {
            BERSERK_COND_ERROR_FAIL(!mInitialized, "System already finalized");

            mInput.finalize();
            GlfwWindows::destroy();
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

        Function<void(unsigned int)> &getWindowBindFunction() override {
            static Function<void(unsigned int)> bind = [](WINDOW_ID id) {
                auto& window = GlfwWindows::get(id);
                glfwMakeContextCurrent(window.handle);
            };

            return bind;
        }

        bool shouldClose(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.shouldClose;
        }

        bool isResizeable(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.resizeable;
        }

        bool isMinimized(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isMinimized;
        }

        bool isRestored(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isRestored;
        }

        bool isMoved(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isMoved;
        }

        bool isResized(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.isResized;
        }

        Size2i getWindowPos(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.pos;
        }

        Size2i getWindowSize(WINDOW_ID id) override {
            auto& window = GlfwWindows::get(id);
            return window.size;
        }

        const CString &getWindowCaption(WINDOW_ID id) const override {
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

        static void glfwErrorCallback(int error_code, const char* description) {
            auto& log = ISystem::getSingleton().getLog();
            log.logf(ELogVerbosity::Error, " GLFW Error: %s", description);
        }

    private:

        void deallocateFile(void* file) {
            Guard guard(mAccessMutex);
            mAllocFile.free(file);
        }

        void deallocateDirectory(void* directory) {
            Guard guard(mAccessMutex);
            mAllocDirectory.free(directory);
        }

    private:
        bool mInitialized = false;
        bool mFinalized = false;

        Mutex mAccessMutex;
        AllocPool mAllocFile;
        AllocPool mAllocDirectory;
        GlfwInput mInput;
        LogStdout mDefaultLog;
        OutputDeviceStd mDefaultOutput;
    };

}

#endif //BERSERK_MACOS_H