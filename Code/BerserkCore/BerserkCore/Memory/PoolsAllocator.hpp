/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_POOLSALLOCATOR_HPP
#define BERSERK_POOLSALLOCATOR_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Memory/PoolAllocator.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    class PoolsAllocator {
    public:
        using Allocator = Platform::Allocator;
        using Pool = PoolAllocator<Platform::Allocator>;
        using Pointer = void*;
        using Size = size_t;

        static const size_t SCALE_FACTOR = 2;
        static const size_t INITIAL_POOL_CHUNK_SIZE = 64;

        explicit PoolsAllocator(size_t initialPoolChunkSize = INITIAL_POOL_CHUNK_SIZE);
        ~PoolsAllocator();

        void* Allocate(size_t sizeInBytes);
        void Deallocate(void* memory, size_t sizeInBytes);

        size_t GetInitialPoolChunkSize() const { return mInitialChunkSize; }
        size_t GetAllocatedChunks() const;

    private:

        size_t GetPoolIndex(size_t bytesToAllocate) const;
        void ExpandIfNeeded(size_t index);

        Array<Pool> mPools;
        size_t mInitialChunkSize = 0;
        size_t mAllocatedChunks = 0;

        mutable Platform::SpinMutex mMutex;
    };

}

#endif //BERSERK_POOLSALLOCATOR_HPP
