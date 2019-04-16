//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_SHAREDLIST_H
#define BERSERK_SHAREDLIST_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Object/NewDelete.h"
#include "Memory/PoolAllocator.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    /**
     * Dynamically expandable shared list for elements of type T.
     * Automatically expands in the add method whether it does not have enough
     * space in the internal buffer. Relies on the engine pool allocator.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * It gets pool allocator and uses that for allocation of memory for its
     * internal nodes. Note: that one pool allocator could serve for different
     * number of shared lists;
     *
     * @tparam T Type of stored elements
     */
    template <typename T>
    class SharedList
    {
    public:

        struct Node
        {
            Node* next;
            T data;
        };

    public:

        SharedList()
        {
            mSize = 0;
            mHead = nullptr;
            mTail = nullptr;
            mIterator = nullptr;
            mPool = nullptr;
        }

        GENARATE_NEW_DELETE(SharedList);

    public:

        /**
         * Creates shared linked list for and uses for allocations
         * PoolAllocator pool
         * @param pool Initialized pool allocator
         */
        explicit SharedList(PoolAllocator* pool);

        ~SharedList();

        /**
         * Deletes element with index and call Destructor for that
         * @warning ASSERT on range check
         * @param index
         */
        void remove(uint32 index);

        /** Removes all the elements and calls default destructors */
        void empty();

        /** Add before head */
        void addHead(const T& element);

        /** Add after tail */
        void addTail(const T& element);

        /** Add in the end f the list */
        void operator += (const T& element);

        /**
         * Get element with index
         * @warning ASSERT on range check
         * @param index
         * @return Element
         */
        T* operator [](uint32 index);

        /** @return Element in the head */
        T* getFirst();

        /** @return Element in the tail */
        T* getLast();

        /** @return Start iterating */
        T* iterate();

        /** @return Next element in the iteration or nullptr */
        T* next();

        /** @return Number of elements */
        uint32 getSize() const;

        /** @return Total number of elements (chunks) in pool  */
        uint32 getTotalSize() const;

        /** @return Total number of bytes allocated in the heap  */
        uint32 getMemoryUsage() const;

        /** @return Size of internal node */
        static uint32 getNodeSize() { return sizeof(Node); }

    private:

        uint32  mSize;
        Node*   mHead;
        Node*   mTail;
        Node*   mIterator;
        PoolAllocator* mPool;

    };

    template <typename T>
    SharedList<T>::SharedList(PoolAllocator *pool)
    {
        FAIL(pool != nullptr, "Pool allocator is null");
        FAIL(pool->getChunkSize() >= sizeof(Node), "Pool allocator chunk size less than %lu (arg: %u)", sizeof(Node), pool->getChunkSize());

        mHead = nullptr;
        mTail = nullptr;
        mIterator = nullptr;

        mSize = 0;
        mPool = pool;
    }

    template <typename T>
    SharedList<T>::~SharedList()
    {
        empty();
        PUSH("Shared List: delete list pool: %p", mPool);
    }


    template <typename T>
    void SharedList<T>::remove(uint32 index)
    {
        FAIL(index < mSize, "Index out of range %u", index);

        if (index == 0)
        {
            auto block = mHead;

            mHead->data.~T();
            mHead = mHead->next;

            mPool->free(block);
            mSize -= 1;

            if (mSize == 0) mTail = nullptr;

            return;
        }

        uint32 i = 1;
        auto current = mHead;
        auto next = mHead->next;
        while (next && i < index)
        {
            current = next;
            next = next->next;
            i += 1;
        }

        next->data.~T();
        current->next = next->next;

        if (next == mTail) mTail = current;

        mPool->free(next);
        mSize -= 1;
    }

    template <typename T>
    void SharedList<T>::empty()
    {
        auto current = mHead;
        while (current)
        {
            auto next = current->next;
            current->data.~T();
            mPool->free(current);
            current = next;
        }

        mHead = mTail = nullptr;
        mSize = 0;
    }

    template <typename T>
    void SharedList<T>::addHead(const T& element)
    {
        auto block = (Node*) mPool->allocate(0);
        memcpy(&block->data, &element, sizeof(T));
        block->next = mHead;
        mHead = block;

        mSize += 1;

        if (mTail == nullptr) { mTail = mHead; }
    }

    template <typename T>
    void SharedList<T>::addTail(const T& element)
    {
        auto block = (Node*) mPool->allocate(0);
        memcpy(&block->data, &element, sizeof(T));
        block->next = nullptr;
        mTail->next = block;
        mTail = block;

        mSize += 1;
    }

    template <typename T>
    void SharedList<T>::operator+=(const T& element)
    {
        auto block = (Node*) mPool->allocate(0);
        memcpy(&block->data, &element, sizeof(T));
        block->next = nullptr;

        if (mTail)
        {
            mTail->next = block;
            mTail = block;
        }
        else
        {
            mHead = mTail = block;
        }

        mSize += 1;
    }

    template <typename T>
    T* SharedList<T>::operator[](uint32 index)
    {
        FAIL(index < mSize, "Index out of range %u", index);

        uint32 i = 0;
        auto current = mHead;
        while (i < index)
        {
            current = current->next;
            i += 1;
        }

        return &(current->data);
    }

    template <typename T>
    T* SharedList<T>::getFirst()
    {
        if (!mHead) return nullptr;
        return &(mHead->data);
    }

    template <typename T>
    T* SharedList<T>::getLast()
    {
        if (!mTail) return nullptr;
        return &(mTail->data);
    }

    template <typename T>
    T* SharedList<T>::iterate()
    {
        mIterator = mHead;
        return (mIterator ? &(mIterator->data) : nullptr);
    }

    template <typename T>
    T* SharedList<T>::next()
    {
        mIterator = mIterator->next;
        return (mIterator ? &(mIterator->data) : nullptr);
    }

    template <typename T>
    uint32 SharedList<T>::getSize() const
    {
        return mSize;
    }

    template <typename T>
    uint32 SharedList<T>::getTotalSize() const
    {
        return (uint32)mPool->getTotalMemoryUsage() / mPool->getChunkSize();
    }

    template <typename T>
    uint32 SharedList<T>::getMemoryUsage() const
    {
        return (uint32)mPool->getTotalMemoryUsage();
    }

} // namespace Berserk

#endif //BERSERK_SHAREDLIST_H