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

#ifndef BERSERK_TLINKEDLIST_H
#define BERSERK_TLINKEDLIST_H

#include <Containers/TList.h>
#include <Memory/Allocator.h>
#include <Misc/Error.h>

namespace Berserk {

    /**
     * Dynamically expandable linked list for elements of type T.
     *
     * Elements should be of re-allocatable type and support safe memory move without copy-constructor.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Template type for elements of the list
     */
    template<typename T>
    class TLinkedListBase : public TList<T> {
    public:

        /** When to stop recursive list split for merge sort */
        static const uint32 STOP_RECURSIVE_SORT = 8;

        /** Compare predicate type */
        typedef bool (*Predicate)(const T &a, const T &b);

        /** Find predicate type */
        typedef bool (*Satisfy)(const T &a);

    public:

        /**
         * Initialize empty list with internal allocator
         * @param allocator Allocator for internal nodes
         */
        explicit TLinkedListBase(IAllocator &allocator = Allocator::getSingleton())
                : mAllocator(allocator) {

        }

        /**
         * Initialize container from raw C-style array
         * @param array Pointer to buffer with elements
         * @param count Num of elements in buffer
         * @param allocator Allocator for internal nodes
         */
        TLinkedListBase(const T *array, uint32 count, IAllocator &allocator = Allocator::getSingleton())
                : mAllocator(allocator) {
            append(array, count);
        }

        /** Copy content of source list */
        TLinkedListBase(TLinkedListBase &list) : TLinkedListBase(list.mAllocator) {
            append(list);
        }

        /** Move list content to this list */
        TLinkedListBase(TLinkedListBase &&list) noexcept
                : mAllocator(list.mAllocator),
                  mSize(list.mSize),
                  mHead(list.mHead),
                  mTail(list.mTail) {
            list.mSize = 0;
            list.mHead = nullptr;
            list.mTail = nullptr;
            list.mIterator = nullptr;
        }

        ~TLinkedListBase() override {
            if (mHead != nullptr) {
                clear();
            }
        }

        /** @copydoc TList::add() */
        void add(const T &element) override {
            if (mHead != nullptr) {
                void *memory = mAllocator.malloc(sizeof(Node));
                Node *node = new(memory) Node();
                mTail->linkAfter(node);
                mTail = node;

                new(node->data()) T(element);
            } else {
                void *memory = mAllocator.malloc(sizeof(Node));
                Node *node = new(memory) Node();
                mHead = node;
                mTail = node;

                new(node->data()) T(element);
            }

            mSize += 1;
        }

        /** @copydoc TList::addUninitialized() */
        T *addUninitialized() override {
            Node *node = nullptr;

            if (mHead != nullptr) {
                void *memory = mAllocator.malloc(sizeof(Node));
                node = new(memory) Node();
                mTail->linkAfter(node);
                mTail = node;
            } else {
                void *memory = mAllocator.malloc(sizeof(Node));
                node = new(memory) Node();
                mHead = node;
                mTail = node;
            }

            mSize += 1;
            return node->data();
        }

        /** @copydoc TList::append() */
        void append(const T *array, uint32 count) override {
            for (uint32 i = 0; i < count; i++) {
                add(array[i]);
            }
        }

        /** @copydoc TList::append() */
        void append(const std::initializer_list<T> &list) override {
            for (const T& element : list) {
                add(element);
            }
        }

        /** @copydoc TList::get() */
        T &get(uint32 index) const override {
            rangeCheck(index);

            uint32 i = 0;
            Node *current = mHead;
            while (current != nullptr) {
                if (i == index) {
                    return *current->data();
                }

                i += 1;
                current = current->next();
            }
        }

        /** @copydoc TList::find() */
        TVariant<T*> find(const typename TPredicate::Satisfy<T>::type &predicate) const override {
            Node *current = mHead;
            while (current != nullptr) {
                if (predicate(*current->data())) {
                    T* ptr = current->data();
                    return TVariant<T*>(ptr);
                }

                current = current->next();
            }

            return TVariant<T*>();
        }

