//
// Created by Egor Orachyov on 17.06.2019.
//

#ifndef BERSERK_LISTALLOCATOR_H
#define BERSERK_LISTALLOCATOR_H

#include <Memory/IAllocator.h>
#include <Memory/Allocator.h>
#include <Memory/PoolAllocator.h>
#include <Containers/THashMap.h>
#include <Containers/TLinkedList.h>

namespace Berserk
{

    class MEMORY_API ListAllocator : public IAllocator
    {
    public:

        explicit ListAllocator(uint32 bufferSize = DEFAULT_BUFFER_SIZE, IAllocator& allocator = Allocator::get());

        ~ListAllocator() override;

        /** @copydoc IAllocator::allocate() */
        void *allocate(uint32 size) override;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override;

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        uint32 getFreeCalls() const override { return mFreeCalls; }

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        uint32 getAllocateCalls() const override { return mAllocCalls; }

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        uint64 getTotalMemoryUsage() const override { return mTotalMemUsage; }

    protected:

        struct MemoryChunk
        {
        public:

            explicit MemoryChunk(uint32 size) : mSize(size)
            { }

            /** @return True if address of chunk: this < chunk < next */
            bool canLinkAfter(MemoryChunk* chunk)
            {
                return  (this < chunk && (mNext == nullptr || chunk < mNext));
            }

            /** Link and merge chunks */
            bool linkAfter(MemoryChunk* chunk)
            {
                chunk->mNext = mNext;
                mNext = chunk;

                /** Where this chunk is located */
                uint8* mMemory = getMemory();

                /** Try to merge this and chunk */
                {
                    uint8* afterThat = mMemory + mSize;

                    if (afterThat == chunk->getMemory())
                    {
                        mergeChunk(this, chunk);

                        /** Try to merge merged chunk and this next */
                        if (mNext != nullptr)
                        {
                            afterThat = mMemory + mSize;

                            if (afterThat == mNext->getMemory())
                            {
                                mergeChunk(this, mNext);
                            }
                        }
                    }
                    else
                    {
                        /** Try to merge chunk and this next */
                        if (mNext != nullptr)
                        {
                            uint8* afterChunk = chunk->getMemory() + chunk->size();

                            if (afterChunk == mNext->getMemory())
                            {
                                mergeChunk(chunk, mNext);
                            }
                        }
                    }
                }
            }

            /**
             * Inserts this chunk in the proper place
             * @return Prev chunk to chunk when insert
             */
            MemoryChunk* insert(MemoryChunk* chunk)
            {
                if (chunk < this)
                {
                    chunk->mNext = this;
                    return nullptr;
                }
                else
                {
                    MemoryChunk* current = this;
                    while (current != nullptr)
                    {
                        if (canLinkAfter(chunk))
                        {
                            chunk->mNext = current->mNext;
                            current->mNext = chunk;

                            return current;
                        }
                        current = current->next();
                    }
                }

                assertion_dev_msg(false, "ListAllocator: all chucks must be inserted");
            }

            /**
             * Inserts this chunk in the proper place and merge
             * @return New list head
             */
            MemoryChunk* insertAndMerge(MemoryChunk* chunk)
            {
                if (chunk < this)
                {
                    chunk->linkAfter(this);
                    return chunk;
                }
                else
                {
                    MemoryChunk* current = this;
                    while (current != nullptr)
                    {
                        if (canLinkAfter(chunk))
                        {
                            current->linkAfter(chunk);
                            return this;
                        }
                        current = current->next();
                    }
                }

                assertion_dev_msg(false, "ListAllocator: all chucks must be inserted and merged");
            }

            /** Splits this chunk into 2 (if possible): desired size chunk -> rest size chunk */
            void split(uint32 desiredSize)
            {
                if (desiredSize + MIN_SPLIT_SIZE > mSize) return;

                uint32 leftSize = desiredSize;
                uint32 rightSize = mSize - desiredSize;
                MemoryChunk* next = mNext;

                MemoryChunk* left = new (getMemory()) MemoryChunk(leftSize);
                MemoryChunk* right = new (getMemory() + size()) MemoryChunk(rightSize);

                left->mNext = right;
                right->mNext = next;
            }

