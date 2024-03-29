/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <BerserkPlatform/Unix/UnixSystem.hpp>
#include <chrono>
#include <clocale>
#include <ctime>

namespace Berserk {

    UnixSystem::UnixImpl::UnixImpl() {
        // At this point global memory ops are available
        Provide(this);

        // Strings pool setup
        mStringsPool = Memory::Make<PoolsAllocator>();
        // String table setup
        mStringTable = Memory::Make<PlatformStringTable::PImpl>();
        // Set global locale across entire app
        mLocale = DEFAULT_LOCALE;
        std::setlocale(LC_ALL, mLocale.GetStr_C());

        mAppName = BERSERK_TEXT("Berserk Application");
        mEngineName = BERSERK_TEXT("Berserk Engine");

        // Console output setup
        if (mStdoutEnabled)
            mConsoleOut = Memory::Make<UnixOut>(stdout);
        else
            mConsoleOut = Memory::Make<PlatformOutDummy>();

        if (mStderrEnabled)
            mConsoleError = Memory::Make<UnixOut>(stderr);
        else
            mConsoleError = Memory::Make<PlatformOutDummy>();

        // Global logger
        mLogger = Memory::Make<Log>();

        // Setup foundation systems
        mFileSystem = Memory::Make<UnixFileSystem::UnixImpl>();
        mThreadManager = Memory::Make<PlatformThreadManager::PImpl>();
        mTaskManager = Memory::Make<PlatformTaskManager::PImpl>(TASK_MAN_WORKERS_COUNT);

        // Setup windows/input
        if (mWindowsEnabled) {
            mDialogs = Memory::Make<PlatformDialogs::PImpl>();
            mGlfwContext = Memory::Make<GlfwContext>(mUseVsync, mGlfwNoClientApi);
            // Initialized later by post rhi init: mRHIImpl = Memory::Make<RHI::GLDriver::GLImpl>();
        }
    }

    UnixSystem::UnixImpl::~UnixImpl() noexcept {
        // Release in reverse order
        if (mWindowsEnabled) {
#ifdef BERSERK_WITH_OPENGL
            if (mGlRHIImpl)
                Memory::Release(mGlRHIImpl);
#endif
#ifdef BERSERK_WITH_VULKAN
            if (mVkRHIImpl)
                Memory::Release(mVkRHIImpl);
#endif
            Memory::Release(mGlfwContext);
            Memory::Release(mDialogs);
        }

        Memory::Release(mTaskManager);
        Memory::Release(mThreadManager);
        Memory::Release(mFileSystem);

        Memory::Release(mLogger);
        Memory::Release(mConsoleError);
        Memory::Release(mConsoleOut);

        Memory::Release(mStringTable);
        Memory::Release(mStringsPool);

        Remove(this);

    #ifdef BERSERK_DEBUG
        auto allocCalls = GetAllocateCallsCount();
        auto deallocCalls = GetDeallocateCallsCount();

        printf("Alloc calls=%llu, Dealloc calls=%llu\n", (unsigned long long)allocCalls, (unsigned long long) deallocCalls);
    #endif
    }

    void UnixSystem::UnixImpl::InitializeRHI() {
        if (mWindowsEnabled) {
            bool initialized = false;

#ifdef BERSERK_WITH_VULKAN
            if (mGlfwNoClientApi && !initialized) {
                RHI::VulkanDeviceInitInfo initStruct;
                initStruct.applicationName = mAppName;
                initStruct.engineName = mEngineName;
                initStruct.requiredExtensions = std::move(mGlfwContext->GetRequiredInstanceExt());
                initStruct.primaryWindow = mGlfwContext->GetPrimaryWindow();
                initStruct.clientSurfaceFactory = std::move(mGlfwContext->GetSurfaceCreationFunction());

                mVkRHIImpl = Memory::Make<RHI::VulkanDriver::VkImpl>(std::move(initStruct));
                initialized = mVkRHIImpl->IsInitialized();

                if (!initialized) {
                    Memory::Release(mVkRHIImpl);
                    mVkRHIImpl = nullptr;
                }
            }
#endif

#ifdef BERSERK_WITH_OPENGL
            if (!mGlfwNoClientApi && !initialized) {
                mGlRHIImpl = Memory::Make<RHI::GLDriver::GLImpl>();
                initialized = mGlRHIImpl->IsInitialized();

                if (!initialized) {
                    Memory::Release(mGlRHIImpl);
                    mGlRHIImpl = nullptr;
                }
            }
#endif
        }
    }

    void *UnixSystem::UnixImpl::Allocate(uint64 sizeInBytes) {
        mAllocCalls.fetch_add(1);
        return std::malloc(sizeInBytes);
    }

