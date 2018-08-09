//
// Created by Egor Orachyov on 09.08.2018.
//

namespace Berserk
{

    ListAllocator::ListAllocator()
    {
        mNumOfBuffers = 0;
        mExpandSize = 0;
        mBufferSize = 0;
        mAllocatedMemory = 0;
        mFirst = nullptr;
        mBuffer = nullptr;
        mListOfFreeBlocks = nullptr;
    }

    ListAllocator::~ListAllocator()
    {
        reset();
    }

    void ListAllocator::init(UINT32 bufferSize)
    {
        mNumOfBuffers = 0;
        mExpandSize = 0;
        mBufferSize = 0;
        mAllocatedMemory = 0;
        mFirst = nullptr;
        mBuffer = nullptr;
        mListOfFreeBlocks = nullptr;

        ASSERT(bufferSize, "ListAllocator: Buffer size should be > 0");

        mBufferSize = bufferSize + ((bufferSize & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (bufferSize % MEMORY_ALIGNMENT));

        UINT32 expandBuffer = max(mBufferSize + (UINT32)sizeof(void*), (UINT32)sizeof(Buffer));
        mExpandSize = expandBuffer + ((expandBuffer & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (expandBuffer % MEMORY_ALIGNMENT));

        createList();
    }

    void ListAllocator::reset()
    {
        if (mBuffer)
        {
            Buffer* tmp = mBuffer;
            while (tmp != nullptr)
            {
                Buffer* toDelete = tmp;
                tmp = tmp->next;

                mem_free(toDelete);
            }
        }

        mNumOfBuffers = 0;
        mExpandSize = 0;
        mBufferSize = 0;
        mAllocatedMemory = 0;
        mFirst = nullptr;
        mBuffer = nullptr;
        mListOfFreeBlocks = nullptr;
    }

    void* ListAllocator::alloc(UINT32 size)
    {
        size = max(size, (UINT32)sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        return getBlock(size);
    }

    void* ListAllocator::alloc(UINT32 size, UINT32 alignment)
    {
        size = max(size, (UINT32)sizeof(Block));
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");
        ASSERT((alignment - 1) & alignment == 0, "ListAllocator: Alignment should be power of 2")

        return getBlock(size);
    }

    template <typename Element>
    Element* ListAllocator::alloc()
    {
        UINT32 size = max(sizeof(Element), sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        auto element = (Element*)getBlock(size);
        return element;
    }

    template <typename Element>
    Element* ListAllocator::allocCopy(const Element* data)
    {
        UINT32 size = max(sizeof(Element), sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        auto element = (Element*)getBlock(size);
        memcpy(element, data, sizeof(Element));
        return element;
    }

    void* ListAllocator::free(void* block, UINT32 size)
    {
        size = max(size, (UINT32)sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        returnBlock(block, size);

        return nullptr;
    }

    void* ListAllocator::free(void* block, UINT32 size, UINT32 alignment)
    {
        size = max(size, (UINT32)sizeof(Block));
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));

        ASSERT((alignment - 1) & alignment == 0, "ListAllocator: Alignment should be power of 2")

        returnBlock(block, size);

        return nullptr;
    }

    template <typename Element>
    void* ListAllocator::free(Element* element)
    {
        UINT32 size = max(sizeof(Element), sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        element->~Element();
        returnBlock(element, size);

        return nullptr;
    }

    void ListAllocator::uniteBlocks()
    {
        UINT32 index = 0;
        Block* tmp_print = mListOfFreeBlocks;
        while (tmp_print != nullptr)
        {
            printf("Block i=%i p=%p n=%p s=%u p+s=%p\n", index, tmp_print, tmp_print->next, tmp_print->size, (INT8*)tmp_print + tmp_print->size);
            index += 1;
            tmp_print = tmp_print->next;
        }

        index = 0;
        tmp_print = mFirst;
        while (tmp_print != nullptr)
        {
            printf("Block i=%i p=%p n=%p s=%u p+s=%p\n", index, tmp_print, tmp_print->next, tmp_print->size, (INT8*)tmp_print + tmp_print->size);
            index += 1;
            tmp_print = tmp_print->next;
        }

        if (mListOfFreeBlocks == nullptr)
        {
            return;
        }

        if (mFirst == nullptr)
        {
            mFirst = mListOfFreeBlocks;
            mListOfFreeBlocks = mListOfFreeBlocks->next;

            mFirst->next = nullptr;
            mFirst->prev = nullptr;

            if (mListOfFreeBlocks != nullptr)
            { mListOfFreeBlocks->prev = nullptr; }
        }

        Block* tmp = mListOfFreeBlocks;
        while (tmp != nullptr)
        {
            Block* iteration_prev = nullptr;
            Block* iteration = mFirst;
            bool was_found = false;

            while (iteration != nullptr)
            {
                if (tmp < iteration)
                {
                    Block* to_insert = tmp;
                    UINT32 size = tmp->size;
                    tmp = tmp->next;

                    if (iteration_prev)
                    { iteration_prev->next = to_insert; }
                    else
                    { mFirst = to_insert; }

                    to_insert->prev = iteration->prev;
                    to_insert->next = iteration;
                    to_insert->size = size;
                    iteration->prev = to_insert;

                    was_found = true;
                    break;
                }

                iteration_prev = iteration;
                iteration = iteration->next;
            }

            if (!was_found)
            {
                iteration_prev->next = tmp;

                tmp = tmp->next;

                iteration_prev->next->next = nullptr;
                iteration_prev->next->prev = iteration_prev;
            }
        }

        index = 0;
        tmp_print = mFirst;
        while (tmp_print != nullptr)
        {
            printf("Block i=%i p=%p n=%p s=%u p+s=%p\n", index, tmp_print, tmp_print->next, tmp_print->size, (INT8*)tmp_print + tmp_print->size);
            index += 1;
            tmp_print = tmp_print->next;
        }

        Block* iterator = mFirst;
        while (iterator != nullptr)
        {
            // Can we unite that ?
            if ((INT8*)iterator + iterator->size == (INT8*)(iterator->next))
            {
                iterator->size = iterator->size + iterator->next->size;
                iterator->next = iterator->next->next;
            }
            else
            {
                iterator = iterator->next;
            }
        }

        mListOfFreeBlocks = nullptr;
    }

    void*  ListAllocator::getBuffer() const
    {
        return (void*)mBuffer;
    }

    UINT32 ListAllocator::getExpandBufferSize() const
    {
        return mExpandSize;
    }

    UINT32 ListAllocator::getNumOfBuffers() const
    {
        return mNumOfBuffers;
    }

    UINT32 ListAllocator::getBufferSize() const
    {
        return mBufferSize;
    }

    UINT32 ListAllocator::getMemoryUsage() const
    {
        return mNumOfBuffers * mExpandSize;
    }

    UINT32 ListAllocator::getMaxAvailableMemory() const
    {
        return mNumOfBuffers * mBufferSize;
    }

    UINT32 ListAllocator::getAllocatedMemory() const
    {
        return mAllocatedMemory;
    }

    UINT32 ListAllocator::getMinimalBlockSize() const
    {
        return sizeof(Block);
    }

    void ListAllocator::printBlocksInfo() const
    {
        if (mFirst == nullptr)
        {
            PUSH("Blocks for alloc (null) | used mem %u | free memory %u \n", mAllocatedMemory, getMaxAvailableMemory() - mAllocatedMemory);
        }
        else
        {
            OPEN_BLOCK("Blocks for alloc");

            UINT32 i = 0;
            Block* tmp = mFirst;
            while (tmp != nullptr)
            {
                PUSH_BLOCK("Block %u: add %p prev %p next %p size %u", i, tmp, tmp->prev, tmp->next, tmp->size);

                tmp = tmp->next;
                i += 1;
            }

            CLOSE_BLOCK();

            PUSH("Total alloc: used mem %u | free mem %u \n", mAllocatedMemory, getMaxAvailableMemory() - mAllocatedMemory);
        }
    }

    void ListAllocator::printFreeBlockInfo() const
    {
        if (mListOfFreeBlocks == nullptr)
        {
            PUSH("Blocks for free (null) | used mem %u | free memory %u \n", mAllocatedMemory, getMaxAvailableMemory() - mAllocatedMemory);
        }
        else
        {
            OPEN_BLOCK("Blocks for free");

            UINT32 i = 0;
            Block* tmp = mListOfFreeBlocks;
            while (tmp != nullptr)
            {
                PUSH_BLOCK("Block %u: add %p prev %p next %p size %u", i, tmp, tmp->prev, tmp->next, tmp->size);

                tmp = tmp->next;
                i += 1;
            }

            CLOSE_BLOCK();

            PUSH("Total free: used mem %u | free mem %u \n", mAllocatedMemory, getMaxAvailableMemory() - mAllocatedMemory);
        }
    }

    void ListAllocator::printBuffersInfo() const
    {
        if (mBuffer == nullptr)
        {
            PUSH("Buffer null: buffer size %u | expand size %u \n");
        }
        else
        {
            OPEN_BLOCK("Buffers info");

            UINT32 i = 0;
            Buffer* tmp = mBuffer;
            while (tmp != nullptr)
            {
                PUSH_BLOCK("Buffer %u: add %p next %p data %p", i, tmp, tmp->next, &(tmp->data));

                tmp = tmp->next;
                i += 1;
            }

            CLOSE_BLOCK();

            PUSH("Total Buffers: count %u total used memory %u \n", mNumOfBuffers, mNumOfBuffers * mExpandSize);
        }
    }

    void ListAllocator::returnBlock(void* block, UINT32 size)
    {
        if (mListOfFreeBlocks == nullptr)
        {
            mListOfFreeBlocks = (Block*)block;
            mListOfFreeBlocks->next = nullptr;
            mListOfFreeBlocks->prev = nullptr;
            mListOfFreeBlocks->size = size;
        }
        else
        {
            auto tmp = (Block*)block;
            tmp->next = mListOfFreeBlocks;
            tmp->size = size;
            tmp->prev = nullptr;
            mListOfFreeBlocks->prev = tmp;
            mListOfFreeBlocks = tmp;
        }

        mAllocatedMemory -= size;
    }

    void ListAllocator::createList()
    {
        mBuffer = (Buffer*)mem_alloc(mExpandSize);
        mBuffer->next = nullptr;

        mFirst = &(mBuffer->data);
        mFirst->next = nullptr;
        mFirst->prev = nullptr;
        mFirst->size = mBufferSize;

        mNumOfBuffers += 1;
    }

    void ListAllocator::expand()
    {
        Buffer* new_buffer = (Buffer*)mem_alloc(mExpandSize);
        new_buffer->next = mBuffer;
        mBuffer = new_buffer;

        mNumOfBuffers += 1;

        Block* new_one = &(mBuffer->data);
        
        if (new_one < mFirst || mFirst == nullptr)
        {
            new_one->size = mBufferSize;
            new_one->prev = nullptr;
            new_one->next = mFirst;

            if (mFirst)
            { mFirst->prev = new_one; }

            mFirst = new_one;
        }
        else
        {
            Block* prv = nullptr;
            Block* tmp = mFirst;
            while (tmp != nullptr)
            {
                if (new_one < tmp)
                {
                    tmp->prev->next = new_one;
                    new_one->prev = tmp->prev;
                    new_one->next = tmp;
                    tmp->prev = new_one;
                    new_one->size = mBufferSize;

                    return;
                }

                prv = tmp;
                tmp = tmp->next;
            }

            prv->next = new_one;
            new_one->prev = prv;
            new_one->next = nullptr;
            new_one->size = mBufferSize;
        }
    }

    void* ListAllocator::getBlock(UINT32 size)
    {
        if (size > mBufferSize)
        {
            ERROR("ListAllocator: Ask for big block : cannot alloc %u with buffer size %u", size, mBufferSize);
            return nullptr;
        }

        Block* n1 = nullptr;
        Block* tmp = mFirst;

        while (tmp != nullptr)
        {
            if (tmp->size >= size)
            {
                if (tmp->size < size + sizeof(Block))
                {
                    n1 = tmp;

                    if (tmp->next != nullptr)
                    { tmp->next->prev = tmp->prev; }

                    if (tmp->prev != nullptr)
                    { tmp->prev->next = tmp->next; }
                    else
                    { mFirst = tmp->next; }

                    break;
                }
                else
                {
                    INT8* dt = ((INT8*)tmp + size);
                    Block* n2 = (Block*)dt;

                    n1 = tmp;

                    n2->next = tmp->next;
                    n2->prev = tmp->prev;
                    n2->size = tmp->size - size;

                    if (tmp->next)
                    { tmp->next->prev = n2; }

                    if (tmp->prev != nullptr)
                    { tmp->prev->next = n2; }
                    else
                    { mFirst = n2; }

                    break;
                }
            }
            else
            {
                tmp = tmp->next;
            }
        }

        if (n1 != nullptr)
        {
            mAllocatedMemory += size;
            return n1;
        }
        else
        {
            INFO("ListAllocator: add %p expand buffer", this);
            expand();
            return getBlock(size);
        }
    }

} // namespace Berserk