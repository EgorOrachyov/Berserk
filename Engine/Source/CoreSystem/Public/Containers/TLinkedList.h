//
// Created by Egor Orachyov on 03.06.2019.
//

#ifndef BERSERK_TLINKEDLIST_H
#define BERSERK_TLINKEDLIST_H

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
    class TLinkedList : public TList<T>
    {
    public:

        GENERATE_NEW_DELETE(TLinkedList);

        ~TLinkedList() override = default
        {

        }

        void add(const T &element) override
        {

        }

        T *addUninitialized() override
        {
            return nullptr;
        }

        uint32 addUninitialized(uint32 count) override
        {
            return 0;
        }

        void append(const TList<T> &container) override
        {

        }

        void append(const T *array, uint32 count) override
        {

        }

        T &get(uint32 index) const override
        {
            return <#initializer#>;
        }

        void remove(uint32 index) override
        {

        }

        void clear() override
        {

        }

        void sort(bool (*predicate)(const T &, const T &)) override
        {

        }

        uint32 getSize() const override
        {
            return mSize;
        }

        uint32 getMemoryUsage() const override {
            return nullptr;
        }

        T *begin() const override {
            return nullptr;
        }

        T *next() const override {
            return nullptr;
        }

    private:

        /** List template node to STORE data */
        struct Node
        {
        public:

            GENERATE_NEW_DELETE(Node);

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

            /** @return Pointer to next node */
            Node* next() const { return mNext; }

            /** @return Pointer to previous node */
            Node* prev() const { return mPrev; }

            /** @return Pointer to stored data */
            T* data() { return &mData[0]; }

        private:

            /** New node in the list or null */
            T* mNext = nullptr;

            /** New node in the list or null */
            T* mPrev = nullptr;

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
        class Node* mIterator = nullptr;


    };

} // namespace Berserk

#endif //BERSERK_TLINKEDLIST_H