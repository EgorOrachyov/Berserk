/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Memory.h>
#include <BuildOptions.h>
#include <stdlib.h>

namespace Berserk {

    static std::atomic_long sAllocCalls(0);
    static std::atomic_long sFreeCalls(0);

    void* Memory::allocate(uint64 size) {
#ifdef BERSERK_DEBUG
        sAllocCalls.fetch_add(1, std::memory_order_relaxed);
#endif
        return malloc(size);
    }

    void* Memory::reallocate(void *source, uint64 newsize) {
        return realloc(source, newsize);
    }

    void Memory::free(void *memory) {
#ifdef BERSERK_DEBUG
        sFreeCalls.fetch_add(1, std::memory_order_relaxed);
#endif
        ::free(memory);
    }

    void Memory::copy(void *destination, const void *source, uint64 size) {
        memcpy(destination, source, size);
    }

    void Memory::set(void *source, Berserk::uint32 value, uint64 size) {
        memset(source, value, size);
    }

    uint64 Memory::getAllocCalls() {
#ifdef BERSERK_DEBUG
        return sAllocCalls.load(std::memory_order_relaxed);
#endif
        return 0;
    }

    uint64 Memory::getFreeCalls() {
#ifdef BERSERK_DEBUG
        return sFreeCalls.load(std::memory_order_relaxed);
#endif
        return 0;
    }

}
