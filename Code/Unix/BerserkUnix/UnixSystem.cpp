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

#include <BerserkUnix/UnixSystem.hpp>
#include <chrono>
#include <clocale>

namespace Berserk {

    UnixSystem::UnixImpl::UnixImpl() {
        // At this point global memory ops are available
        Provide(this);

        // Strings pool setup
        mStringsPool = Memory::Make<PoolsAllocator>();
        // String table setup
        mStringTable = Memory::Make<UnixStringTable::UnixImpl>();
        // Set global locale across entire app
        mLocale = DEFAULT_LOCALE;
        std::setlocale(LC_ALL, mLocale.GetStr_C());

        // Console output setup
        if (mStdoutEnabled)
            mConsoleOut = Memory::Make<UnixConsole>(stdout);
        else
            mConsoleOut = Memory::Make<LinuxConsoleDummy>();

        if (mStderrEnabled)
            mConsoleError = Memory::Make<UnixConsole>(stderr);
        else
            mConsoleError = Memory::Make<LinuxConsoleDummy>();

        // Global logger
        mLogger = Memory::Make<Log>();

        // Setup foundation systems
        mFileSystem = Memory::Make<UnixFileSystem::UnixImpl>();
        mThreadManager = Memory::Make<UnixThreadManager::UnixImpl>();

        // Setup windows/input
        if (mWindowsEnabled) {
            mDialogs = Memory::Make<UnixDialogs::UnixImpl>();
            mGlfwContext = Memory::Make<GlfwContext>(mUseVsync);
            // Initialized later by post rhi init: mRHIImpl = Memory::Make<RHI::GLDriver::GLImpl>();
        }
    }

    UnixSystem::UnixImpl::~UnixImpl() noexcept {
        // Release in reverse order
        if (mWindowsEnabled) {
#ifdef BERSERK_WITH_OPENGL
            if (mRHIImpl)
                Memory::Release(mRHIImpl);
#endif
            Memory::Release(mGlfwContext);
            Memory::Release(mDialogs);
        }

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
#ifdef BERSERK_WITH_OPENGL
            mRHIImpl = Memory::Make<RHI::GLDriver::GLImpl>();
#endif //BERSERK_WITH_OPENGL
        }
    }

    void *UnixSystem::UnixImpl::Allocate(size_t sizeInBytes) {
        mAllocCalls.fetch_add(1);
        return std::malloc(sizeInBytes);
    }

    void * UnixSystem::UnixImpl::Reallocate(void *memory, size_t sizeInBytes) {
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

    void *UnixSystem::UnixImpl::AllocateStringBuffer(size_t sizeInBytes) {
        return mStringsPool->Allocate(sizeInBytes);
    }

    void UnixSystem::UnixImpl::DeallocateStringBuffer(void *buffer, size_t sizeInBytes) {
        mStringsPool->Deallocate(buffer, sizeInBytes);
    }

    void *UnixSystem::UnixImpl::AllocatePtrMeta(size_t sizeInBytes) {
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
        if (mRHIImpl)
            mRHIImpl->FixedUpdate();
#endif
    }
}