/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TSET_H
#define BERSERK_TSET_H

#include <Containers/TIterable.h>
#include <AllocPool.h>
#include <Math/Math.h>
#include <IO/Archive.h>
#include <ErrorMacro.h>
#include <TPredicates.h>

namespace Berserk {

    template <typename T, typename H = THash<T>, typename E = TEquals<T>>
    class TSet final : public TIterable<T> {
    private:

        struct Node {
            uint8 data[sizeof(T)];
            Node* next;

            void destroy() {
                getData()->~T();
            }

            T* getData() { return (T*)data; }
        };

        struct List {
            Node* first;
        };

    public:

        static const uint32 INITIAL_RANGE = 8;
        static const uint32 FACTOR = 4;
        static const uint32 EXPAND_LIST_LEN = 4;

        explicit TSet(IAlloc& alloc = IAlloc::getSingleton())
            :  mNodeAlloc(sizeOfNode(),alloc),
               mAlloc(&alloc) {

        }
        TSet(const std::initializer_list<T> &list) : TSet<T,H,E>() {
            add(list);
        }
        TSet(const TSet& other)
                :  mNodeAlloc(sizeOfNode(), *other.mAlloc),
                   mAlloc(other.mAlloc) {
            for (const auto& e: other) {
                add(e);
            }
        }
        TSet(TSet&& other) noexcept
            :  mNodeAlloc(std::move(other.mNodeAlloc)),
               mAlloc(other.mAlloc) {
            mLists = other.mLists;
            mRange = other.mRange;
            mSize = other.mSize;
            mMaxListLen = other.mMaxListLen;

            other.mAlloc = nullptr;
            other.mLists = nullptr;
            other.mRange = 0;
            other.mSize = 0;
            other.mMaxListLen = 0;
        }
        ~TSet() override {
            if (mLists) {
                clear();
                mAlloc->free(mLists);
                mLists = nullptr;
                mRange = 0;
            }
        }

        void add(const T& element) {
            expand();

            E equals;
            uint32 nodes = 0;
            uint32 i = index(element);
            List& list = mLists[i];
            Node* current = list.first;
            Node* found = nullptr;
            while (current != nullptr) {
                nodes += 1;
                if (equals(*current->getData(), element)) {
                    found = current;
                    break;
                }

                current = current->next;
            }

            if (found) {
                found->destroy();
                new(found->getData()) T(element);
            }
            else {
                mMaxListLen = Math::max(nodes + 1, mMaxListLen);

                Node* toAdd = (Node*) mNodeAlloc.allocate(sizeof(Node));
                new (toAdd->getData()) T(element);
                toAdd->next = list.first;
                list.first = toAdd;
                mSize += 1;
            }
        }
        void move(T& element) {
            expand();

            E equals;
            uint32 nodes = 0;
            uint32 i = index(element);
            List& list = mLists[i];
            Node* current = list.first;
            Node* found = nullptr;
            while (current != nullptr) {
                nodes += 1;
                if (equals(*current->getData(), element)) {
                    found = current;
                    break;
                }

                current = current->next;
            }

            if (found) {
                found->destroy();
                new(found->getData()) T(std::move(element));
            }
            else {
                mMaxListLen = Math::max(nodes + 1, mMaxListLen);

                Node* toAdd = (Node*) mNodeAlloc.allocate(sizeof(Node));
                new (toAdd->getData()) T(std::move(element));
                toAdd->next = list.first;
                list.first = toAdd;
                mSize += 1;
            }
        }
        void add(const std::initializer_list<T> &list) {
            for (const auto& e: list) {
                add(e);
            }
        }

        bool contains(const T &element) const {
            if (mRange == 0)
                return false;

            auto i = index(element);
            E equals;
            Node* current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->getData(), element)) {
                    return true;
                }

