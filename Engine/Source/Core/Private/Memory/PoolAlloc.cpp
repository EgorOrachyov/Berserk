/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
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

#include "Memory/PoolAlloc.h"
#include <Misc/Error.h>
#include <Misc/MathUtils.h>

namespace Berserk {

    /** Cast memory regions and chunks to this structure */
    struct MemSection {
        uint8* next;
    };

    PoolAlloc::PoolAlloc(Berserk::uint32 chunkSize, Berserk::uint32 chunkCount,
                                 Berserk::IAlloc &allocator)
        : mAllocator(allocator),
          mChunkSize(MathUtils::max(chunkSize, sizeof(void*))),
          mRegionSize(chunkCount * mChunkSize + sizeof(void*)) {
        RAW_ERROR_CONDITION(chunkSize > 0, "Zero chunk size");
        RAW_ERROR_CONDITION(chunkCount > 0, "Zero chunk count for single region");
        _expand();
    }

    PoolAlloc::~PoolAlloc() {
        auto current = mRegions;
        while (current != nullptr) {
            auto region = (MemSection*) current;
            auto next = region->next;
            mAllocator.free(current);
            current = next;
        }

        mRegions = nullptr;
        mChunks = nullptr;
        mChunkSize = 0;
        mRegionSize = 0;
        mMemoryUsage = 0;
    }

    void *PoolAlloc::malloc(uint32 size) {
        RAW_ERROR_CONDITION(size <= mChunkSize, "Cannot allocate block of such size");

        if (!mChunks) {
            _expand();
        }

        auto chunk = (MemSection*) mChunks;
        mChunks = chunk->next;
        mFreeChunks -= 1;
        return chunk;
    }

    void *PoolAlloc::malloc(uint32 size, uint32 alignment) {
        RAW_ERROR("Cannot allocate aligned memory block");
        return nullptr;
    }

    void PoolAlloc::free(void *pointer) {
#if MODE_DEBUG || MODE_EDITOR
        auto belongs = _belongs(pointer);
        RAW_ERROR_CONDITION(belongs, "An attempt to free unknown memory block");
#endif
        auto chunk = (MemSection*) pointer;
        chunk->next = mChunks;
        mChunks = (uint8*) chunk;
        mFreeChunks += 1;
    }

    void PoolAlloc::clean() {
        RAW_ERROR_CONDITION(mRegions, "An attempt to clean empty pool");

        mFreeChunks = 0;

        auto currentRegion = mRegions;
        mChunks = currentRegion + sizeof(void*);
        uint8* prevChunk = nullptr;

        while (currentRegion != nullptr) {
            auto currentChunk = currentRegion + sizeof(void*);

            if (prevChunk) {
                auto chunk = (MemSection*) prevChunk;
                chunk->next = currentChunk;
            }

            while (currentChunk + mChunkSize < currentRegion + mRegionSize) {
                auto next = currentChunk + mChunkSize;
                auto chunk = (MemSection*) currentChunk;
                chunk->next = next;
                currentChunk = next;
                mFreeChunks += 1;
            }

            prevChunk = currentChunk;
            mFreeChunks += 1;

            auto region = (MemSection*) currentRegion;
            auto next = region->next;
            currentRegion = next;
        }
    }

    void PoolAlloc::_expand() {
        auto region = (uint8*) mAllocator.malloc(mRegionSize);
        auto first = region + sizeof(void*);
        auto current = first;

        while (current + mChunkSize < region + mRegionSize) {
            auto next = current + mChunkSize;
            auto chunk = (MemSection*) current;
            chunk->next = next;
            current = next;
            mFreeChunks += 1;
        }

        /** Add last chunk. Connect new chunks and prev chunks */
        auto lastChunk = (MemSection*) current;
        lastChunk->next = mChunks;
        mFreeChunks += 1;

        /** Set new chunks as head of all chunks */
        mChunks = first;

        /** Add this region as first region */
        auto firstRegion = (MemSection*) region;
        firstRegion->next = mRegions;
        mRegions = region;

        mMemoryUsage += mRegionSize;
    }

    bool PoolAlloc::_belongs(void *ptr) {
        auto current = mRegions;
        while (current != nullptr) {
            if (ptr > current && ptr <= current + mRegionSize) {
                return true;
            }

            auto region = (MemSection*) current;
            auto next = region->next;
            current = next;
        }

        return false;
    }

}