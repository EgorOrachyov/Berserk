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
    }

    void* ListAllocator::alloc(UINT32 size)
    {
        size = max(size + (UINT32)sizeof(UINT64), (UINT32)sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        return getBlock(size);
    }

    void* ListAllocator::alloc(UINT32 size, UINT32 alignment)
    {
        size = max(size + (UINT32)sizeof(UINT64), (UINT32)sizeof(Block));
        size = size + ((size & (alignment - 1)) != 0) * (alignment - (size % alignment));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");
        ASSERT((alignment - 1) & alignment == 0, "ListAllocator: Alignment should be power of 2")

        return getBlock(size);
    }

    template <typename Element>
    Element* ListAllocator::alloc()
    {
        UINT32 size = max(sizeof(Element) + (UINT32)sizeof(UINT64), sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        auto element = (Element*)getBlock(size);
        return element;
    }

    template <typename Element>
    Element* ListAllocator::allocCopy(const Element* data)
    {
        UINT32 size = max(sizeof(Element) + (UINT32)sizeof(UINT64), sizeof(Block));
        size = size + ((size & (MEMORY_ALIGNMENT - 1)) != 0) * (MEMORY_ALIGNMENT - (size % MEMORY_ALIGNMENT));

        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        auto element = (Element*)getBlock(size);
        memcpy(element, data, sizeof(Element));
        return element;
    }

    void* ListAllocator::free(void* block)
    {
        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        returnBlock(block);

        return nullptr;
    }

    template <typename Element>
    void* ListAllocator::free(Element* element)
    {
        ASSERT(mBuffer != nullptr, "ListAllocator: Buffer should be initialized");

        element->~Element();
        returnBlock(element);

        return nullptr;
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
            PUSH("Blocks for alloc (null) | used mem %u | free memory %u", mAllocatedMemory, getMaxAvailableMemory() - mAllocatedMemory);
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

            PUSH_BLOCK("Total alloc: used mem %u | free mem %u", mAllocatedMemory, getMaxAvailableMemory() - mAllocatedMemory);

            CLOSE_BLOCK();
        }
    }

    void ListAllocator::printBuffersInfo() const
    {
        if (mBuffer == nullptr)
        {
            PUSH("Buffer null: buffer size %u | expand size %u");
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

            PUSH_BLOCK("Total Buffers: count %u total used memory %u", mNumOfBuffers, mNumOfBuffers * mExpandSize);

            CLOSE_BLOCK();
        }
    }

    void ListAllocator::returnBlock(void* block)
    {
        INT8* calculation = (INT8*)block - sizeof(UINT64);
        auto ab = (Allocable_Block*)calculation;

        // printf("Debug -- Get %p Count %p Should %p \n", block, calculation, &ab->data);

        mAllocatedMemory -= ab->size;

        if (mFirst == nullptr)
        {
            mFirst = (Block*)ab;
            mFirst->prev = nullptr;
            mFirst->next = nullptr;
            return;
        }

        auto to_return = (Block*)ab;
        auto tmp = mFirst;
        auto last = tmp->prev;

        while (tmp != nullptr)
        {
            if (tmp->prev)
            {
                if (tmp->prev < to_return && to_return < tmp)
                {
                    auto prev = tmp->prev;
                    auto curr = to_return;
                    auto next = tmp;

                    prev->next = curr;
                    curr->prev = prev;
                    curr->next = next;
                    next->prev = curr;

                    if ((INT8*)prev + prev->size == (INT8*)curr)
                    {
                        prev->size = prev->size + curr->size;
                        prev->next = curr->next;
                        next->prev = prev;
                        curr = prev;
                    }

                    if ((INT8*)curr + curr->size == (INT8*)next)
                    {
                        curr->next = next->next;
                        curr->size = curr->size + next->size;
                        if (next->next) next->next->prev = curr;
                    }

                    return;
                }
                else
                {
                    last = tmp;
                    tmp = tmp->next;
                }
            }
            else
            {
                if (to_return < tmp)
                {
                    auto curr = to_return;
                    auto next = tmp;

                    curr->prev = nullptr;
                    curr->next = next;
                    next->prev = curr;

                    if ((INT8*)curr + curr->size == (INT8*)next)
                    {
                        curr->next = next->next;
                        curr->size = curr->size + next->size;
                        if (next->next) next->next->prev = curr;
                    }

                    mFirst = curr;

                    return;
                }
                else
                {
                    last = tmp;
                    tmp = tmp->next;
                }
            }
        }

        {
            // did not find needed place (insert in the end)
            auto prev = last;
            auto curr = to_return;

            prev->next = curr;
            curr->prev = prev;
            curr->next = nullptr;

            if ((INT8*)prev + prev->size == (INT8*)curr)
            {
                prev->size = prev->size + curr->size;
                prev->next = nullptr;
            }
        }
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
                    {
                        mFirst = tmp->next;
                        if (mFirst) mFirst->prev = nullptr;
                    }

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

                    n1->size = size;

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
            auto to_return = (Allocable_Block*)n1;

            // printf("Debug -- Was %p Return %p \n", n1, &(to_return->data));

            return &(to_return->data);
        }
        else
        {
            // INFO("ListAllocator: add %p expand buffer", this);
            expand();

            // printf("Expand ");

            return getBlock(size);
        }
    }

} // namespace Berserk