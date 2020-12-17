/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxSystem.hpp>

namespace Berserk {
    namespace Linux {

        LinuxSystem::LinuxSystem()
            : mAllocCalls(0), mDeallocCalls(0) {

            Platform::System::Provide(this);
        }

        LinuxSystem::~LinuxSystem() noexcept {
            Platform::System::Remove(this);
        }

        void LinuxSystem::Initialize() {

        }

        void LinuxSystem::Finalize() {}

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

    }
}
