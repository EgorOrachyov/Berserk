/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
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

#ifndef BERSERK_TMAP_HPP
#define BERSERK_TMAP_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Memory/TPoolAllocator.hpp>
#include <BerserkCore/TPredicates.hpp>
#include <BerserkCore/TPair.hpp>

namespace Berserk {

    /**
     * @brief Hash map container
     *
     * Generic hash table container, based on hashing of the keys and storing
     * values in the buckets, where single bucket stores values with different keys but the same hashes.
     *
     * This hash table implementation uses internally pool allocator, to allocate
     * buckets nodes to store key-value pairs, therefore the references to stored
     * values could be safely passed around. This references stay valid until the element
     * is removed from the hash map.
     *
     * There are some default implementations for hashing and compare predicates,
     * which could be found in source files. This predicates serves objects and primitive types.
     *
     * @tparam K Generic type of the key
     * @tparam V Generic type of the value
     * @tparam H Hashing predicate used to hash keys
     * @tparam E Equals predicate used to compare keys
     * @tparam A Allocator used for dynamic allocations
     */
    template<typename K, typename V, typename H = THash<K>, typename E = TEquals<K>, typename A = GlobalAllocator>
    class TMap final {
    private:

        using Pair = TPair<K, V>;

        class Node {
        public:
            Node(const K& key, const V& value) : mData(key, value) {}
            Node(K& key, V& value) : mData(std::move(key), std::move(value)) {}
            ~Node() = default;

            K &GetKey() { return GetPair().GetFirst(); }
            V &GetValue() { return GetPair().GetSecond(); }
            V &PlaceValue(const V& value) { GetValue().~V(); return *(new(&GetValue()) V(value)); }
            V &ReplaceValue(V& value) { GetValue().~V(); return *(new(&GetValue()) V(std::move(value))); }
            Pair &GetPair() { return mData; }
            Node *&GetNext() { return mNext; }

        private:
            Pair mData;
            Node *mNext = nullptr;
        };

        struct List {
            Node *first = nullptr;
        };

    public:

        static const uint32 INITIAL_RANGE = 8;
        static const uint32 FACTOR = 2;
        static const uint32 LOAD_FACTOR_PERCENT = 75;

        // If in the table elementsCount / range > LOAD_FACTOR_PERCENT / 100
        // then will expand by the FACTOR.
        // Initially table always have INITIAL_RANGE as range

        explicit TMap(A alloc = A()) noexcept
            : mNodeAlloc(GetSizeOfNode(), std::move(alloc)) {

        }

        TMap(const std::initializer_list<TPair<K, V>> &list) : TMap<K,V,H,E,A>() {
            Add(list);
        }

        TMap(const TMap &other)
                : mNodeAlloc(GetSizeOfNode(), other.GetAllocator()) {
            for (const auto &p: other) {
                Add(p.first(), p.second());
            }
        }

        TMap(TMap &&other) noexcept
                : mNodeAlloc(std::move(other.mNodeAlloc)) {
            mLists = other.mLists;
            mRange = other.mRange;
            mSize = other.mSize;

            other.mLists = nullptr;
            other.mRange = 0;
            other.mSize = 0;
        }

        ~TMap() {
            if (mLists) {
                Clear();
                GetAllocator().Free(mLists);
                mLists = nullptr;
                mRange = 0;
            }
        }

        TMap &operator=(const TMap &other) {
            if (this == &other) {
                return *this;
            }

            this->~TMap();
            new(this) TMap(other);
            return *this;
        }

        TMap &operator=(TMap &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~TMap();
            new(this) TMap(std::move(other));
            return *this;
        }

        void Add(const K &key, const V &value) {
            Expand();

            E equals;
            List &list = mLists[GetIndex(key)];
            Node *current = list.first;
            Node *found = nullptr;
            while (current != nullptr) {
                if (equals(current->GetKey(), key)) {
                    found = current;
                    break;
                }
                current = current->GetNext();
            }

            if (found) {
                found->PlaceValue(value);
            } else {
                Node *toAdd = new(mNodeAlloc.Allocate(sizeof(Node))) Node(key, value);
                toAdd->GetNext() = list.first;
                list.first = toAdd;
                mSize += 1;
            }
        }

        bool AddIfNotPresent(const K &key, const V &value) {
            Expand();

            E equals;
            List &list = mLists[GetIndex(key)];
            Node *current = list.first;
            while (current != nullptr) {
                if (equals(current->GetKey(), key)) {
                    return false;
                }

                current = current->GetNext();
            }

            Node *toAdd = new(mNodeAlloc.Allocate(sizeof(Node))) Node(key, value);
            toAdd->GetNext() = list.first;
            list.first = toAdd;
            mSize += 1;

            return true;
        }

