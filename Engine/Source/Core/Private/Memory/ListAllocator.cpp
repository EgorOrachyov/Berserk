//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Misc/Assert.h"
#include "Misc/Alignment.h"
#include "Math/MathUtility.h"
#include "Memory/Allocator.h"
#include "Memory/ListAllocator.h"

namespace Berserk
{

    ListAllocator::ListAllocator(uint32 bufferSize)
    {
        FAIL(bufferSize >= MIN_BUFFER_SIZE , "Chunk size must be more than %u", MIN_BUFFER_SIZE);
        ALIGN(bufferSize);

        mUsage = 0;
        mTotalSize = 0;
        mChunk = nullptr;
        mBuffer = nullptr;
        mBufferSize = bufferSize;

        expand();
    }

    ListAllocator::~ListAllocator()
    {
        if (mBuffer != nullptr)
        {
            auto current = mBuffer;
            while (current) {
                fprintf(stdout, "List Allocator: free buffer %p\n", current);

                auto next = current->next;
                Allocator::getSingleton().memoryFree(current);
                current = next;
            }

            mBuffer = nullptr;

            fprintf(stdout, "List Allocator: delete buffers\n");
        }
    }

    void* ListAllocator::alloc(uint32 size)
    {
        FAIL(size <= mBufferSize, "An attempt to acquire block bigger than Buffer Size %u", mBufferSize);
        ALIGN(size);
        size = Math::max(size, MIN_SPLIT_SIZE);

        if (!mChunk)
        {
            expand();
        }

        auto current = mChunk;
        auto best = mChunk;

        while (current)
        {
            if (current->size >= size && current->size <= best->size)
            { best = current; }

            current = current->next;
        }

        if (best->size < size)
        {
            expand();
            best = mChunk;
        }

        auto source = best;
        Chunk* left = nullptr;
        Chunk* right = nullptr;

        split(source, size, &left, &right);

        if (right)
        {
            if (best == mChunk) mChunk = right;
        }
        else
        {
            if (best == mChunk) mChunk = left->next;
        }

        mUsage += left->size + sizeof(Block);
        return ((uint8*)left + sizeof(Block));
    }

    void ListAllocator::free(void *pointer)
    {
        mUsage -= sizeof(Block) + ((Block*)((uint8*)pointer - sizeof(Block)))->size;

        if (mChunk == nullptr)
        {
            auto block = (Block*) ((uint8*)pointer - sizeof(Block));
            mChunk = (Chunk*) (block);
            mChunk->size = block->size;
            mChunk->prev = nullptr;
            mChunk->next = nullptr;

            return;
        }

        // Merge components
        // We insert free chunk in the list in the correct order,
        // and then try to merge it with its neighbours

        Chunk* left = nullptr;
        Chunk* mid = nullptr;
        Chunk* right = nullptr;

        auto block = (Block*) ((uint8*)pointer - sizeof(Block));
        auto chunk = (Chunk*) block;

        if (chunk < mChunk)
        {
            chunk->prev = nullptr;
            chunk->next = mChunk;
            mChunk->prev = chunk;
            mChunk = chunk;

            left = mChunk;
            mid = mChunk->next;
        }
        else
        {
            // We have to iterate through the list and
            // find appropriate place for the chunk

            auto placed = false;
            auto current = mChunk;
            auto next = mChunk->next;

            while (next)
            {
                if (current < chunk && chunk < next)
                {
                    chunk->prev = current;
                    chunk->next = next;
                    current->next = chunk;
                    next->prev = chunk;

                    placed = true;

                    left = current;
                    mid = chunk;
                    right = next;

                    break;
                }

                current = next;
                next = next->next;
            }

            if (!placed)
            {
                current->next = chunk;
                chunk->prev = current;
                chunk->next = nullptr;

                left = current;
                mid = chunk;
            }
        }

        // Join / merge stage
        // Check whether we can do it and join

        if (couldJoin(left, mid, right))
            join(left,mid,right);
        else if (couldJoin(left, mid))
            join(left, mid);
        else if (couldJoin(mid, right))
            join(mid, right);
    }

    uint32 ListAllocator::getChunkSize() const
    {
        return mBufferSize;
    }

    uint32 ListAllocator::getBufferSize() const
    {
        return mBufferSize;
    }

    uint32 ListAllocator::getUsage() const
    {
        return mUsage;
    }

