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
            std::memcpy(destination, source, sizeInBytes);
        }

        void Memory::Set(void *destination, uint32 value, size_t sizeInBytes) {
            std::memset(destination, (int) value, sizeInBytes);
        }

        void* Memory::Allocate(size_t sizeInBytes) {
            return Platform::System::Instance().Allocate(sizeInBytes);
        }

        void Memory::Deallocate(void *memory) {
            Platform::System::Instance().Deallocate(memory);
        }

    }
}