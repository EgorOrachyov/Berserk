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

        A& GetAllocator() { return mAlloc; }
        uint32 GetChunkSize() const { return mChunkSize; }
        uint32 GetChunksCount() const { return mChunksCount; }
        uint32 GetRegionsCount() const;
        uint32 GetChunksAllocated() const { return mChunksAllocated; }
        uint64 GetAllocatedMemory() const { return mMemUsage; }

    private:
        bool Check(void* memory) const;
        void Expand();
        void Mark(uint8* region);

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