    uint32 ListAllocator::getTotalSize() const
    {
        return mTotalSize;
    }

    void ListAllocator::expand()
    {
        auto chunkSize = mBufferSize;
        auto bufferSize = chunkSize + sizeof(Buffer);
        auto buffer = (Buffer*) Allocator::getSingleton().memoryAllocate(bufferSize);
        buffer->next = mBuffer;
        buffer->size = chunkSize;
        mBuffer = buffer;

        auto chunk = (Chunk*)((uint8*)mBuffer + sizeof(Buffer));
        chunk->size = mBufferSize - sizeof(Block);

        // Inserts current allocated free block
        // in the order of increasing of addresses of chunks
        // it allows to insert and merge free block in the correct order

        if (mChunk)
        {
            if (chunk < mChunk)
            {
                chunk->prev = nullptr;
                chunk->next = mChunk;
                mChunk->prev = chunk;
                mChunk = chunk;
            }
            else
            {
                auto placed = false;
                auto current = mChunk;
                auto next = mChunk->next;
                while (next)
                {
                    if (current < chunk && chunk < next)
                    {
                        chunk->next = current->next;
                        chunk->prev = current;
                        current->next = chunk;
                        next->prev = chunk;

                        placed = true;

                        break;
                    }

                    current = next;
                    next = next->next;
                }

                if (!placed)
                {
                    current->next = chunk;
                    chunk->prev = current;
                    chunk->next = nullptr;
                }
            }
        }
        else
        {
            chunk->prev = nullptr;
            chunk->next = nullptr;
            mChunk = chunk;
        }

        mTotalSize += mBufferSize;

#if DEBUG
        profile("expand");
#endif
    }

    void ListAllocator::split(Chunk *source, uint32 size, Chunk **left, Chunk **right)
    {
        if (source->size < size + sizeof(Block) + MIN_SPLIT_SIZE)
        {
            *left = source;
            *right = nullptr;

            if (source->prev) source->prev->next = source->next;
            if (source->next) source->next->prev = source->prev;

            return;
        }

        *right = (Chunk*) ((uint8*)source + size + sizeof(Block));
        (*right)->size = source->size - size - sizeof(Block);
        (*right)->prev = source->prev;
        (*right)->next = source->next;

        *left  = source;
        (*left)->size = size;

        if (source->prev) source->prev->next = *right;
        if (source->next) source->next->prev = *right;
    }

    ListAllocator::Chunk * ListAllocator::join(Chunk *left, Chunk *right)
    {
        auto result = left;

        if (left->prev) left->prev->next = result;
        if (right->next) right->next->prev = result;

        result->size = left->size + sizeof(Block) + right->size;
        result->prev = left->prev;
        result->next = right->next;

        return result;
    }

    ListAllocator::Chunk * ListAllocator::join(Chunk *left, Chunk *mid, Chunk *right)
    {
        auto result = left;

        if (left->prev) left->prev->next = result;
        if (right->next) right->next->prev = result;

        result->size = left->size + sizeof(Block) + mid->size + sizeof(Block) + right->size;
        result->prev = left->prev;
        result->next = right->next;

        return result;
    }

    bool ListAllocator::couldJoin(Chunk *left, Chunk *right)
    {
        return ((uint8*)left + sizeof(Block) + left->size == (uint8*)right);
    }

    bool ListAllocator::couldJoin(Chunk *left, Chunk *mid, Chunk *right)
    {
        return couldJoin(left, mid) && couldJoin(mid,right);
    }

#if DEBUG
    void ListAllocator::profile(const char* msg)
    {
        fprintf(stdout,
                "List Allocator: %s: usage: %u | total: %u | block size: %u | buffer size: %lu\n",
                msg, mUsage, mTotalSize, mBufferSize, sizeof(Buffer) + mBufferSize);
    }

    void ListAllocator::blocks(const char *msg)
    {
        printf("\nList allocator blocks info\n");

        uint64 count = 0;
        uint64 space = 0;
        auto current = mChunk;
        while (current)
        {
            fprintf(stdout, "Size: %lu | prev: %p | this: %p | next: %p \n", current->size, current->prev, current, current->next);
            count += 1;
            space += current->size;
            current = current->next;
        }

        fprintf(stdout, "List allocator: %s: total blocks: %lu | free: %lu \n", msg, count, space);
    }
#endif

}