            /** Remove chunk from the list */
            void remove(MemoryChunk* mPrev)
            {
                if (mPrev != nullptr) mPrev->mNext = mNext;
            }

            /** @return Where this chunk stored */
            uint8* getMemory() const
            {
                return (uint8*) this;
            }

            /** Merge left and right chunk neighbours */
            static void mergeChunk(MemoryChunk* left, MemoryChunk* right)
            {
                uint32 size = left->mSize + right->mSize;
                MemoryChunk* next = right->mNext;
                MemoryChunk* merged = new (left) MemoryChunk(size);
                merged->mNext = next;
            }

            /** @return Next node in the list */
            MemoryChunk* next() { return mNext; }

            /** @return Size of memory when allocate */
            uint32 size() { return mSize; }

        protected:

            /** Next in the list */
            MemoryChunk* mNext = nullptr;

            /** Size of memory when allocate */
            uint32 mSize = 0;

        };

        /** Actual buffers allocated by mAllocator */
        struct MemoryBuffer
        {
        public:

            /** Pass pointer to buffer memory and its size */
            MemoryBuffer(uint8* memory, uint32 size)
                    : mBufferMemoryRegion(memory),
                      mBufferSize(size)
            { }

            /** Creates new memory chunk wwith actual size of this buffer  */
            MemoryChunk* createChunk()
            {
                return new (mBufferMemoryRegion) MemoryChunk(mBufferSize);
            }

            /** @return Buffer size (in bytes) */
            uint32 size() const { return mBufferSize; }

            /** @return Buffer memory region pointer */
            uint8* memory() const { return mBufferMemoryRegion; }

        protected:

            /** Memory region */
            uint8* mBufferMemoryRegion = nullptr;

            /** Memory size (actual in bytes) */
            uint32 mBufferSize = 0;

        };

    private:

        /** Buffer expand size and max size of allocatable chunk */
        static const uint32 DEFAULT_BUFFER_SIZE = Buffers::MiB * 10;

        /** Min size of memory chunk which could be allocated */
        static const uint32 MIN_SPLIT_SIZE = sizeof(MemoryChunk);

        /** Chunks in pool to preallocate */
        static const uint32 POOL_CHUNKS_COUNT = 16;

        /** Range to preallocate in the map */
        static const uint32 MEMORY_MAP_INITIAL_SIZE = 32;


        /** Store here all the allocated buffers */
        typedef TLinkedList<MemoryBuffer> BuffersList;

        /** Map with allocated memory chunks and its sizes */
        typedef THashMap<uint8*, uint32> AllocatedMemory;

        /** For buffers list and map allocations (note: MemoryBuffer and TPair<uint8*,uint32 has te same size>) */
        PoolAllocator mMemoryPool;

        /** Buffers */
        BuffersList mBuffers;

        /** Ensure ourselves that we allocate and free memory properly by storing allocations info */
        AllocatedMemory mMemoryMap;

        /** Allocator for internal usage and allocations of memory (buffers) */
        IAllocator& mAllocator;

        /** Free chunks wits available memory */
        class MemoryChunk* mChunks = nullptr;

        /** Size of buffers when expand */
        uint32 mBufferSize = 0;

        /** Total number of allocated mem (this mem actually could be freed) */
        volatile uint64 mMemoryUsage = 0;

        /** Total number of free calls in the engine [in bytes] */
        volatile uint32 mFreeCalls = 0;

        /** Total number of allocate and memoryCAllocate in the engine [in bytes] */
        volatile uint32 mAllocCalls = 0;

        /** Total number of allocated mem (for internal buffers) */
        volatile uint64 mTotalMemUsage = 0;

    };

} // namespace Berserk

#endif //BERSERK_LISTALLOCATOR_H