    void * UnixSystem::UnixImpl::Reallocate(void *memory, uint64 sizeInBytes) {
        if (memory != nullptr)
            mDeallocCalls.fetch_add(1);

        mAllocCalls.fetch_add(1);
        return std::realloc(memory, sizeInBytes);
    }

    void UnixSystem::UnixImpl::Deallocate(void *memory) {
        if (!memory)
            return;

        mDeallocCalls.fetch_add(1);
        std::free(memory);
    }

    void *UnixSystem::UnixImpl::AllocateStringBuffer(uint64 sizeInBytes) {
        return mStringsPool->Allocate(sizeInBytes);
    }

    void UnixSystem::UnixImpl::DeallocateStringBuffer(void *buffer, uint64 sizeInBytes) {
        mStringsPool->Deallocate(buffer, sizeInBytes);
    }

    void *UnixSystem::UnixImpl::AllocatePtrMeta(uint64 sizeInBytes) {
        return Allocate(sizeInBytes);
    }

    void UnixSystem::UnixImpl::DeallocatePtrMeta(void *buffer) {
        Deallocate(buffer);
    }

    void QueryTimeStruct(std::time_t systemTime, TimeType type, std::tm& timeStruct) {
        if (type == TimeType::Local) {
            localtime_r(&systemTime, &timeStruct);
        }
        else {
            gmtime_r(&systemTime, &timeStruct);
        }
    }

    void QueryTime(TimeType type, std::tm& timeStruct) {
        using namespace std::chrono;
        time_t systemTime = system_clock::to_time_t(system_clock::now());

        QueryTimeStruct(systemTime, type, timeStruct);
    }

    Date UnixSystem::UnixImpl::GetDate(TimeType type) {
        std::tm timeStruct{};
        QueryTime(type, timeStruct);

        auto year = (uint32)(1900u + (uint32) timeStruct.tm_year);
        auto dayYear = (uint32) timeStruct.tm_yday;
        auto dayMonth = (uint32) timeStruct.tm_mday;
        Date::Month month = Date::GetMonth(timeStruct.tm_mon);
        Date::Weekday weekday = Date::GetWeekday(timeStruct.tm_wday);

        return Date(weekday, month, dayYear, dayMonth, year);
    }

    Time UnixSystem::UnixImpl::GetTime(TimeType type) {
        std::tm timeStruct{};
        QueryTime(type, timeStruct);

        auto hour = (uint32) timeStruct.tm_hour;
        auto min = (uint32) timeStruct.tm_min;
        auto sec = (uint32) timeStruct.tm_sec;

        return Time(hour, min, sec);
    }

    TimeStamp UnixSystem::UnixImpl::GetTimeStamp() {
        using namespace std::chrono;
        time_t systemTime = system_clock::to_time_t(system_clock::now());

        return TimeStamp((uint64) systemTime);
    }

    void UnixSystem::UnixImpl::GetDateTime(TimeStamp timeStamp, Date &date, Time &time, TimeType timeType) {
        std::tm timeStruct{};
        QueryTimeStruct((std::time_t) timeStamp.native, timeType, timeStruct);

        auto year = (uint32)(1900u + (uint32) timeStruct.tm_year);
        auto dayYear = (uint32) timeStruct.tm_yday;
        auto dayMonth = (uint32) timeStruct.tm_mday;
        Date::Month month = Date::GetMonth(timeStruct.tm_mon);
        Date::Weekday weekday = Date::GetWeekday(timeStruct.tm_wday);

        auto hour = (uint32) timeStruct.tm_hour;
        auto min = (uint32) timeStruct.tm_min;
        auto sec = (uint32) timeStruct.tm_sec;

        date = Date(weekday, month, dayYear, dayMonth, year);
        time = Time(hour, min, sec);
    }

    TextWriter &UnixSystem::UnixImpl::GetOutStream() {
        return *mConsoleOut;
    }

    TextWriter &UnixSystem::UnixImpl::GetErrorStream() {
        return *mConsoleError;
    }

    Log &UnixSystem::UnixImpl::GetLogger() {
        return *mLogger;
    }

    void UnixSystem::UnixImpl::Abort() {
        std::abort();
    }

    void UnixSystem::UnixImpl::FixedUpdate() {
        if (mGlfwContext)
            mGlfwContext->Update();
#ifdef BERSERK_WITH_OPENGL
        if (mGlRHIImpl)
            mGlRHIImpl->FixedUpdate();
#endif
#ifdef BERSERK_WITH_VULKAN
        if (mVkRHIImpl)
            mVkRHIImpl->FixedUpdate();
#endif
    }
}