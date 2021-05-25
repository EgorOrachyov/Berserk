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
        using Pool = PoolAllocator<Allocator>;
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

        mutable SpinMutex mMutex;
    };

}

#endif //BERSERK_POOLSALLOCATOR_HPP
