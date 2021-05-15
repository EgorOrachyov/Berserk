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

#include <BerserkCore/Memory/TPoolAllocator.hpp>
#include <BerserkCore/Math/Utils.hpp>
#include <BerserkCore/Logging.hpp>

namespace Berserk {

    namespace Details {
        struct AllocRegion {
            AllocRegion* next;
            uint64 chunkCount;
        };

        struct AllocChunk {
            AllocChunk* next;
        };
    }

    template <typename A>
    TPoolAllocator<A>::TPoolAllocator(uint32 chunkSize, A alloc)
        : TPoolAllocator<A>(chunkSize, INITIAL_CHUNKS_COUNT, std::move(alloc)) {

    }

    template <typename A>
    TPoolAllocator<A>::TPoolAllocator(uint32 chunkSize, uint32 initialChunksCount, A alloc)
        : mAlloc(std::move(alloc)),
          mChunkSize(Math::Max(chunkSize, (uint32)sizeof(Details::AllocChunk))),
          mChunksToExpand(Math::Max(1u, initialChunksCount)) {

    }

    template <typename A>
    TPoolAllocator<A>::TPoolAllocator(TPoolAllocator<A> &&other) noexcept {
        mAlloc = std::move(other.mAlloc);

        mRegions = other.mRegions;
        mChunks = other.mChunks;
        mChunkSize = other.mChunkSize;
        mChunksCount = other.mChunksCount;
        mChunksAllocated = other.mChunksAllocated;
        mChunksToExpand = other.mChunksToExpand;

        other.mRegions = nullptr;
        other.mChunks = nullptr;
        other.mChunkSize = 0;
        other.mChunksCount = 0;
        other.mChunksAllocated = 0;
        other.mChunksToExpand = 0;
    }

    template <typename A>
    TPoolAllocator<A>::~TPoolAllocator() {
        if (mRegions) {
            auto current = (Details::AllocRegion*) mRegions;
            while (current != nullptr) {
                auto next = current->next;
                mAlloc.Free(current);
                current = next;
            }

            mRegions = nullptr;
            mChunks = nullptr;
            mChunkSize = 0;
            mChunksCount = 0;
            mChunksToExpand = 0;
            mChunksAllocated = 0;
        }
    }

    template <typename A>
    void *TPoolAllocator<A>::Allocate(uint64 size) {
        if (size > mChunkSize) {
            BERSERK_LOG_ERROR("Cannot allocate chunk of specified size");
            return nullptr;
        }

        if (mChunks == nullptr) {
            Expand();
        }

        uint8* memory = mChunks;
        mChunks = (uint8*)(((Details::AllocChunk*)mChunks)->next);
        mChunksAllocated += 1;

        return memory;
    }

    template <typename A>
    void TPoolAllocator<A>::Free(void *memory) {
        if (!Check(memory)) {
            BERSERK_LOG_ERROR("Attempt to free invalid memory chunk");
            return;
        }

        auto chunk = (Details::AllocChunk*) mChunks;
        auto freed = (Details::AllocChunk*) memory;
        freed->next = chunk;
        mChunks = (uint8*) freed;
        mChunksAllocated -= 1;
    }

    template <typename A>
    uint32 TPoolAllocator<A>::GetRegionsCount() const {
        uint32 c = 0;
        auto current = (Details::AllocRegion*)mRegions;
        while (current != nullptr) {
            c += 1;
            current = current->next;
        }
        return c;
    }

    template <typename A>
    bool TPoolAllocator<A>::Check(void *memory) const {
        auto region = (Details::AllocRegion*) mRegions;

        while (region != nullptr) {
            auto count = region->chunkCount;
            auto size = count * mChunkSize;
            auto begin = (uint8*)region + sizeof(Details::AllocRegion);
            auto end = begin + size;
            auto remainder = ((uint64)memory - (uint64)begin) % mChunkSize;

            if (memory >= begin && memory < end && remainder == 0) {
                return true;
            }

            region = region->next;
        }

        return false;
    }

    template <typename A>
    void TPoolAllocator<A>::Expand() {
        uint32 regionSize = sizeof(Details::AllocRegion) + mChunksToExpand * mChunkSize;
        auto region = (Details::AllocRegion*) mAlloc.Allocate(regionSize);
        region->next = (Details::AllocRegion*) mRegions;
        region->chunkCount = mChunksToExpand;
        mRegions = (uint8*) region;
        mChunksToExpand = mChunksToExpand * FACTOR;
        mMemUsage += regionSize;

        Mark(mRegions);
    }

    template <typename A>
    void TPoolAllocator<A>::Mark(uint8 *region) {
        auto reg = (Details::AllocRegion*) region;
        auto count = reg->chunkCount;

        uint8* current = region + sizeof(Details::AllocRegion);
        uint8* next = current + mChunkSize;
        uint8* first = current;

        while (next < (first + count * mChunkSize)) {
            auto chunkCurrent = (Details::AllocChunk*) current;
            auto chunkNext = (Details::AllocChunk*) next;
            chunkCurrent->next = chunkNext;

            current = next;
            next = next + mChunkSize;
        }

        auto chunkCurrent = (Details::AllocChunk*) current;
        chunkCurrent->next = (Details::AllocChunk*) mChunks;

        mChunks = first;
        mChunksCount += count;
    }
    
}