/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Memory.h>
#include <stdlib.h>

namespace Berserk {

    void* Memory::allocate(uint64 size) {
        return malloc(size);
    }

    void* Memory::reallocate(void *source, uint64 newsize) {
        return realloc(source, newsize);
    }

    void Memory::free(void *memory) {
        ::free(memory);
    }

    void Memory::copy(void *destination, const void *source, uint64 size) {
        memcpy(destination, source, size);
    }

    void Memory::set(void *source, Berserk::uint32 value, uint64 size) {
        memset(source, value, size);
    }

}
