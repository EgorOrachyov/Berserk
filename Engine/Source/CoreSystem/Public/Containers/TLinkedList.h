//
// Created by Egor Orachyov on 03.06.2019.
//

#ifndef BERSERK_TLINKEDLIST_H
#define BERSERK_TLINKEDLIST_H

#include <Misc/Assert.h>
#include <Object/NewDelete.h>
#include <Containers/TList.h>
#include <Memory/Allocator.h>

namespace Berserk
{

    /**
     * Dynamically expandable linked list for elements of type T.
     *
     * Elements should be of re-allocatable type and support safe memory move without copy-constructor.
     * Automatically expands in the add method whether it does not have enough space in the internal buffer.
     * Relies on the engine memory allocator.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Template type for elements of the list
     */
    template <typename T>
    class CORE_EXPORT TLinkedList : public TList<T>
    {
    public:

        /** Compare predicate type */
        typedef bool (*Predicate)(const T& a, const T& b);

    public:

        GENERATE_NEW_DELETE(TLinkedList);

        /**
         * Initialize empty list with internal allocator
         * @param allocator Allocator for internal nodes
         */
        explicit TLinkedList(IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator)
        {

        }

        /**
         * Initialize container from raw C-style array
         * @param array Pointer to buffer with elements
         * @param count Num of elements in buffer
         * @param allocator Allocator for internal nodes
         */
        TLinkedList(const T* array, uint32 count, IAllocator& allocator = Allocator::getSingleton())
                : mAllocator(allocator)
        {
            append(array, count);
        }

        /** Prohibited */
        TLinkedList(const TLinkedList& list) = delete;

        /** Prohibited */
        TLinkedList(const TLinkedList&& list) = delete;

        ~TLinkedList() override
        {
            if (mHead != nullptr)
            {
                clear();
            }
        }

        /** @copydoc TList::add() */
        void add(const T &element) override
        {
            if (mHead != nullptr)
            {
                void* memory = mAllocator.allocate(sizeof(Node));
                Node* node = new(memory) Node(element);
                mTail->addAfter(node);
                mTail = node;
            }
            else
            {
                void* memory = mAllocator.allocate(sizeof(Node));
                Node* node = new(memory) Node(element);
                mHead = node;
                mTail = node;
            }

            mSize += 1;
        }

        /** @copydoc TList::addUninitialized() */
        T *addUninitialized() override
        {
            Node* node = nullptr;

            if (mHead != nullptr)
            {
                void* memory = mAllocator.allocate(sizeof(Node));
                node = new(memory) Node();
                mTail->addAfter(node);
                mTail = node;
            }
            else
            {
                void* memory = mAllocator.allocate(sizeof(Node));
                node = new(memory) Node();
                mHead = node;
                mTail = node;
            }

            mSize += 1;
            return node->data();
        }

        /** @copydoc TList::append() */
        void append(const TList<T> &container) override
        {
            for (T* itr = container.begin(); itr != nullptr; itr = container.next())
            {
                add(*itr);
            }
        }

        /** @copydoc TList::append() */
        void append(const T *array, uint32 count) override
        {
            for (uint32 i = 0; i < count; i++)
            {
                add(array[i]);
            }
        }

        /** @copydoc TList::get() */
        T &get(uint32 index) const override
        {
            rangeCheck(index);

            uint32 i = 0;
            Node* current = mHead;
            while (current != nullptr)
            {
                if (i == index)
                {
                    return *current->data();
                }

                i += 1;
                current = current->next();
            }
        }

        /** @copydoc TList::remove() */
        void remove(uint32 index) override
        {
            rangeCheck(index);

            uint32 i = 0;
            Node* current = mHead;
            while (current != nullptr)
            {
                if (i == index)
                {
                    current->data()->~T();

                    if (mSize == 1)
                    {
                        mHead = nullptr;
                        mTail = nullptr;
                    }
                    else if (current == mHead)
                    {
                        mHead = current->next();
                    }
                    else if (current == mTail)
                    {
                        mTail = current->prev();
                    }

                    mSize -= 1;
                    current->remove();
                    mAllocator.free(current);

                    return;
                }

                i += 1;
                current = current->next();
            }
        }

        /** @copydoc TList::clear() */
        void clear() override
        {
            Node* current = mHead;
            while (current != nullptr)
            {
                Node* next = current->next();
                current->data()->~T();
                mAllocator.free(current);
                current = next;
            }
            mHead = nullptr;
            mTail = nullptr;
            mSize = 0;
        }

        /** @copydoc TList::sort() */
        void sort(Predicate predicate) override
        {

        }

        /** @copydoc TList::getSize() */
        uint32 getSize() const override
        {
            return mSize;
        }

        /** @return Size of internal list node */
        static uint32 getNodeSize()
        {
            return sizeof(Node);
        }

        /** @copydoc TList::getMemoryUsage() */
        uint32 getMemoryUsage() const override
        {
            return mSize * sizeof(Node);
        }

        /** @copydoc TList::begin() */
        T *begin() const override
        {
            mIterator = mHead;
            return (mIterator != nullptr ? mIterator->data() : nullptr);
        }

        /** @copydoc TList::next() */
        T *next() const override
        {
            mIterator = (mIterator != nullptr ? mIterator->next() : nullptr);
            return (mIterator != nullptr ? mIterator->data() : nullptr);
        }

    private:

        /** Assert fail on index out of range */
        void rangeCheck(uint32 index) const
        {
            // todo: add Debug class
            assert(index < mSize);
        }

        /** List template node to STORE data */
        struct Node
        {
        public:

            GENERATE_NEW_DELETE(Node);

            /**
             * Creates node with uninitialized data T content
             * to be initialized later (used for addUninitialized)
             */
            Node()
            {

            }

            /**
             * Initialize node and move (i.e. copy content / memory of
             * the object T inside this node)
             * @param data Template data T
             */
            Node(const T &data)
            {
                memcpy(&mData[0], &data, sizeof(T));
            }

            /** @return True if next nod exist */
            bool hasNext() const { return mNext != nullptr; }

            /** @return True if previous nod exist */
            bool hasPrev() const { return mPrev != nullptr; }

            /** Adds node after this one */
            void addAfter(Node* node)
            {
                mNext = node;
                node->mPrev = this;
            }

            /** Adds node before this one */
            void addBefore(Node* node)
            {
                mPrev = node;
                node->mNext = this;
            }

            /** Removes this node from nodes list */
            void remove()
            {
                if (mPrev != nullptr) mPrev->mNext = mNext;
                if (mNext != nullptr) mNext->mPrev = mPrev;
            }

            /** @return Pointer to next node */
            Node* next() const { return mNext; }

            /** @return Pointer to previous node */
            Node* prev() const { return mPrev; }

            /** @return Pointer to stored data */
            T* data() { return (T*) &mData[0]; }

        private:

            /** New node in the list or null */
            Node* mNext = nullptr;

            /** New node in the list or null */
            Node* mPrev = nullptr;

            /** Stored data (memory) */
            char mData[sizeof(T)];

        };

    private:

        /** Used to allocate memory for nodes */
        IAllocator& mAllocator;

        /** Number of nodes in the list */
        uint32 mSize = 0;

        /** List head - always does not have prev */
        class Node* mHead = nullptr;

        /** List tail - always does not have next */
        class Node* mTail = nullptr;

        /** For iterator logic of this container */
        mutable class Node* mIterator = nullptr;


    };

} // namespace Berserk

#endif //BERSERK_TLINKEDLIST_H