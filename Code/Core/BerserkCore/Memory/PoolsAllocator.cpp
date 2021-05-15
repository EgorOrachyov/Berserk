/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <BerserkCore/Memory/PoolsAllocator.hpp>

namespace Berserk {

    PoolsAllocator::PoolsAllocator(size_t initialPoolChunkSize) {
        mInitialChunkSize = Math::Utils::Max(initialPoolChunkSize, INITIAL_POOL_CHUNK_SIZE);
        mPools.Emplace(mInitialChunkSize);
    }

    PoolsAllocator::~PoolsAllocator() {
        assert(mAllocatedChunks == 0);
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