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

#ifndef BERSERK_POOLALLOCATOR_HPP
#define BERSERK_POOLALLOCATOR_HPP

#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Math/Utils.hpp>

namespace Berserk {

    template<typename Alloc = Allocator>
    class PoolAllocator {
    public:
        using Allocator = Alloc;
        using Pointer = void*;
        using Size = size_t;

        /** How internal buffers are scaled on memory expand  */
        static const size_t EXPAND_FACTOR = 2;
        static const size_t INITIAL_CHUNKS_COUNT = 4;

        explicit PoolAllocator(size_t chunkSize, size_t initialChunksCount = INITIAL_CHUNKS_COUNT, Alloc&& alloc = Alloc())
            : mAlloc(std::move(alloc)), mRegions(Alloc(alloc)) {

            mChunkSize = Math::Utils::Max(chunkSize, sizeof(Chunk));
            mNextToExpand = Math::Utils::Max((size_t)1, initialChunksCount);
        }

        PoolAllocator(const PoolAllocator& other)
            : mAlloc(other.mAlloc) {
            mChunkSize = other.mChunkSize;
        }

        PoolAllocator(PoolAllocator&& other) noexcept
            : mAlloc(std::move(other.mAlloc)), mRegions(std::move(other.mRegions)) {
            mNextToExpand = other.mNextToExpand;
            mChunkSize = other.mChunkSize;
            mTotalChunks = other.mTotalChunks;
            mAllocatedChunks = other.mAllocatedChunks;
            mFirstFree = other.mFirstFree;

            other.mNextToExpand = 0;
            other.mChunkSize = 0;
            other.mTotalChunks = 0;
            other.mAllocatedChunks = 0;
            other.mFirstFree = nullptr;
        }

        ~PoolAllocator() {
            assert(mAllocatedChunks == 0);

            for (auto& region: mRegions) {
                mAlloc.Deallocate(region.pointer);
            }
        }

        void* Allocate(size_t sizeInBytes) {
            assert(sizeInBytes == mChunkSize);

            if (sizeInBytes > mChunkSize) {
                return nullptr;
            }

            ExpandIfNeeded();

            auto memory = mFirstFree;
            mFirstFree = mFirstFree->next;
            mAllocatedChunks += 1;

            return memory;
        }

        void Deallocate(void* pointer) {
            assert(pointer != nullptr);
            assert(mAllocatedChunks > 0);
            assert(Belongs(pointer));

            auto newHead = (Chunk*) pointer;
            newHead->next = mFirstFree;
            mFirstFree = newHead;
            mAllocatedChunks -= 1;
        }

        size_t GetChunkSize() const {
            return mChunkSize;
        }

        size_t GetTotalChunks() const {
            return mTotalChunks;
        }

        size_t GetAllocatedChunks() const {
            return mAllocatedChunks;
        }

        Alloc& GetAllocator() {
            return mAlloc;
        }

    private:

        struct Chunk {
            Chunk* next = nullptr;
        };

        struct RegionInfo {
            RegionInfo() = default;

            RegionInfo(void* pointer, size_t chunksCount)
                : pointer(pointer), chunksCount(chunksCount) {

            }

            void MarkRegion(Chunk* &newHead, Chunk* oldHead, size_t chunkSize) const {
                auto region = pointer;
                auto count = chunksCount;

                uint8* current = (uint8*) region;
                uint8* next = current + chunkSize;

                uint8* first = current;

                while (next < (first + count * chunkSize)) {
                    auto chunkCurrent = (Chunk*) current;
                    auto chunkNext = (Chunk*) next;

                    chunkCurrent->next = chunkNext;

                    current = next;
                    next = next + chunkSize;
                }

                auto chunkCurrent = (Chunk*) current;
                chunkCurrent->next = oldHead;

                newHead = (Chunk*) first;
            }

            bool Belongs(void* chunk, size_t chunkSize) const {
                auto size = chunksCount * chunkSize;
                auto begin = (uint8*)pointer;
                auto end = begin + size;
                auto remainder = ((uint64)chunk - (uint64)begin) % chunkSize;

                return (chunk >= begin) && (chunk < end) && (remainder == 0);
            }

            void* pointer = nullptr;
            size_t chunksCount = 0;
        };

        void ExpandIfNeeded() {
            assert(mChunkSize > 0);

            if (mFirstFree == nullptr) {
                auto toExpand = mNextToExpand;
                auto toAllocate = toExpand * mChunkSize;

                void* pointer = mAlloc.Allocate(toAllocate);
                auto& region = mRegions.Emplace(pointer, toExpand);

                Chunk* newHead = nullptr;

                region.MarkRegion(newHead, mFirstFree, mChunkSize);

                mFirstFree = newHead;
                mTotalChunks += toExpand;
                mNextToExpand *= EXPAND_FACTOR;
            }
        }

        bool Belongs(void* pointer) {
            for (auto& region: mRegions) {
                if (region.Belongs(pointer, mChunkSize))
                    return true;
            }

            return false;
        }

        Alloc mAlloc;
        Array<RegionInfo, Alloc> mRegions;
        size_t mNextToExpand = INITIAL_CHUNKS_COUNT;
        size_t mChunkSize = 0;
        size_t mTotalChunks = 0;
        size_t mAllocatedChunks = 0;
        Chunk* mFirstFree = nullptr;
    };

}



#endif //BERSERK_POOLALLOCATOR_HPP
