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

            mFileSystem = Create<LinuxFileSystem::LinuxImpl>();
        }

        LinuxSystem::LinuxImpl::~LinuxImpl() noexcept {
            // Release in reverse order
            Release(mFileSystem);

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
            return Allocate(sizeInBytes);
        }

        void LinuxSystem::LinuxImpl::DeallocateStringBuffer(void *buffer) {
            Deallocate(buffer);
        }

        void *LinuxSystem::LinuxImpl::AllocatePtrMeta(size_t sizeInBytes) {
            return Allocate(sizeInBytes);
        }

        void LinuxSystem::LinuxImpl::DeallocatePtrMeta(void *buffer) {
            Deallocate(buffer);
        }

        void QueryTimeStruct(TimeType type, std::tm& timeStruct) {
            using namespace std::chrono;
            time_t systemTime = system_clock::to_time_t(system_clock::now());

            if (type == TimeType::Local) {
                localtime_r(&systemTime, &timeStruct);
            }
            else {
                gmtime_r(&systemTime, &timeStruct);
            }
        }

        Date LinuxSystem::LinuxImpl::GetDate(TimeType type) {
            std::tm timeStruct{};
            QueryTimeStruct(type, timeStruct);

            auto year = (uint32)(1900u + (uint32) timeStruct.tm_year);
            auto day = (uint32) timeStruct.tm_yday;
            Date::Month month = Date::GetMonth(timeStruct.tm_mon);
            Date::Weekday weekday = Date::GetWeekday(timeStruct.tm_wday);

            return Date(weekday, month, day, year);
        }

        Time LinuxSystem::LinuxImpl::GetTime(TimeType type) {
            std::tm timeStruct{};
            QueryTimeStruct(type, timeStruct);

            auto hour = (uint32) timeStruct.tm_hour;
            auto min = (uint32) timeStruct.tm_min;
            auto sec = (uint32) timeStruct.tm_sec;

            return Time(hour, min, sec);
        }

    }
}