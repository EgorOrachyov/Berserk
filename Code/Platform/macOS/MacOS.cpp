/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <macOS/MacOS.h>
#include <Path.h>
#include <Paths.h>
#include <Std/StdFile.h>
#include <Unix/UnixLibrary.h>
#include <Unix/UnixDirectory.h>
#include <GlfwSystem/GlfwInput.h>

#ifdef BERSERK_WITH_OPENGL
#include <GLDevice.h>
#endif

#ifdef BERSERK_WITH_WHEREAMI
#include <whereami.h>
#endif

namespace Berserk {

    MacOS MacOS::gMacOS;

    MacOS::MacOS() noexcept
        : System(),
          mRequestClose(false),
          mAllocFile(sizeof(StdFile)),
          mAllocDirectory(sizeof(UnixDirectory)) {

        std::setlocale(LC_ALL, "");
        extractExecutablePath();
        extractRootPath();

        mStdLog = TPtrUnique<LogStdout>::make();
        std::initializer_list<Log*> list = { mStdLog.getPtr() };
        mDefaultLog = TPtrUnique<LogComposite>::make(list);
    }

    MacOS::~MacOS() {
        if (!mFinalized) {
            finalize();
        }

        mDefaultLog.free();
        mFileLog.free();
        mStdLog.free();

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

    void MacOS::initialize(const CString &name, const CString &caption, Size2i size, bool forceVSync,
                           ERenderDeviceType deviceType,
                           CString logPath, bool logToFile) {
        BERSERK_COND_ERROR_FAIL(!mInitialized, "System already initialized");
        BERSERK_COND_ERROR_FAIL(deviceType == ERenderDeviceType::OpenGL, "Unsupported system rendering device");

        // Initialize file logger if needed
        if (logToFile) {
            CString logName = CString{"/Log_"} + getTime().toStringConservative() + ".txt";
            CString fullPath = logPath + logName;
            auto logFile = openFile(fullPath, EFileMode::Write);

            if (logFile.isNotNull() && logFile->isOpen()) {
                mFileLog = TPtrUnique<LogFile>::make(logFile);
                mDefaultLog->addLogger(mFileLog.getPtr());
            }
        }

        // Error handling on glfw side
        mManager.setErrorFunction([](const char* what){
            auto& log = System::getSingleton().getLog();
            log.logf(ELogType::Error, " GLFW Error: %s", what);
        });

        // Primary window and glfw init
        mManager.initializeForMacOS();
        auto w = mManager.createInternal(name, caption, size, EWindowActionOnClose::RequestSystemClose);
        w->makeRenderContextCurrent();

        // Force v-sync for all windows
        if (forceVSync)
            mManager.forceVSync();

        // Todo: capture input from an arbitrary window
        mInput.initialize(w->getWindowHandleGLFW());

#ifdef BERSERK_WITH_OPENGL
        // Initialize OpenGL RHI device - no arguments required
        mDevice = TPtrUnique<GLDevice>::make();
#endif // BERSERK_WITH_OPENGL
    }

    void MacOS::update() {
        // Reset input
        mInput.preUpdate();
        // Capture input data after pool events call
        mManager.pollEvents();
        // Dispatch events
        mInput.postUpdate();
    }

    void MacOS::finalize() {
        BERSERK_COND_ERROR_FAIL(!mFinalized, "System already finalized");
        BERSERK_COND_ERROR_FAIL(!mInitialized, "System is not initialized");

        mDevice.free();
        mManager.finalize();

        mFinalized = true;
    }

    void MacOS::requestClose() {
        mRequestClose.store(true);
    }

    bool MacOS::isCloseRequested() {
        return mRequestClose.load();
    }

    void MacOS::onError(const char *message, uint64 line, const char *function, const char *file) {
        const uint32 size = 2048;
        char buffer[size];
        snprintf(buffer, size, "Line: %llu Function: %s File: %s\nMessage: %s",
        line, function, file, message);
        mDefaultLog->log(ELogType::Error, buffer);
    }

    void MacOS::onWarning(const char *message, uint64 line, const char *function, const char *file) {
        const uint32 size = 2048;
        char buffer[size];
        snprintf(buffer, size, "Line: %llu Function: %s File: %s\nMessage: %s",
        line, function, file, message);
        mDefaultLog->log(ELogType::Warning, buffer);
    }

    System::Time MacOS::getTime() const {
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

    System::Time MacOS::getTime(const TimeValue &t) const {
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
    
    TimeValue MacOS::getTimeValue(const System::Time &t) const {
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

    TPtrUnique<File> MacOS::openFile(CString path, EFileMode mode) {
        static Function<void(void*)> dealloc = [](void* a){ ((MacOS&)System::getSingleton()).deallocateFile(a); };

        Guard guard(mAccessMutex);
        void* memory = mAllocFile.allocate(0);
        File* file = new (memory) StdFile(path, mode);
        return TPtrUnique<File>(file, &dealloc);
    }

    TPtrUnique<Directory> MacOS::openDirectory(CString path) {
        static Function<void(void*)> dealloc = [](void* a){ ((MacOS&)System::getSingleton()).deallocateDirectory(a); };

        Guard guard(mAccessMutex);
        void* memory = mAllocDirectory.allocate(0);
        Directory* directory = new (memory) UnixDirectory(path);
        return TPtrUnique<Directory>(directory, &dealloc);
    }

    TPtrUnique<Library> MacOS::openLibrary(Berserk::CString path) {
        return (TPtrUnique<Library>) TPtrUnique<UnixLibrary>::make(path);
    }

    void MacOS::extractExecutablePath() {
#ifdef BERSERK_WITH_WHEREAMI
        auto pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
        BERSERK_COND_ERROR_FAIL(pathLength > 0, "Failed to extract executable path length");

        mExecutablePath.ensureCapacity(pathLength + 1);
        auto result = wai_getExecutablePath(mExecutablePath.data(), pathLength, nullptr);
        BERSERK_COND_ERROR_FAIL(result == pathLength, "Failed to extract executable path");

        mExecutablePath.data()[pathLength] = '\0';
#endif
    }

    void MacOS::extractRootPath() {
        const char BERSERK_SEARCH_NAME[] = "Berserk";
        const char ENGINE_SECTION_NAME[] = "Engine";

        Path path = mExecutablePath;

        BERSERK_COND_ERROR_FAIL(path.entriesCount() >= 3, "Invalid engine executable path");
        BERSERK_COND_ERROR_FAIL(path.containsSection(BERSERK_SEARCH_NAME), "Invalid engine executable path");

        // ... / Berserk / Engine / Binary / Exec.exe
        //              n-3      n-2       n-1

        mExecutableName = path.last();
        auto index = path.findLastSection(BERSERK_SEARCH_NAME);
        auto prefix = path.firstSections(index + 1);

        mEnginePath = prefix.toString();
        mCachePath = mEnginePath + "/Engine/Cache";
        mConfigPath = mEnginePath + "/Engine/Config";
    }

    void MacOS::deallocateFile(void *file) {
        Guard guard(mAccessMutex);
        mAllocFile.free(file);
    }

    void MacOS::deallocateDirectory(void *directory) {
        Guard guard(mAccessMutex);
        mAllocDirectory.free(directory);
    }

}

