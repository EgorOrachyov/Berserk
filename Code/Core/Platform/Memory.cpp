/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Memory.h>
#include <BuildOptions.h>
#include <stdlib.h>

namespace Berserk {

    struct MemDebugStat {
        MemDebugStat() noexcept {
            allocCalls.store(0);
            freeCalls.store(0);
        }
        ~MemDebugStat() {
            auto a = allocCalls.load();
            auto f = allocCalls.load();

            printf("[Berserk Core] Memory stat: alloc: %llu free: %llu\n", a, f);
            if (a != f) {
                fprintf(stderr, "[Berserk Core] Memory leak");
            }
        }
        std::atomic_uint64_t allocCalls{};
        std::atomic_uint64_t freeCalls{};
    };

#ifdef BERSERK_DEBUG
    static MemDebugStat sMemDebugStat;
#endif

    void* Memory::allocate(uint64 size) {
#ifdef BERSERK_DEBUG
        sMemDebugStat.allocCalls.fetch_add(1, std::memory_order_relaxed);
#endif
        return malloc(size);
    }

    void* Memory::reallocate(void *source, uint64 newsize) {
        return realloc(source, newsize);
    }

    void Memory::free(void *memory) {
#ifdef BERSERK_DEBUG
        sMemDebugStat.freeCalls.fetch_add(1, std::memory_order_relaxed);
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
        return sMemDebugStat.freeCalls.load(std::memory_order_relaxed);
#endif
        return 0;
    }

    uint64 Memory::getFreeCalls() {
#ifdef BERSERK_DEBUG
        return sMemDebugStat.freeCalls.load(std::memory_order_relaxed);
#endif
        return 0;
    }

    const Function<void(void*)> Memory::DEFAULT_DEALLOC = [](void* mem){ Memory::free(mem); };

}
