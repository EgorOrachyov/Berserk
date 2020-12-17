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
#include <BerserkLinux/LinuxFileSystem.hpp>

namespace Berserk {
    namespace Platform {

        class LinuxSystem : public System {
        public:
            LinuxSystem();
            ~LinuxSystem() override;

        protected:

            void *Allocate(size_t sizeInBytes) override;
            void Deallocate(void *memory) override;

            uint64 GetAllocateCallsCount() const override;
            uint64 GetDeallocateCallsCount() const override;

            void *AllocateStringBuffer(size_t sizeInBytes) override;
            void DeallocateStringBuffer(void *buffer) override;

            void *AllocatePtrMeta(size_t sizeInBytes) override;
            void DeallocatePtrMeta(void *buffer) override;

            template<typename T, typename ... TArgs>
            T* Create(TArgs&& ... args) {
                auto mem = Allocate(sizeof(T));
                return new (mem) T(std::forward<TArgs>(args)...);
            }

            template<typename T>
            void Release(T* system) {
                system->~T();
                Deallocate(system);
            }

        private:
            AtomicUint64 mAllocCalls;
            AtomicUint64 mDeallocCalls;

            LinuxFileSystem::LinuxImpl* mFileSystem;

            volatile uint32 mExitCode = 0;
        };

    }
}

#endif //BERSERK_LINUXSYSTEM_HPP