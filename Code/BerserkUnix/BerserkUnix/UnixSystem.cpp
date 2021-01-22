/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixSystem.hpp>
#include <chrono>

namespace Berserk {
    namespace Platform {

        UnixSystem::UnixImpl::UnixImpl()
                : mAllocCalls(0), mDeallocCalls(0) {

            // At this point global memory ops are available
            Provide(this);

            // Strings pool setup
            mStringsPool = Create<PoolsAllocator>();
            // String table setup
            mStringTable = Create<UnixStringTable::UnixImpl>();
            // Set global locale across entire app
            mLocale = DEFAULT_LOCALE;
            std::setlocale(LC_ALL, mLocale.GetStr_C());

            // Console output setup
            if (mIsOutputPresented) {
                mConsoleOut = Create<UnixConsole>(stdout);
                mConsoleError = Create<UnixConsole>(stderr);
            }
            else {
                mConsoleOut = Create<LinuxConsoleDummy>();
                mConsoleError = Create<LinuxConsoleDummy>();
            }

            // Global logger
            mLogger = Create<Log>();

            // Setup foundation systems
            mFileSystem = Create<UnixFileSystem::UnixImpl>();
            mThreadManager = Create<UnixThreadManager::UnixImpl>();

            // Setup windows/input
            if (mIsGuiElementsProvided) {
                mGlfwContext = Create<GlfwContext>();
                // Initialized later by post rhi init: mRHIImpl = Create<RHI::GLDriver::GLImpl>();
            }
        }

        UnixSystem::UnixImpl::~UnixImpl() noexcept {
            // Release in reverse order
            if (mIsGuiElementsProvided) {
                Release(mRHIImpl);
                Release(mGlfwContext);
            }

            Release(mThreadManager);
            Release(mFileSystem);

            Release(mLogger);
            Release(mConsoleError);
            Release(mConsoleOut);

            Release(mStringTable);
            Release(mStringsPool);

            Remove(this);

        #ifdef BERSERK_DEBUG
            auto allocCalls = GetAllocateCallsCount();
            auto deallocCalls = GetDeallocateCallsCount();

            printf("Alloc calls=%llu, Dealloc calls=%llu\n", (unsigned long long)allocCalls, (unsigned long long) deallocCalls);
        #endif
        }

        void UnixSystem::UnixImpl::InitializeRHI() {
            if (mIsGuiElementsProvided) {
                mRHIImpl = Create<RHI::GLDriver::GLImpl>();
            }
        }

        void *UnixSystem::UnixImpl::Allocate(size_t sizeInBytes) {
            mAllocCalls.fetch_add(1);
            return malloc(sizeInBytes);
        }

        void * UnixSystem::UnixImpl::Reallocate(void *memory, size_t sizeInBytes) {
            if (memory != nullptr)
                mDeallocCalls.fetch_add(1);

            mAllocCalls.fetch_add(1);
            return realloc(memory, sizeInBytes);
        }

        void UnixSystem::UnixImpl::Deallocate(void *memory) {
            mDeallocCalls.fetch_add(1);
            free(memory);
        }

        uint64 UnixSystem::UnixImpl::GetAllocateCallsCount() const {
            return mAllocCalls.load();
        }

        uint64 UnixSystem::UnixImpl::GetDeallocateCallsCount() const {
            return mDeallocCalls.load();
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

        const Array<String> &UnixSystem::UnixImpl::GetCmdArgs() const {
            return mCmdArgs;
        }

        const String &UnixSystem::UnixImpl::GetLocale() const {
            return mLocale;
        }

        bool UnixSystem::UnixImpl::IsOutputPresented() const {
            return mIsOutputPresented;
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

        bool UnixSystem::UnixImpl::HasNativeGui() const {
            return mIsGuiElementsProvided;
        }

        void UnixSystem::UnixImpl::FixedUpdate() {
            if (mGlfwContext)
                mGlfwContext->Update();
        }

    }
}