/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <macOS/macOS.h>
#include <Path.h>
#include <Std/StdFile.h>
#include <Unix/UnixLibrary.h>
#include <Unix/UnixDirectory.h>
#include <GlfwSystem/GlfwInput.h>

#ifdef BERSERK_WITH_WHEREAMI
    #include <whereami.h>
#endif

namespace Berserk {

    macOS macOS::gMacOS;

    macOS::macOS() noexcept
        : System(),
          mAllocFile(sizeof(StdFile)),
          mAllocDirectory(sizeof(UnixDirectory)) {

        std::setlocale(LC_ALL, "");
        extractExecutablePath();
    }

    macOS::~macOS() {
#ifdef BERSERK_DEBUG
        uint32 count;

        count = mAllocFile.getChunksAllocated();
        if (count != 0) {
            fprintf(stderr, "[Berserk Core] Lost allocated file [%u]\n", count);
        }

        count = mAllocDirectory.getChunksAllocated();
        if (count != 0) {
            fprintf(stderr, "[Berserk Core] Lost allocated directory [%u]\n", count);
        }
#endif
}

    void macOS::initialize(const CString &name, const CString &caption, Size2i size,
                           bool forceVSync, ERenderDeviceType deviceType) {
        BERSERK_COND_ERROR_FAIL(!mInitialized, "System already initialized");
        BERSERK_COND_ERROR_FAIL(deviceType == ERenderDeviceType::OpenGL, "Unsupported system rendering device");

        mManager.setErrorFunction([](const char* what){
            auto& log = System::getSingleton().getLog();
            log.logf(ELogVerbosity::Error, " GLFW Error: %s", what);
        });

        mManager.initializeForMacOS();
        auto w = mManager.createInternal(name, caption, size);
        w->makeRenderContextCurrent();

        if (forceVSync)
            mManager.forceVSync();

        // Todo: capture input from an arbitrary window
        mInput.initialize(w->getWindowHandleGLFW());

#ifdef BERSERK_WITH_OPENGL
        // Initialize OpenGL RHI device - no arguments required
        mDevice = TPtrUnique<GLDevice>::make();
#endif // BERSERK_WITH_OPENGL
    }

    void macOS::update() {
        // Reset input
        mInput.reset();
        // Capture input data after pool events call
        mManager.pollEvents();
        // Dispatch events
        mInput.update();
    }

    void macOS::finalize() {
        BERSERK_COND_ERROR_FAIL(!mInitialized, "System already finalized");

        mDevice.free();
        mInput.finalize();
        mManager.finalize();

        mFinalized = true;
    }

    void macOS::onError(const char *message, uint64 line, const char *function, const char *file) {
        const uint32 size = 2048;
        char buffer[size];
        snprintf(buffer, size, "Line: %llu Function: %s File: %s\nMessage: %s",
        line, function, file, message);
        mDefaultLog.log(ELogVerbosity::Error, buffer);
    }

    void macOS::onWarning(const char *message, uint64 line, const char *function, const char *file) {
        const uint32 size = 2048;
        char buffer[size];
        snprintf(buffer, size, "Line: %llu Function: %s File: %s\nMessage: %s",
        line, function, file, message);
        mDefaultLog.log(ELogVerbosity::Warning, buffer);
    }

    System::Time macOS::getTime() const {
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

    System::Time macOS::getTime(const TimeValue &t) const {
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
    
    TimeValue macOS::getTimeValue(const System::Time &t) const {
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

    TPtrUnique<File> macOS::openFile(CString path, EFileMode mode) {
        static Function<void(void*)> dealloc = [](void* a){ ((macOS&)System::getSingleton()).deallocateFile(a); };

        Guard guard(mAccessMutex);
        void* memory = mAllocFile.allocate(0);
        File* file = new (memory) StdFile(path, mode);
        return TPtrUnique<File>(file, &dealloc);
    }

    TPtrUnique<Directory> macOS::openDirectory(CString path) {
        static Function<void(void*)> dealloc = [](void* a){ ((macOS&)System::getSingleton()).deallocateDirectory(a); };

        Guard guard(mAccessMutex);
        void* memory = mAllocDirectory.allocate(0);
        Directory* directory = new (memory) UnixDirectory(path);
        return TPtrUnique<Directory>(directory, &dealloc);
    }

    TPtrUnique<Library> macOS::openLibrary(Berserk::CString path) {
        return (TPtrUnique<Library>) TPtrUnique<UnixLibrary>::make(path);
    }

    void macOS::extractExecutablePath() {
#ifdef BERSERK_WITH_WHEREAMI
        auto pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
        BERSERK_COND_ERROR_FAIL(pathLength > 0, "Failed to extract executable path length");

        mExecutablePath.ensureCapacity(pathLength + 1);
        auto result = wai_getExecutablePath(mExecutablePath.data(), pathLength, nullptr);
        BERSERK_COND_ERROR_FAIL(result == pathLength, "Failed to extract executable path");

        mExecutablePath.data()[pathLength] = '\0';
#endif
    }

    void macOS::extractEnginePath() {
        Path path = mExecutablePath;

        BERSERK_COND_ERROR_FAIL(path.entriesCount() >= 3, "Invalid engine executable path");

        // ... / ... / Engine / Binary / Exec.exe
        //              n-3      n-2       n-1

        auto entries = path.entriesCount();
        auto exec = entries - 1;
        auto bin = exec - 1;
        auto engine = bin - 1;

        mExecutableName = path.last();
    }

    void macOS::deallocateFile(void *file) {
        Guard guard(mAccessMutex);
        mAllocFile.free(file);
    }

    void macOS::deallocateDirectory(void *directory) {
        Guard guard(mAccessMutex);
        mAllocDirectory.free(directory);
    }

}