        template<typename ... TArgs>
        V &Emplace(const K &key, TArgs &&... args) {
            Expand();

            E equals;
            List &list = mLists[GetIndex(key)];
            Node *current = list.first;
            Node *found = nullptr;
            while (current != nullptr) {
                if (equals(current->GetKey(), key)) {
                    found = current;
                    break;
                }

                current = current->GetNext();
            }

            V value(std::forward<TArgs>(args)...);

            if (found) {
                return found->ReplaceValue(value);
            } else {
                Node *toAdd = new(mNodeAlloc.Allocate(sizeof(Node))) Node(key, value);
                toAdd->GetNext() = list.first;
                list.first = toAdd;
                mSize += 1;

                return toAdd->GetValue();
            }
        }

        void Move(K &key, V &value) {
            Expand();

            auto i = index(key);
            E equals;
            uint32 nodes = 0;
            List &list = mLists[i];
            Node *current = list.first;
            Node *found = nullptr;
            while (current != nullptr) {
                nodes += 1;
                if (equals(*current->GetKey(), key)) {
                    found = current;
                    break;
                }

                current = current->GetNext();
            }

            if (found) {
                found->ReplaceValue(value);
            } else {
                Node *toAdd = new(mNodeAlloc.Allocate(sizeof(Node))) Node((K&) key, (V&) value);
                toAdd->GetNext() = list.first;
                list.first = toAdd;
                mSize += 1;
            }
        }

        void Add(const std::initializer_list<TPair<K, V>> &list) {
            for (auto &p: list) {
                Add(p.GetFirst(), p.GetSecond());
            }
        }

        bool Contains(const K &key) const {
            if (mRange == 0)
                return false;

            auto i = index(key);
            E equals;
            Node *current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->GetKey(), key)) {
                    return true;
                }

