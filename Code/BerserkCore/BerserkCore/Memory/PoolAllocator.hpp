/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_POOLALLOCATOR_HPP
#define BERSERK_POOLALLOCATOR_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    class PoolAllocator: public Allocator {
    public:
        static const uint32 INITIAL_CHUNKS_COUNT = 4;
        static const uint32 FACTOR = 2;

        explicit PoolAllocator(uint32 chunkSize);
        PoolAllocator(uint32 chunkSize, uint32 initialChunksCount);
        PoolAllocator(PoolAllocator&& other) noexcept;
        ~PoolAllocator() override;

        void *allocate(uint64 size) override;
        void free(void *memory) override;

        uint32 getChunkSize() const { return mChunkSize; }
        uint32 getChunksCount() const { return mChunksCount; }
        uint32 getRegionsCount() const;
        uint32 getChunksAllocated() const { return mChunksAllocated; }
        uint64 getAllocatedMemory() const { return mMemUsage; }

    private:
        bool check(void* memory) const;
        void expand();
        void mark(uint8* region);

        GlobalAllocator mAlloc;
        uint8* mRegions = nullptr;
        uint8* mChunks = nullptr;
        uint32 mChunkSize;
        uint32 mChunksCount = 0;
        uint32 mChunksAllocated = 0;
        uint32 mChunksToExpand;
        uint64 mMemUsage = 0;
    };

}

#endif //BERSERK_POOLALLOCATOR_HPP