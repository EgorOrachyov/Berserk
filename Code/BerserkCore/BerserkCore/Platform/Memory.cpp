/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <cstring>

namespace Berserk {
    namespace Platform {

        void Memory::Copy(void *destination, const void *source, size_t sizeInBytes) {
            if (sizeInBytes > 0)
                std::memcpy(destination, source, sizeInBytes);
        }

        void Memory::Set(void *destination, uint32 value, size_t sizeInBytes) {
            if (sizeInBytes > 0)
                std::memset(destination, (int) value, sizeInBytes);
        }

        void* Memory::Allocate(size_t sizeInBytes) {
            return Platform::System::Impl::Instance().Allocate(sizeInBytes);
        }

        void * Memory::Reallocate(void *memory, size_t sizeInBytes) {
            return Platform::System::Impl::Instance().Reallocate(memory, sizeInBytes);
        }

        void Memory::Deallocate(void *memory) {
            Platform::System::Impl::Instance().Deallocate(memory);
        }

        uint64 Memory::GetAllocateCalls() {
            return Platform::System::Impl::Instance().GetAllocateCallsCount();
        }

        uint64 Memory::GetDeallocateCalls() {
            return Platform::System::Impl::Instance().GetDeallocateCallsCount();
        }

    }
}