        /** @copydoc TList::remove() */
        void remove(uint32 index) override {
            rangeCheck(index);

            uint32 i = 0;
            Node *current = mHead;
            while (current != nullptr) {
                if (i == index) {
                    current->data()->~T();

                    if (mSize == 1) {
                        mHead = nullptr;
                        mTail = nullptr;
                    } else if (current == mHead) {
                        mHead = current->next();
                    } else if (current == mTail) {
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
        void clear() override {
            Node *current = mHead;
            while (current != nullptr) {
                Node *next = current->next();
                current->data()->~T();
                mAllocator.free(current);
                current = next;
            }
            mHead = nullptr;
            mTail = nullptr;
            mSize = 0;
        }

        /** @copydoc TList::clearNoDestructorCall() */
        void clearNoDestructorCall() override {
            Node *current = mHead;
            while (current != nullptr) {
                Node *next = current->next();
                mAllocator.free(current);
                current = next;
            }
            mHead = nullptr;
            mTail = nullptr;
            mSize = 0;
        }

        /** @copydoc TList::sort() */
        void sort(const typename TPredicate::Compare<T>::type &predicate) override {
            if (mSize > 1) {
                sort(mSize, mHead, predicate, mHead, mTail);
            }
        }

        /** @copydoc TList::getSize() */
        uint32 getSize() const override {
            return mSize;
        }

        /** @return Size of internal list node */
        static uint32 getNodeSize() {
            return sizeof(Node);
        }

        /** @copydoc TList::getMemoryUsage() */
        uint32 getMemoryUsage() const override {
            return mSize * sizeof(Node);
        }

        /** @return Allocator for this container */
        IAllocator &getAllocator() const {
            return mAllocator;
        }

    private:

        /** List template node to STORE data */
        struct Node {
        public:

            /**
             * Creates node with uninitialized data T content
             * to be initialized later (used for addUninitialized)
             */
            Node() = default;

            /** @return True if next nod exist */
            bool hasNext() const {
                return mNext != nullptr;
            }

            /** @return True if previous nod exist */
            bool hasPrev() const {
                return mPrev != nullptr;
            }

            /** Unlink the nodes chain after that node */
            void unlinkAfter() {
                if (mNext != nullptr) mNext->mPrev = nullptr;
                mNext = nullptr;
            }

            /** Unlink the nodes chain before that node */
            void unlinkBefore() {
                if (mPrev != nullptr) mPrev->mNext = nullptr;
                mPrev = nullptr;
            }

            /** Adds node after this one */
            void linkAfter(Node *node) {
                mNext = node;
                node->mPrev = this;
            }

            /** Adds node before this one */
            void linkBefore(Node *node) {
                mPrev = node;
                node->mNext = this;
            }

            /** Removes this node from nodes list */
            void remove() {
                if (mPrev != nullptr) mPrev->mNext = mNext;
                if (mNext != nullptr) mNext->mPrev = mPrev;
            }

            /** @return Pointer to next node */
            Node *next() const {
                return mNext;
            }

            /** @return Pointer to previous node */
            Node *prev() const {
                return mPrev;
            }

            /** @return Pointer to stored data */
            T *data() {
                return (T *) &mData[0];
            }

            /**
             * Swaps a and b nodes in the nodes list without breaking the
             * other order and correctness of the pointers
             *
             * Before: ... -> a -> b -> ...
             * After:  ... -> b -> a -> ...
             *
             * @param a Not null node
             * @param b Not null node
             */
            static void swap(Node *a, Node *b) {
                Node *afterB = b->mNext;
                Node *beforeA = a->mPrev;

                if (beforeA != nullptr) beforeA->mNext = b;
                b->mNext = a;
                a->mNext = afterB;

                b->mPrev = beforeA;
                a->mPrev = b;
                if (afterB != nullptr) afterB->mPrev = a;
            }

        private:

            /** New node in the list or null */
            Node *mNext = nullptr;

            /** New node in the list or null */
            Node *mPrev = nullptr;

            /** Stored data (memory) */
            char mData[sizeof(T)];

        };

        /**
         * Sorts the list of specified size from the specified start and returns
         * new head and new tail pointers for the sorted list
         * @param size Number of nodes to sort
         * @param start Pointer to the first node
         * @param predicate Function to check the order
         * @param head Pointer to new head in the sorted list
         * @param tail Pointer to new tail in the sorted list
         */
        void sort(uint32 size, Node *start,
                const typename TPredicate::Compare<T>::type &predicate,
                Node *&head, Node *&tail) {
            if (size <= STOP_RECURSIVE_SORT) {
                // Bubble sort here for small amount of data
                if (size == 1) {
                    head = start;
                    tail = start;

                    return;
                }

                uint32 i = 1;

                Node *end = nullptr;
                Node *last = nullptr;

                while (i < size) {
                    Node *a = start;
                    Node *b = start->next();

                    while (b != last) {
                        if (predicate(*b->data(), *a->data())) {
                            Node::swap(a, b);
                            if (a == start) { start = b; }
                            b = a->next();
                        } else {
                            a = b;
                            b = b->next();
                        }
                    }

                    if (last == nullptr) { end = a; }

                    last = a;
                    i += 1;
                }

                head = start;
                tail = end;
            } else {
                // Merge sort (split list and the merge)
                Node *left = start;
                Node *right = start;

                uint32 i = 0;
                uint32 center = size / 2;
                while (i < center) {
                    right = right->next();
                    i += 1;
                }
                right->unlinkBefore();

                Node *lstart = nullptr;
                Node *lend = nullptr;
                Node *rstart = nullptr;
                Node *rend = nullptr;

                sort(center, left, predicate, lstart, lend);
                sort(size - center, right, predicate, rstart, rend);

                left = lstart;
                right = rstart;

                if (predicate(*left->data(), *right->data())) {
                    head = left;
                    left = left->next();
                } else {
                    head = right;
                    right = right->next();
                }

                Node *current = head;

                while (left != nullptr && right != nullptr) {
                    if (predicate(*left->data(), *right->data())) {
                        current->linkAfter(left);
                        current = left;
                        left = left->next();
                    } else {
                        current->linkAfter(right);
                        current = right;
                        right = right->next();
                    }
                }
                while (left != nullptr) {
                    current->linkAfter(left);
                    current = left;
                    left = left->next();
                }
                while (right != nullptr) {
                    current->linkAfter(right);
                    current = right;
                    right = right->next();
                }

                tail = current;
            }
        }

        /** Assert fail on index out of range */
        void rangeCheck(uint32 index) const {
            DEV_ERROR_CONDITION(index < mSize, "Index out of bounds [size: %u][index: %u]", mSize, index);
        }

    public:

        /** For-each loop iterator */
        class Iterator {
        public:

            explicit Iterator(Node* node) : node(node) {
            }

            bool operator!=(const Iterator& other) {
                return node != other.node;
            }

            T& operator*() {
                return *node->data();
            }

            void operator++() {
                node = node->next();
            }

        private:
            Node* node;
        };

        /** loop functionality */
        Iterator begin() const {
            return Iterator(mHead);
        }

        /** loop functionality */
        Iterator end() const {
            return Iterator(nullptr);
        }

        /** @copydoc TIterable::foreach() */
        void forEach(const typename TPredicate::Consume<T>::type &function) override {
            for (const T& element : *this) {
                function(element);
            }
        }

    private:

        /** Used to allocate memory for nodes */
        IAllocator &mAllocator;

        /** Number of nodes in the list */
        uint32 mSize = 0;

        /** List head - always does not have prev */
        Node *mHead = nullptr;

        /** List tail - always does not have next */
        Node *mTail = nullptr;

    };


    /**
     * Dynamically expandable linked list for elements of type T.
     * Uses internal pool for allocating nodes.
     *
     * Elements should be of re-allocatable type and support safe memory move without copy-constructor.
     * Provides iteration mechanism for elements for using in for loop.
     *
     * @tparam T Template type for elements of the list
     */
    template <typename T>
    class TLinkedList {

    };

} // namespace Berserk

#endif //BERSERK_TLINKEDLIST_H