/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_THASHMAP_H
#define BERSERK_THASHMAP_H

#include <TMap.h>
#include <TPredicates.h>
#include <ErrorMacro.h>
#include <AllocPool.h>
#include <Math/Math.h>
#include <initializer_list>

namespace Berserk {

    template <typename K, typename V, typename E = TEquals<K>, typename H = THash<K>>
    class THashMap : public TMap<K,V> {
    private:

        struct Node {
            uint8 data[sizeof(TPair<K,V>)];
            Node* next;

            void destroy() {
                getKey()->~K();
                getValue()->~V();
            }

            K* getKey() { return &getPair()->first(); }
            V* getValue() { return &getPair()->second(); }
            TPair<K,V>* getPair() { return (TPair<K,V>*)data; }
        };

        struct List {
            Node* first;
        };

    public:

        static const uint32 INITIAL_RANGE = 8;
        static const uint32 FACTOR = 4;
        static const uint32 EXPAND_LIST_LEN = 4;

        explicit THashMap(IAlloc& alloc = IAlloc::getSingleton())
            :  mNodeAlloc(sizeOfNode(),alloc),
               mAlloc(&alloc) {

        }
        THashMap(const std::initializer_list<TPair<K,V>> &list) : THashMap<K,V,E,H>() {
            add(list);
        }
        THashMap(const THashMap& other)
            :  mNodeAlloc(sizeOfNode(), other.mAlloc),
               mAlloc(other.mAlloc) {

        }
        THashMap(THashMap&& other) noexcept
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
        ~THashMap() override {
            if (mLists) {
                clear();
                mAlloc->free(mLists);
                mLists = nullptr;
                mRange = 0;
            }
        }

        void add(const K &key, const V &value) override {
            expand();

            auto i = index(key);
            E equals;
            uint32 nodes = 0;
            List& list =  mLists[i];
            Node* current = list.first;
            Node* found = nullptr;
            while (current != nullptr) {
                nodes += 1;
                if (equals(*current->getKey(), key)) {
                    found = current;
                    break;
                }

                current = current->next;
            }

            if (found) {
                found->getValue()->~V();
                new(found->getValue()) V(value);
            }
            else {
                mMaxListLen = Math::max(nodes + 1, mMaxListLen);

                Node* toAdd = (Node*) mNodeAlloc.allocate(sizeof(Node));
                new (toAdd->getKey()) K(key);
                new (toAdd->getValue()) V(value);
                toAdd->next = list.first;
                list.first = toAdd;
                mSize += 1;
            }
        }
        bool addIfNotPresent(const K &key, const V &value) override {
            expand();

            auto i = index(key);
            E equals;
            uint32 nodes = 0;
            List& list =  mLists[i];
            Node* current = list.first;
            while (current != nullptr) {
                nodes += 1;
                if (equals(*current->getKey(), key)) {
                    return false;
                }

                current = current->next;
            }

            mMaxListLen = Math::max(nodes + 1, mMaxListLen);

            Node* toAdd = (Node*) mNodeAlloc.allocate(sizeof(Node));
            new (toAdd->getKey()) K(key);
            new (toAdd->getValue()) V(value);
            toAdd->next = list.first;
            list.first = toAdd;
            mSize += 1;

            return true;
        }
        template <typename ... TArgs>
        V& emplace(const K& key, TArgs&& ... args) {
            expand();

            auto i = index(key);
            E equals;
            uint32 nodes = 0;
            List& list =  mLists[i];
            Node* current = list.first;
            Node* found = nullptr;
            while (current != nullptr) {
                nodes += 1;
                if (equals(*current->getKey(), key)) {
                    found = current;
                    break;
                }

                current = current->next;
            }

            if (found) {
                found->getValue()->~V();
                new (found->getValue()) V(std::forward<TArgs>(args)...);
                return *found->getValue();
            }
            else {
                mMaxListLen = Math::max(nodes + 1, mMaxListLen);

                Node* toAdd = (Node*) mNodeAlloc.allocate(sizeof(Node));
                new (toAdd->getKey()) K(key);
                new (toAdd->getValue()) V(std::forward<TArgs>(args)...);
                toAdd->next = list.first;
                list.first = toAdd;
                mSize += 1;
                return *toAdd->getValue();
            }
        }
        void add(const std::initializer_list<TPair<K,V>> &list) {
            for (auto& p: list) {
                add(p.first(), p.second());
            }
        }

