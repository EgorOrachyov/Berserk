/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxSystem.hpp>

namespace Berserk {
    namespace Platform {

        LinuxSystem::LinuxSystem()
                : mAllocCalls(0), mDeallocCalls(0) {

            // At this point global memory ops are available
            Platform::System::Provide(this);

            mFileSystem = Create<LinuxFileSystem::LinuxImpl>();
        }

        LinuxSystem::~LinuxSystem() noexcept {
            // Release in reverse order
            Release(mFileSystem);

            Platform::System::Remove(this);

        #ifdef BERSERK_DEBUG
            auto allocCalls = GetAllocateCallsCount();
            auto deallocCalls = GetDeallocateCallsCount();

            printf("Alloc calls=%llu, Dealloc calls=%llu\n", (unsigned long long)allocCalls, (unsigned long long) deallocCalls);
        #endif
        }

        void *LinuxSystem::Allocate(size_t sizeInBytes) {
            mAllocCalls.fetch_add(1);
            return malloc(sizeInBytes);
        }

        void LinuxSystem::Deallocate(void *memory) {
            mDeallocCalls.fetch_add(1);
            free(memory);
        }

        uint64 LinuxSystem::GetAllocateCallsCount() const {
            return mAllocCalls.load();
        }

        uint64 LinuxSystem::GetDeallocateCallsCount() const {
            return mDeallocCalls.load();
        }

        void *LinuxSystem::AllocateStringBuffer(size_t sizeInBytes) {
            return Allocate(sizeInBytes);
        }

        void LinuxSystem::DeallocateStringBuffer(void *buffer) {
            Deallocate(buffer);
        }

        void *LinuxSystem::AllocatePtrMeta(size_t sizeInBytes) {
            return Allocate(sizeInBytes);
        }

        void LinuxSystem::DeallocatePtrMeta(void *buffer) {
            Deallocate(buffer);
        }

    }
}