                current = current->next;
            }

            return false;
        }

        bool remove(const T &element) {
            if (mRange == 0)
                return false;

            auto i = index(element);
            E equals;
            List& list = mLists[i];
            Node* current = list.first;

            if (current != nullptr) {
                if (equals(*current->getData(), element)) {
                    list.first = current->next;
                    current->destroy();
                    mNodeAlloc.free(current);
                    return true;
                }

                Node* prev = current;
                current = current->next;

                while (current != nullptr) {
                    if (equals(*current->getData(), element)) {
                        prev->next = current->next;
                        current->destroy();
                        mNodeAlloc.free(current);
                        return true;
                    }

                    prev = current;
                    current = current->next;
                }
            }

            return false;
        }

        void clear() {
            for (uint32 i = 0; i < mRange; i++) {
                Node* current = mLists[i].first;
                mLists[i].first = nullptr;
                while (current != nullptr) {
                    auto next = current->next;
                    current->destroy();
                    mNodeAlloc.free(current);
                    current = next;
                }
            }
            mSize = 0;
        }
        void clearNoDestuctorCall() {
            for (uint32 i = 0; i < mRange; i++) {
                Node* current = mLists[i].first;
                mLists[i].first = nullptr;
                while (current != nullptr) {
                    auto next = current->next;
                    mNodeAlloc.free(current);
                    current = next;
                }
            }
            mSize = 0;
        }

        uint32 size() const {
            return mSize;
        }

        uint32 range() const {
            return mRange;
        }

        /** @return Dynamically allocated memory count (in bytes) */
        uint64 getAllocatedMemory() const {
            return mNodeAlloc.getAllocatedMemory() + sizeof(List) * mRange;
        }

        static constexpr uint32 sizeOfNode() {
            return sizeof(Node);
        }

        template <typename I>
        class Iterator {
        private:
            friend class TSet;

            Node* current;
            List* lists;
            uint32 listIndex;
            uint32 range;

            Iterator(Node* c, List* l, uint32 i, uint32 r)
                    : current(c), lists(l), listIndex(i), range(r) {

            }
        public:
            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }
            void operator++() {
                if (current)
                    current = current->next;

                if (current == nullptr) {
                    if (lists != nullptr) {
                        for (uint32 i = listIndex + 1; i < range; i++) {
                            if (lists[i].first != nullptr) {
                                listIndex = i;
                                current = lists[i].first;
                                return;
                            }
                        }

                        lists = nullptr;
                    }
                }
            }
            I &operator*() {
                return *current->getData();
            }
        };

        Iterator<const T> begin() const {
            Node* f; uint32 i;
            firstNodeNotNull(f, i);
            return {f, mLists, i, mRange};
        }

        Iterator<const T> end() const {
            return {nullptr, nullptr,0,0};
        }

        Iterator<T> begin() {
            Node* f; uint32 i;
            firstNodeNotNull(f, i);
            return {f, mLists, i, mRange};
        }

        Iterator<T> end() {
            return {nullptr, nullptr,0,0};
        }

        void forEach(const Function<void(T&)> &function) override {
            for (auto& p: *this) {
                function(p);
            }
        }

        void forEach(const Function<void(const T&)> &function) const override {
            for (const auto& p: *this) {
                function(p);
            }
        }


        friend Archive& operator<<(Archive& archive, const TSet<T,H,E> &set) {
            auto elementsCount = set.size();
            archive << elementsCount;

            for (const auto& e: set) {
                archive << e;
            }

            return archive;
        }

        friend Archive& operator>>(Archive& archive, TSet<T,H,E> &set) {
            uint32 elementsCount = 0;
            archive >> elementsCount;

            for (uint32 i = 0; i < elementsCount; i++) {
                T element;
                archive >> element;
                set.move(element);
            }

            return archive;
        }

    private:

        void expand() {
            // Allocate initial lists array if needed.
            // It prevents allocating dynamic memory for empty maps
            if (mLists == nullptr) {
                mRange = INITIAL_RANGE;
                mLists = (List*) mAlloc->allocate(mRange * sizeof(List));

                for (uint32 i = 0; i < mRange; i++) {
                    mLists[i].first = nullptr;
                }
            }
            else if (mMaxListLen > EXPAND_LIST_LEN) {
                mMaxListLen = 0;

                uint32 newRange = mRange * FACTOR;
                List* newLists = (List*) mAlloc->allocate(newRange * sizeof(List));

                for (uint32 i = 0; i < newRange; i++) {
                    newLists[i].first = nullptr;
                }

                auto newIndex = [&](const T& key){
                    H hash;
                    return hash(key) % newRange;
                };

                // Attach old nodes to new lists
                for (uint32 i = 0; i < mRange; i++) {
                    Node* current = mLists[i].first;

                    while (current != nullptr) {
                        auto next = current->next;
                        auto h = newIndex(*current->getData());
                        auto& list = newLists[h];

                        current->next = list.first;
                        list.first = current;

                        current = next;
                    }
                }

                mAlloc->free(mLists);
                mRange = newRange;
                mLists = newLists;
            }
        }

        uint32 index(const T& key) const {
            H hash;
            return hash(key) % mRange;
        }

        void firstNodeNotNull(Node* &node, uint32 &listIndex) const {
            for (uint32 i = 0; i < mRange; i++) {
                if (mLists[i].first != nullptr) {
                    node = mLists[i].first;
                    listIndex = i;
                    return;
                }
            }

            node = nullptr;
            listIndex = mRange;
        }

    private:
        AllocPool mNodeAlloc;
        IAlloc* mAlloc;
        List* mLists = nullptr;
        uint32 mRange = 0;
        uint32 mSize = 0;
        uint32 mMaxListLen = 0;
    };

}




#endif //BERSERK_TSET_H