                current = current->GetNext();
            }

            return false;
        }

        bool Remove(const K &key) {
            if (mRange == 0)
                return false;

            E equals;
            List &list = mLists[GetIndex(key)];
            Node *current = list.first;

            if (current != nullptr) {
                if (equals(current->GetKey(), key)) {
                    list.first = current->GetNext();
                    current->~Node();
                    mNodeAlloc.Free(current);
                    return true;
                }

                Node *prev = current;
                current = current->GetNext();

                while (current != nullptr) {
                    if (equals(current->GetKey(), key)) {
                        prev->GetNext() = current->GetNext();
                        current->~Node();
                        mNodeAlloc.Free(current);
                        return true;
                    }

                    prev = current;
                    current = current->GetNext();
                }
            }

            return false;
        }

        void Clear() {
            for (uint32 i = 0; i < mRange; i++) {
                Node *current = mLists[i].first;
                mLists[i].first = nullptr;
                while (current != nullptr) {
                    auto next = current->GetNext();
                    current->~Node();
                    mNodeAlloc.Free(current);
                    current = next;
                }
            }
            mSize = 0;
        }

        V* GetPtr(const K &key) {
            if (mRange == 0)
                return nullptr;

            auto i = index(key);
            E equals;
            Node *current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->GetKey(), key))
                    return current->getValue();

                current = current->GetNext();
            }

            return nullptr;
        }

        const V* GetPtr(const K &key) const {
            if (mRange == 0)
                return {};

            auto i = index(key);
            E equals;
            Node *current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->GetKey(), key))
                    return current->getValue();

                current = current->GetNext();
            }

            return {};
        }

        V& operator[](const K &key) {
            if (mSize > 0) {
                auto i = index(key);
                E equals;
                Node *current = mLists[i].first;
                while (current != nullptr) {
                    if (equals(*current->GetKey(), key))
                        return *current->getValue();

                    current = current->GetNext();
                }
            }

            return emplace(key);
        }

        const V& operator[](const K &key) const {
            if (mSize > 0) {
                auto i = index(key);
                E equals;
                Node *current = mLists[i].first;
                while (current != nullptr) {
                    if (equals(*current->GetKey(), key))
                        return *current->getValue();

                    current = current->GetNext();
                }
            }

            return emplace(key);
        }

        bool operator==(const TMap &other) const {
            if (mSize != other.mSize)
                return false;

            for (const auto &pair: *this) {
                const V *value = other.getPtr(pair.first()).GetPtr();

                if (value == nullptr)
                    return false;

                if (!(pair.GetValue() != *value))
                    return false;
            }

            return false;
        }

        bool operator!=(const TMap &other) const {
            return !(*this == other);
        }

        void GetKeys(TArray<K> &keys) const {
            keys.EnsureToAdd(GetSize());
            for (const auto &pair: *this) {
                keys.Emplace(pair.first());
            }
        }

        void GetKeyValues(TArray<TPair<K, V>> &keyValues) const {
            keyValues.EnsureToAdd(GetSize());
            for (const auto &pair: *this) {
                keyValues.Emplace(pair);
            }
        }

        uint32 GetSize() const {
            return mSize;
        }

        uint32 GetRange() const {
            return mRange;
        }

        float GetLoadFactor() const {
            return mRange > 0 ? (float) mSize / (float) mRange: 0.0f;
        }

        /** @return Dynamically allocated memory count (in bytes) */
        uint64 GetAllocatedMemory() const {
            return mNodeAlloc.getAllocatedMemory() + sizeof(List) * mRange;
        }

        static constexpr uint32 GetSizeOfNode() {
            return sizeof(Node);
        }

        template<typename Tit>
        class TIterator {
        public:

            TIterator(List* list, Node* current, uint32 listIndex, uint32 range) {
                mLists = list;
                mCurrent = current;
                mListIndex = listIndex;
                mRange = range;
            }

            bool operator==(const TIterator &other) const {
                return mCurrent == other.mCurrent;
            }

            bool operator!=(const TIterator &other) const {
                return mCurrent != other.mCurrent;
            }

            void operator++() {
                if (mCurrent)
                    mCurrent = mCurrent->GetNext();

                if (mCurrent == nullptr) {
                    if (mLists != nullptr) {
                        for (uint32 i = mListIndex + 1; i < mRange; i++) {
                            if (mLists[i].first != nullptr) {
                                mListIndex = i;
                                mCurrent = mLists[i].first;
                                return;
                            }
                        }

                        mLists = nullptr;
                        mListIndex = 0;
                        mRange = 0;
                    }
                }
            }

            Tit &operator*() {
                return mCurrent->GetPair();
            }

        protected:
            List* mLists;
            Node* mCurrent;
            uint32 mListIndex;
            uint32 mRange;
        };

        class Iterator: public TIterator<Pair> {
        public:
            using TIterator<Pair>::mLists;
            using TIterator<Pair>::mCurrent;
            using TIterator<Pair>::mListIndex;
            using TIterator<Pair>::mRange;
            using Map = TMap<K,V,H,E,A>;

            Iterator(Map* map, List* list, Node* current, uint32 listIndex, uint32 range) : TIterator<Pair>(list, current, listIndex, range) {
                mMap = map;
            }

            bool Remove() {
                if (mCurrent) {
                    K& key = mCurrent->GetKey();
                    Node* next = mCurrent->GetNext();

                    mMap->Remove(key);

                    mCurrent == next;
                    this->operator++();

                    return true;
                }

                return false;
            }

        private:
            Map* mMap;
        };

        class ConstIterator: public TIterator<const Pair> {
        public:

            ConstIterator(List* list, Node* current, uint32 listIndex, uint32 range) : TIterator<const Pair>(list, current, listIndex, range) {

            }
        };

        Iterator begin() {
            Node* node;
            uint32 listIndex;
            GetFirstNodeNotNull(node, listIndex);
            return Iterator(this, mLists, node, listIndex, mRange);
        }

        Iterator end() {
            return Iterator(nullptr, nullptr, nullptr, 0, 0);
        }

        ConstIterator begin() const {
            Node* node;
            uint32 listIndex;
            GetFirstNodeNotNull(node, listIndex);
            return ConstIterator(mLists, node, listIndex, mRange);
        }

        ConstIterator end() const {
            return Iterator(nullptr, nullptr, 0, 0);
        }

    private:

        void Expand() {
            // Allocate initial lists array if needed.
            // It prevents allocating dynamic memory for empty maps
            if (mLists == nullptr) {
                mRange = INITIAL_RANGE;
                mLists = (List *) GetAllocator().Allocate(mRange * sizeof(List));

                for (uint32 i = 0; i < mRange; i++) {
                    mLists[i].first = nullptr;
                }
            // Otherwise expand if needed
            // All elements are relinked to the new range lists (so exposed values pointers will remain valid)
            } else if (GetLoadFactor() > (float) LOAD_FACTOR_PERCENT / 100.0f ) {
                uint32 newRange = mRange * FACTOR;
                List *newLists = (List *) GetAllocator().Allocate(newRange * sizeof(List));

                for (uint32 i = 0; i < newRange; i++) {
                    newLists[i].first = nullptr;
                }

                auto newIndex = [&](const K &key) {
                    H hash;
                    return hash(key) % newRange;
                };

                // Attach old nodes to new lists
                for (uint32 i = 0; i < mRange; i++) {
                    Node *current = mLists[i].first;

                    while (current != nullptr) {
                        auto next = current->GetNext();
                        auto h = newIndex(current->GetKey());
                        auto &list = newLists[h];

                        current->GetNext() = list.first;
                        list.first = current;

                        current = next;
                    }
                }

                GetAllocator().Free(mLists);
                mRange = newRange;
                mLists = newLists;
            }
        }

        uint32 GetIndex(const K &key) const {
            H hash;
            return hash(key) % mRange;
        }

        void GetFirstNodeNotNull(Node *&node, uint32 &listIndex) const {
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

        A& GetAllocator() {
            return mNodeAlloc.GetAllocator();
        }

    private:
        TPoolAllocator<A> mNodeAlloc;
        List *mLists = nullptr;
        uint32 mRange = 0;
        uint32 mSize = 0;
    };

}

#endif //BERSERK_TMAP_HPP