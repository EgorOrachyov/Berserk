/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TPOOLALLOCATOR_HPP
#define BERSERK_TPOOLALLOCATOR_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    template <typename A = GlobalAllocator>
    class TPoolAllocator: public Allocator {
    public:
        static const uint32 INITIAL_CHUNKS_COUNT = 4;
        static const uint32 FACTOR = 2;

        TPoolAllocator(uint32 chunkSize, A alloc = A());
        TPoolAllocator(uint32 chunkSize, uint32 initialChunksCount, A alloc = A());
        TPoolAllocator(TPoolAllocator&& other) noexcept;
        ~TPoolAllocator() override;

        void *Allocate(uint64 size) override;
        void Free(void *memory) override;

        uint32 getChunkSize() const { return mChunkSize; }
        uint32 getChunksCount() const { return mChunksCount; }
        uint32 getRegionsCount() const;
        uint32 getChunksAllocated() const { return mChunksAllocated; }
        uint64 getAllocatedMemory() const { return mMemUsage; }

    private:
        bool check(void* memory) const;
        void expand();
        void mark(uint8* region);

        A mAlloc;
        uint8* mRegions = nullptr;
        uint8* mChunks = nullptr;
        uint32 mChunkSize = 0;
        uint32 mChunksCount = 0;
        uint32 mChunksAllocated = 0;
        uint32 mChunksToExpand = 0;
        uint64 mMemUsage = 0;
    };

}

#include <BerserkCore/Memory/TPoolAllocator.inl>

#endif //BERSERK_TPOOLALLOCATOR_HPP