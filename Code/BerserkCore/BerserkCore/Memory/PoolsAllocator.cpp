/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Memory/PoolsAllocator.hpp>

namespace Berserk {

    PoolsAllocator::PoolsAllocator(size_t initialPoolChunkSize) {
        mInitialChunkSize = Math::Utils::Max(initialPoolChunkSize, INITIAL_POOL_CHUNK_SIZE);
        mPools.Emplace(mInitialChunkSize);
    }

    PoolsAllocator::~PoolsAllocator() {
        BERSERK_ASSERT(mAllocatedChunks == 0);
    }

    void* PoolsAllocator::Allocate(size_t sizeInBytes) {
        Guard<SpinMutex> guard(mMutex);

        auto poolIndex = GetPoolIndex(sizeInBytes);
        ExpandIfNeeded(poolIndex);
        mAllocatedChunks += 1;
        return mPools[poolIndex].Allocate(sizeInBytes);
    }

    void PoolsAllocator::Deallocate(void *memory, size_t sizeInBytes) {
        Guard<SpinMutex> guard(mMutex);

        auto poolIndex = GetPoolIndex(sizeInBytes);
        mPools[poolIndex].Deallocate(memory);
        mAllocatedChunks -= 1;
    }

    size_t PoolsAllocator::GetAllocatedChunks() const {
        Guard<SpinMutex> guard(mMutex);
        return mAllocatedChunks;
    }

    size_t PoolsAllocator::GetPoolIndex(size_t bytesToAllocate) const {
        size_t base = mInitialChunkSize;
        size_t index = 0;

        while (base < bytesToAllocate) {
            base *= SCALE_FACTOR;
            index += 1;
        }

        return index;
    }

    void PoolsAllocator::ExpandIfNeeded(size_t index) {
        if (index >= mPools.GetSize()) {
            mPools.EnsureCapacity(index + 1);

            size_t lastPoolIndex = mPools.GetSize() - 1;
            size_t chunkSize = mPools[lastPoolIndex].GetChunkSize();

            for (size_t i = lastPoolIndex + 1; i <= index; i++) {
                chunkSize *= SCALE_FACTOR;
                mPools.Emplace(chunkSize);
            }
        }
    }

}