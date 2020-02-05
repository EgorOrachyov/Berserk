/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <String/TString.h>
#include <Platform/ISystem.h>
#include <BuildOptions.h>

namespace Berserk {

    const uint32 StringBufferAlloc::POOL_SIZE_INITIAL = 64;
    const uint32 StringBufferAlloc::POOL_SIZE_FACTOR = 2;

    StringBufferAlloc::StringBufferAlloc() {
        mStringPools.emplace(POOL_SIZE_INITIAL);
        mAccessMutex = ISystem::getSingleton().createMutex();
    }

    StringBufferAlloc::~StringBufferAlloc() {
#ifdef BERSERK_DEBUG
        for (const auto& pool: mStringPools) {
            auto count = pool.getChunksAllocated();
            if (count != 0) {
                fprintf(stderr, "[Berserk Core] Lost string buffers [%u] for pool [%u]\n", count, pool.getChunkSize());
            }
        }
#endif
    }

    void* StringBufferAlloc::allocate(uint32& size) {
        Guard guard(*mAccessMutex);

        auto index = getIndex(size);

        if (index >= mStringPools.size()) {
            expand(index);
        }

        auto& pool = mStringPools[index];
        size = pool.getChunkSize();
        return pool.allocate(0);
    }

    void StringBufferAlloc::free(void *memory, Berserk::uint32 size) {
        Guard guard(*mAccessMutex);

        auto index = getIndex(size);
        mStringPools[index].free(memory);
    }

    void StringBufferAlloc::expand(Berserk::uint32 index) {
        mStringPools.ensureCapacity(index);

        uint32 last = mStringPools.size() - 1;
        uint32 size = mStringPools[last].getChunkSize();

        for (uint32 i = last + 1; i <= index; i++) {
            size *= POOL_SIZE_FACTOR;
            mStringPools.emplace(size);
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


