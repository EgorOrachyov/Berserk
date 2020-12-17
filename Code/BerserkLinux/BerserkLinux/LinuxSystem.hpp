/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUXSYSTEM_HPP
#define BERSERK_LINUXSYSTEM_HPP

#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Atomic.hpp>

namespace Berserk {
    namespace Linux {

        class LinuxSystem: public Platform::System {
        public:
            LinuxSystem();
            ~LinuxSystem() override;

            void Initialize();
            void Finalize();

        protected:

            void *Allocate(size_t sizeInBytes) override;
            void Deallocate(void *memory) override;

            uint64 GetAllocateCallsCount() const override;
            uint64 GetDeallocateCallsCount() const override;

            void *AllocateStringBuffer(size_t sizeInBytes) override;
            void DeallocateStringBuffer(void *buffer) override;

        private:
            Platform::AtomicUint64 mAllocCalls;
            Platform::AtomicUint64 mDeallocCalls;

            volatile uint32 mExitCode = 0;
        };

    }
}

#endif //BERSERK_LINUXSYSTEM_HPP