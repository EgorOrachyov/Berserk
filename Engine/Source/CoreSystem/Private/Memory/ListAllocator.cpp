//
// Created by Egor Orachyov on 17.06.2019.
//

#include <Memory/ListAllocator.h>
#include <Misc/Alignment.h>

namespace Berserk
{

    ListAllocator::ListAllocator(uint32 bufferSize,
                                 uint32 mapRange,
                                 uint32 poolChunksCount,
                                 IAllocator &allocator)
            : mMemoryPool(Math::max(BuffersList::getNodeSize(), AllocatedMemory::getNodeSize()), poolChunksCount, allocator),
              mBuffers(mMemoryPool),
              mMemoryMap(mapRange, allocator, mMemoryPool),
              mAllocator(allocator),
              mBufferSize(bufferSize)
    {

    }

    ListAllocator::~ListAllocator()
    {
        for (auto buffer = mBuffers.begin(); buffer != nullptr; buffer = mBuffers.next())
        {
            mAllocator.free(buffer->memory());
        }

        char buffer[20];
#if DEBUG
        printf("ListAllocator: [allocation calls: %u] [free calls: %u] [total memory usage: %s]\n",
               mAllocCalls, mFreeCalls, Printer::print((uint32)getTotalMemoryUsage(), buffer));
#endif

        assertion_dev_msg(mAllocCalls == mFreeCalls,
                          "ListAllocator: [allocation calls: %u] [free calls: %u] [total memory usage: %s]\n",
                          mAllocCalls, mFreeCalls, Printer::print((uint32)getTotalMemoryUsage(), buffer));
    }

    void *ListAllocator::allocate(uint32 size)
    {
        size = Math::max(size, MIN_SPLIT_SIZE);

        ALIGN(size);
        assertion_dev_msg(size <= mBufferSize, "ListAllocator: cannot allocate chunk for size: %u", size);

        uint32 bestSize = mBufferSize + 1;
        MemoryChunk* bestFit = nullptr;
        MemoryChunk* bestFitPrev = nullptr;
        MemoryChunk* prev = nullptr;
        MemoryChunk* current = mChunks;

        while (current != nullptr)
        {
            if (current->size() < bestSize && current->size() >= size)
            {
                bestFit = current;
                bestSize = current->size();
                bestFitPrev = prev;

                if (current->size() == size) break;
            }

            prev = current;
            current = current->next();
        }

        if (bestFit == nullptr)
        {
            MemoryBuffer buffer((uint8*) mAllocator.allocate(mBufferSize), mBufferSize);
            mBuffers.add(buffer);
            mTotalMemUsage += mBufferSize;

            bestFit = buffer.createChunk();
            if (mChunks == nullptr)
            {
                mChunks = bestFit;

                bestFit->split(size);
                mChunks = bestFit->next();
                bestFit->remove(nullptr);
            }
            else
            {
                mChunks = mChunks->insert(bestFit, bestFitPrev);

                bestFit->split(size);
                if (bestFit == mChunks) mChunks = bestFit->next();
                bestFit->remove(bestFitPrev);
            }
        }
        else
        {
            bestFit->split(size);
            if (bestFit == mChunks) mChunks = bestFit->next();
            bestFit->remove(bestFitPrev);
        }

        mAllocCalls += 1;
        mMemoryUsage += bestFit->size();
        mMemoryMap.put(bestFit->getMemory(), bestFit->size());

        return bestFit;
    }

    void ListAllocator::free(void *pointer)
    {
        uint32* size = mMemoryMap.get((uint8*) pointer);
        assertion_dev_msg(size != nullptr, "ListAllocator: an attempt to free not allocated memory [pointer: %p]", pointer);

        uint32 chunkSize = *size;
        MemoryChunk* chunk = new (pointer) MemoryChunk(chunkSize);

        if (mChunks != nullptr) mChunks = mChunks->insertAndMerge(chunk);
        else mChunks = chunk;

        uint32 total = 0;
        MemoryChunk* current = mChunks;
        while (current != nullptr)
        {
            total += current->size();
            current = current->next();
        }

        mFreeCalls += 1;
        mMemoryUsage -= chunkSize;
        mMemoryMap.remove((uint8*) pointer);
    }

} // namespace Berserk