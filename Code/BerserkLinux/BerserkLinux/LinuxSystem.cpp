/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxSystem.hpp>
#include <chrono>

namespace Berserk {
    namespace Platform {

        LinuxSystem::LinuxImpl::LinuxImpl()
                : mAllocCalls(0), mDeallocCalls(0) {

            // At this point global memory ops are available
            Provide(this);

            // Strings pool setup
            mStringsPool = Create<PoolsAllocator>();
            // String table setup
            mStringTable = Create<LinuxStringTable::LinuxImpl>();
            // Set global locale across entire app
            mLocale = DEFAULT_LOCALE;
            std::setlocale(LC_ALL, mLocale.GetStr_C());

            // Console output setup
            if (mIsOutputPresented) {
                mConsoleOut = Create<LinuxConsole>(stdout);
                mConsoleError = Create<LinuxConsole>(stderr);
            }
            else {
                mConsoleOut = Create<LinuxConsoleDummy>();
                mConsoleError = Create<LinuxConsoleDummy>();
            }

            // Global logger
            mLogger = Create<Log>();

            // Setup foundation systems
            mFileSystem = Create<LinuxFileSystem::LinuxImpl>();
            mThreadManager = Create<LinuxThreadManager::LinuxImpl>();

            // Setup windows/input
            if (mIsGuiElementsProvided) {
                mGlfwContext = Create<GlfwContext>();
            }
        }

        LinuxSystem::LinuxImpl::~LinuxImpl() noexcept {
            // Release in reverse order
            Release(mGlfwContext);
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

        void *LinuxSystem::LinuxImpl::Allocate(size_t sizeInBytes) {
            mAllocCalls.fetch_add(1);
            return malloc(sizeInBytes);
        }

        void LinuxSystem::LinuxImpl::Deallocate(void *memory) {
            mDeallocCalls.fetch_add(1);
            free(memory);
        }

        uint64 LinuxSystem::LinuxImpl::GetAllocateCallsCount() const {
            return mAllocCalls.load();
        }

        uint64 LinuxSystem::LinuxImpl::GetDeallocateCallsCount() const {
            return mDeallocCalls.load();
        }

        void *LinuxSystem::LinuxImpl::AllocateStringBuffer(size_t sizeInBytes) {
            return mStringsPool->Allocate(sizeInBytes);
        }

        void LinuxSystem::LinuxImpl::DeallocateStringBuffer(void *buffer, size_t sizeInBytes) {
            mStringsPool->Deallocate(buffer, sizeInBytes);
        }

        void *LinuxSystem::LinuxImpl::AllocatePtrMeta(size_t sizeInBytes) {
            return Allocate(sizeInBytes);
        }

        void LinuxSystem::LinuxImpl::DeallocatePtrMeta(void *buffer) {
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

        Date LinuxSystem::LinuxImpl::GetDate(TimeType type) {
            std::tm timeStruct{};
            QueryTime(type, timeStruct);

            auto year = (uint32)(1900u + (uint32) timeStruct.tm_year);
            auto dayYear = (uint32) timeStruct.tm_yday;
            auto dayMonth = (uint32) timeStruct.tm_mday;
            Date::Month month = Date::GetMonth(timeStruct.tm_mon);
            Date::Weekday weekday = Date::GetWeekday(timeStruct.tm_wday);

            return Date(weekday, month, dayYear, dayMonth, year);
        }

        Time LinuxSystem::LinuxImpl::GetTime(TimeType type) {
            std::tm timeStruct{};
            QueryTime(type, timeStruct);

            auto hour = (uint32) timeStruct.tm_hour;
            auto min = (uint32) timeStruct.tm_min;
            auto sec = (uint32) timeStruct.tm_sec;

            return Time(hour, min, sec);
        }

        TimeStamp LinuxSystem::LinuxImpl::GetTimeStamp() {
            using namespace std::chrono;
            time_t systemTime = system_clock::to_time_t(system_clock::now());

            return TimeStamp((uint64) systemTime);
        }

        void LinuxSystem::LinuxImpl::GetDateTime(TimeStamp timeStamp, Date &date, Time &time, TimeType timeType) {
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

        const Array<String> &LinuxSystem::LinuxImpl::GetCmdArgs() const {
            return mCmdArgs;
        }

        const String &LinuxSystem::LinuxImpl::GetLocale() const {
            return mLocale;
        }

        bool LinuxSystem::LinuxImpl::IsOutputPresented() const {
            return mIsOutputPresented;
        }

        TextWriter &LinuxSystem::LinuxImpl::GetOutStream() {
            return *mConsoleOut;
        }

        TextWriter &LinuxSystem::LinuxImpl::GetErrorStream() {
            return *mConsoleError;
        }

        Log &LinuxSystem::LinuxImpl::GetLogger() {
            return *mLogger;
        }

        void LinuxSystem::LinuxImpl::Abort() {
            std::abort();
        }

        bool LinuxSystem::LinuxImpl::HasNativeGui() const {
            return mIsGuiElementsProvided;
        }

        void LinuxSystem::LinuxImpl::FixedUpdate() {
            if (mGlfwContext)
                mGlfwContext->Update();
        }

    }
}