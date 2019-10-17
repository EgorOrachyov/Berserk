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

#ifndef BERSERKTESTS_POOLALLOCATOR_H
#define BERSERKTESTS_POOLALLOCATOR_H

#include <Memory/Allocator.h>

namespace Berserk {

    class PoolAllocator : public IAllocator {
    public:

        /** Default number of chinks in the single region */
        static const uint32 DEFAULT_CHUNKS_COUNT_IN_REGION = 16;

    public:

        ~PoolAllocator() override;

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size) override;

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size, uint32 alignment) override;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override;

        /** @return Size of single allocatable chunk */
        uint32 getChunkSize() const {
            return mChunkSize;
        }

        /** @return Size of the regions to expand the buffer */
        uint32 getRegionSize() const {
            return mRegionSize;
        }

        /** @return Total allocated memory usage by allocator */
        uint32 getMemoryUsage() const {
            return mMemoryUsage;
        }

    private:

        /** Allocate new memory region and split it into chunks */
        void _expand();

    private:

        IAllocator& mAllocator;
        void** mChunks;
        void** mRegions;
        uint32 mChunkSize;
        uint32 mRegionSize;
        uint32 mFreeChunks;
        uint32 mMemoryUsage;

    };

}

#endif //BERSERKTESTS_POOLALLOCATOR_H