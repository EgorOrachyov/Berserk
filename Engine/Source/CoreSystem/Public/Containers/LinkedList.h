//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_LINKEDLIST_H
#define BERSERK_LINKEDLIST_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Object/NewDelete.h"
#include "Memory/PoolAllocator.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    /**
     * Dynamically expandable linked list for elements of type T.
     * Automatically expands in the add method whether it does not have enough
     * space in the internal buffer. Relies on the engine pool allocator.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Type of stored elements
     */
    template <typename T>
    class LinkedList
    {
    private:

        struct Node
        {
            Node* next;
            T data;
        };

    public:

        static const uint32 MIN_INITIAL_SIZE = PoolAllocator::INITIAL_CHUNK_COUNT;

        LinkedList()
        {
            mSize = 0;
            mHead = nullptr;
            mTail = nullptr;
            mIterator = nullptr;
        }

        GEN_NEW_DELETE(LinkedList);

    public:

        /**
         * Creates linked list and initializes internal allocator with desired
         * number of memory chunks for list nodes
         * @param initialSize Number of nodes at the beginning of list work
         */
        explicit LinkedList(uint32 initialSize);

        ~LinkedList();

        /**
         * Deletes element with index and call Destructor for that
         * @warning ASSERT on range check
         * @param index
         */
        void remove(uint32 index);

        /**
         * Deletes element with specified data pointer
         * @param element
         */
        void remove(T* element);

        /** Removes first element of the list */
        void removeFirst();

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
        T operator [] (uint32 index);

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

    private:

        uint32  mSize;
        Node*   mHead;
        Node*   mTail;
        Node*   mIterator;
        PoolAllocator mPool;

    };

    template <typename T>
    LinkedList<T>::LinkedList(uint32 initialSize) :
            mPool(Math::max(PoolAllocator::MIN_CHUNK_SIZE,(uint32)(sizeof(Node))),
                  Math::max(PoolAllocator::INITIAL_CHUNK_COUNT,initialSize))
    {
        mSize = 0;
        mHead = nullptr;
        mTail = nullptr;
        mIterator = nullptr;
    }

    template <typename T>
    LinkedList<T>::~LinkedList()
    {
        empty();
        PUSH("Linked List: delete list");
    }

    template <typename T>
    void LinkedList<T>::remove(uint32 index)
    {
        FAIL(index < mSize, "Index out of range %u", index);

        if (index == 0)
        {
            auto block = mHead;

            mHead->data.~T();
            mHead = mHead->next;

            mPool.free(block);
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

        if (next == mTail)
        {
            mTail = current;
            mTail->next = nullptr;
        }

        mPool.free(next);
        mSize -= 1;
    }

    template <typename T>
    void LinkedList<T>::remove(T* element)
    {
        if (mHead == nullptr)
        {
            return;
        }

        if (&mHead->data == element)
        {
            auto block = mHead;

            mHead->data.~T();
            mSize -= 1;

            if (mHead == mTail)
            {
                mHead = mTail = nullptr;
            }
            else
            {
                mHead = mHead->next;
            }

            mPool.free(block);

            return;
        }

        Node* current = mHead;
        Node* next = current->next;

        while (next)
        {
            if (&next->data == element)
            {
                auto block = next;

                next->data.~T();
                mSize -= 1;

                if (next == mTail)
                {
                    mTail = current;
                    mTail->next = nullptr;
                }
                else
                {
                    current->next = next->next;
                }

                mPool.free(block);

                return;
            }

            current = next;
            next = next->next;
        }
    }

    template <typename T>
    void LinkedList<T>::removeFirst()
    {
        if (mHead)
        {
            mHead->data.~T();
            mHead = mHead->next;

            mSize -= 1;
        }

        if (mHead == nullptr) mTail = nullptr;
    }

    template <typename T>
    void LinkedList<T>::empty()
    {
        auto current = mHead;
        while (current)
        {
            auto next = current->next;
            current->data.~T();
            mPool.free(current);
            current = next;
        }

        mHead = mTail = nullptr;
        mSize = 0;
    }

    template <typename T>
    void LinkedList<T>::addHead(const T& element)
    {
        auto block = (Node*) mPool.alloc();
        memcpy(&block->data, &element, sizeof(T));
        block->next = mHead;
        mHead = block;

        mSize += 1;

        if (mTail == nullptr) { mTail = mHead; }
    }

    template <typename T>
    void LinkedList<T>::addTail(const T& element)
    {
        auto block = (Node*) mPool.alloc();
        memcpy(&block->data, &element, sizeof(T));
        block->next = nullptr;
        mTail->next = block;
        mTail = block;

        mSize += 1;
    }

    template <typename T>
    void LinkedList<T>::operator+=(const T& element)
    {
        auto block = (Node*) mPool.alloc();
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
    T LinkedList<T>::operator[](uint32 index)
    {
        FAIL(index < mSize, "Index out of range %u", index);

        uint32 i = 0;
        auto current = mHead;
        while (i < index)
        {
            current = current->next;
            i += 1;
        }

        return current->data;
    }

    template <typename T>
    T* LinkedList<T>::getFirst()
    {
        FAIL(mHead, "List is empty");

        return &(mHead->data);
    }

    template <typename T>
    T* LinkedList<T>::getLast()
    {
        FAIL(mTail, "List is empty");

        return &(mTail->data);
    }

    template <typename T>
    T* LinkedList<T>::iterate()
    {
        mIterator = mHead;
        return (mIterator ? &(mIterator->data) : nullptr);
    }

    template <typename T>
    T* LinkedList<T>::next()
    {
        mIterator = mIterator->next;
        return (mIterator ? &(mIterator->data) : nullptr);
    }

    template <typename T>
    uint32 LinkedList<T>::getSize() const
    {
        return mSize;
    }

    template <typename T>
    uint32 LinkedList<T>::getTotalSize() const
    {
        return mPool.getTotalSize() / mPool.getChunkSize();
    }

    template <typename T>
    uint32 LinkedList<T>::getMemoryUsage() const
    {
        return mPool.getTotalSize();
    }

} // namespace Berserk

#endif //BERSERK_LINKEDLIST_H