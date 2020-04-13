/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <String/TString.h>
#include <Platform/ISystem.h>
#include <Platform/Mutex.h>
#include <BuildOptions.h>
#include <TArray.h>


namespace Berserk {

    const uint32 StringBufferAlloc::POOL_SIZE_INITIAL = 64;
    const uint32 StringBufferAlloc::POOL_SIZE_FACTOR = 2;

    /** Each pool allocates string buffers with concrete size */
    TArray<AllocPool> gStringPools;
    Mutex gAccessMutex;
    
    StringBufferAlloc::StringBufferAlloc() {
        gStringPools.emplace(POOL_SIZE_INITIAL);
    }

    StringBufferAlloc::~StringBufferAlloc() {
#ifdef BERSERK_DEBUG
        for (const auto& pool: gStringPools) {
            auto count = pool.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost string buffers [%u] for pool [%u]\n", count, pool.getChunkSize());
            }
        }
#endif
    }

    void* StringBufferAlloc::allocate(uint32& size) {
        Guard guard(gAccessMutex);

        auto index = getIndex(size);

        if (index >= gStringPools.size()) {
            expand(index);
        }

        auto& pool = gStringPools[index];
        size = pool.getChunkSize();
        return pool.allocate(0);
    }

    void StringBufferAlloc::free(void *memory, Berserk::uint32 size) {
        Guard guard(gAccessMutex);

        auto index = getIndex(size);
        gStringPools[index].free(memory);
    }

    void StringBufferAlloc::expand(Berserk::uint32 index) {
        gStringPools.ensureCapacity(index);

        uint32 last = gStringPools.size() - 1;
        uint32 size = gStringPools[last].getChunkSize();

        for (uint32 i = last + 1; i <= index; i++) {
            size *= POOL_SIZE_FACTOR;
            gStringPools.emplace(size);
        }
    }

    uint32 StringBufferAlloc::getIndex(Berserk::uint32 size) const {
        uint32 i = 0;
        uint32 r = size / (POOL_SIZE_INITIAL + 1);

        while (r != 0) {
            r = r / POOL_SIZE_FACTOR;
            i += 1;
        }

        return i;
    }

    StringBufferAlloc& StringBufferAlloc::getSingleton() {
        static StringBufferAlloc sStringPool;
        return sStringPool;
    }

}