        bool contains(const K &key) const override {
           if (mRange == 0)
               return false;

            auto i = index(key);
            E equals;
            Node* current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->getKey(), key)) {
                    return true;
                }

                current = current->next;
            }

            return false;
        }

        bool remove(const K &key) override {
            if (mRange == 0)
                return false;

            auto i = index(key);
            E equals;
            List& list = mLists[i];
            Node* current = list.first;

            if (current != nullptr) {
                if (equals(*current->getKey(), key)) {
                    list.first = current->next;
                    current->destroy();
                    mNodeAlloc.free(current);
                    return true;
                }

                Node* prev = current;
                current = current->next;

                while (current != nullptr) {
                    if (equals(*current->getKey(), key)) {
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

        void clear() override {
            for (uint32 i = 0; i < mRange; i++) {
                Node* current = mLists[i].first;
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
                while (current != nullptr) {
                    auto next = current->next;
                    mNodeAlloc.free(current);
                    current = next;
                }
            }
            mSize = 0;
        }

        V *getPtr(const K &key) override {
            if (mRange == 0)
                return nullptr;

            auto i = index(key);
            E equals;
            Node* current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->getKey(), key))
                    return current->getValue();

                current = current->next;
            }

            return nullptr;
        }

        const V *getPtr(const K &key) const override {
            if (mRange == 0)
                return nullptr;

            auto i = index(key);
            E equals;
            Node* current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->getKey(), key))
                    return current->getValue();

                current = current->next;
            }

            return nullptr;
        }

        V &operator[](const K &key) override {
            BERSERK_COND_ERROR_FAIL(mRange > 0, "Hash map is empty")

            auto i = index(key);
            E equals;
            Node* current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->getKey(), key))
                    return *current->getValue();

                current = current->next;
            }

            BERSERK_ERROR_FAIL("Hash map has no element with such key")
        }

        const V &operator[](const K &key) const override {
            BERSERK_COND_ERROR_FAIL(mRange > 0, "Hash map is empty")

            auto i = index(key);
            E equals;
            Node* current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->getKey(), key))
                    return *current->getValue();

                current = current->next;
            }

            BERSERK_ERROR_FAIL("Hash map has no element with such key")
        }

        uint32 size() const override {
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

        template <typename T>
        class Iterator {
        private:
            friend class THashMap;

            Node* current;
            List* lists;
            uint32 listIndex;
            uint32 range;

            Iterator(Node* c, List* l, uint32 i, uint32 r)
                : current(c), lists(l), listIndex(i), range(r) {

            }
        public:
            bool operator!=(const Iterator& other) const {
                return lists != other.lists || current != other.current;
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
            T &operator*() {
                return *current->getPair();
            }
        };

        Iterator<const TPair<K,V>> begin() const {
            Node* f; uint32 i;
            firstNodeNotNull(f, i);
            return {f, mLists, i, mRange};
        }

        Iterator<const TPair<K,V>> end() const {
            return {nullptr, nullptr,0,0};
        }

        Iterator<TPair<K,V>> begin() {
            Node* f; uint32 i;
            firstNodeNotNull(f, i);
            return {f, mLists, i, mRange};
        }

        Iterator<TPair<K,V>> end() {
            return {nullptr, nullptr,0,0};
        }

        void forEach(const Function<void(TPair<K,V> &)> &function) override {
            for (auto& p: *this) {
                function(p);
            }
        }

        void forEach(const Function<void(const TPair<K,V> &)> &function) const override {
            for (const auto& p: *this) {
                function(p);
            }
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

                auto newIndex = [&](const K& key){
                    H hash;
                    return hash(key) % newRange;
                };

                // Attach old nodes to new lists
                for (uint32 i = 0; i < mRange; i++) {
                    Node* current = mLists[i].first;

                    while (current != nullptr) {
                        auto next = current->next;
                        auto h = newIndex(*current->getKey());
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

        uint32 index(const K& key) const {
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

#endif //BERSERK_THASHMAP_H
