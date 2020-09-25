/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Memory/PoolAllocator.hpp>
#include <BerserkCore/Math/Math.hpp>
#include <BerserkCore/LogMacro.hpp>

namespace Berserk {

    struct AllocRegion {
        AllocRegion* next;
        uint64 chunkCount;
    };

    struct AllocChunk {
        AllocChunk* next;
    };

    PoolAllocator::PoolAllocator(uint32 chunkSize)
        : PoolAllocator(chunkSize, INITIAL_CHUNKS_COUNT) {

    }

    PoolAllocator::PoolAllocator(uint32 chunkSize, uint32 initialChunksCount)
        : mChunkSize(Math::max(chunkSize, (uint32)sizeof(AllocChunk))),
          mChunksToExpand(Math::max(1u, initialChunksCount)) {

    }

    PoolAllocator::PoolAllocator(PoolAllocator &&other) noexcept {
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

    PoolAllocator::~PoolAllocator() {
        if (mRegions) {
            auto current = (AllocRegion*) mRegions;
            while (current != nullptr) {
                auto next = current->next;
                mAlloc.free(current);
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

    void *PoolAllocator::allocate(uint64 size) {
        if (size > mChunkSize) {
            BERSERK_LOG_ERROR("Cannot allocate chunk of specified size");
            return nullptr;
        }

        if (mChunks == nullptr) {
            expand();
        }

        uint8* memory = mChunks;
        mChunks = (uint8*)(((AllocChunk*)mChunks)->next);
        mChunksAllocated += 1;

        return memory;
    }

    void PoolAllocator::free(void *memory) {
        if (!check(memory)) {
            BERSERK_LOG_ERROR("Attempt to free invalid memory chunk");
            return;
        }

        auto chunk = (AllocChunk*) mChunks;
        auto freed = (AllocChunk*) memory;
        freed->next = chunk;
        mChunks = (uint8*) freed;
        mChunksAllocated -= 1;
    }

    uint32 PoolAllocator::getRegionsCount() const {
        uint32 c = 0;
        auto current = (AllocRegion*)mRegions;
        while (current != nullptr) {
            c += 1;
            current = current->next;
        }
        return c;
    }

    bool PoolAllocator::check(void *memory) const {
        auto region = (AllocRegion*) mRegions;

        while (region != nullptr) {
            auto count = region->chunkCount;
            auto size = count * mChunkSize;
            auto begin = (uint8*)region + sizeof(AllocRegion);
            auto end = begin + size;
            auto remainder = ((uint64)memory - (uint64)begin) % mChunkSize;

            if (memory >= begin && memory < end && remainder == 0) {
                return true;
            }

            region = region->next;
        }

        return false;
    }

    void PoolAllocator::expand() {
        uint32 regionSize = sizeof(AllocRegion) + mChunksToExpand * mChunkSize;
        auto region = (AllocRegion*) mAlloc.allocate(regionSize);
        region->next = (AllocRegion*) mRegions;
        region->chunkCount = mChunksToExpand;
        mRegions = (uint8*) region;
        mChunksToExpand = mChunksToExpand * FACTOR;
        mMemUsage += regionSize;

        mark(mRegions);
    }

    void PoolAllocator::mark(uint8 *region) {
        auto reg = (AllocRegion*) region;
        auto count = reg->chunkCount;

        uint8* current = region + sizeof(AllocRegion);
        uint8* next = current + mChunkSize;
        uint8* first = current;

        while (next < (first + count * mChunkSize)) {
            auto chunkCurrent = (AllocChunk*) current;
            auto chunkNext = (AllocChunk*) next;
            chunkCurrent->next = chunkNext;

            current = next;
            next = next + mChunkSize;
        }

        auto chunkCurrent = (AllocChunk*) current;
        chunkCurrent->next = (AllocChunk*) mChunks;

        mChunks = first;
        mChunksCount += count;
    }
    
}