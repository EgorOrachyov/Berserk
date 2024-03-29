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

#ifndef BERSERK_HASHTABLE_HPP
#define BERSERK_HASHTABLE_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/Pair.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Memory/PoolAllocator.hpp>

namespace Berserk {

    /**
     * @brief Hash table container
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
    template<typename K, typename V, typename H = Hash<K>, typename E = Equals<K>, typename A = Allocator>
    class HashTable final {
    private:

        using MapPair = Pair<K, V>;
        using Alloc = A;

        class Node {
        public:
            Node(MapPair &&pair) : mData(std::move(pair)) {}
            Node(const K& key, const V& value) : mData(key, value) {}
            Node(K& key, V& value) : mData(std::move(key), std::move(value)) {}
            ~Node() = default;

            K &GetKey() { return GetPair().GetFirst(); }
            V &GetValue() { return GetPair().GetSecond(); }
            V &PlaceValue(const V& value) { GetValue().~V(); return *(new(&GetValue()) V(value)); }
            V &ReplaceValue(V& value) { GetValue().~V(); return *(new(&GetValue()) V(std::move(value))); }

            MapPair &GetPair() { return mData; }
            Node *&GetNext() { return mNext; }

        private:
            MapPair mData;
            Node *mNext = nullptr;
        };

        struct List {
            Node *first = nullptr;
        };

    public:

        static const uint64 INITIAL_RANGE = 8;
        static const uint64 FACTOR = 2;
        static const uint64 LOAD_FACTOR_PERCENT = 75;

        // If in the table elementsCount / range > LOAD_FACTOR_PERCENT / 100
        // then will expand by the FACTOR.
        // Initially table always have INITIAL_RANGE as range

        explicit HashTable(A&& alloc = A()) noexcept
            : mNodeAlloc(GetSizeOfNode(), PoolAllocator<A>::INITIAL_CHUNKS_COUNT, std::move(alloc)) {

        }

        HashTable(const std::initializer_list<MapPair> &list) : HashTable<K,V,H,E,A>() {
            Add(list);
        }

        HashTable(const HashTable &other)
            : mNodeAlloc(GetSizeOfNode(), other.GetAllocator()) {
            for (const auto &p: other) {
                Add(p.GetFirst(), p.GetSecond());
            }
        }

        HashTable(HashTable &&other) noexcept
            : mNodeAlloc(std::move(other.mNodeAlloc)) {
            mLists = other.mLists;
            mRange = other.mRange;
            mSize = other.mSize;

            other.mLists = nullptr;
            other.mRange = 0;
            other.mSize = 0;
        }

        ~HashTable() {
            if (mLists) {
                Clear();
                GetAllocator().Deallocate(mLists);
                mLists = nullptr;
                mRange = 0;
            }
        }

        HashTable &operator=(const HashTable &other) {
            if (this == &other) {
                return *this;
            }

            HashTable tmp = other;
            *this = std::move(tmp);

            return *this;
        }

        HashTable &operator=(HashTable &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~HashTable();
            new(this) HashTable(std::move(other));
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
            V valueData(std::forward<TArgs>(args)...);
            K keyData = key;

            return Move(keyData, valueData);
        }

        V& Move(K &key, V &value) {
            Expand();

            auto i = GetIndex(key);
            E equals;
            uint64 nodes = 0;
            List &list = mLists[i];
            Node *current = list.first;
            Node *found = nullptr;
            while (current != nullptr) {
                nodes += 1;
                if (equals(current->GetKey(), key)) {
                    found = current;
                    break;
                }

                current = current->GetNext();
            }

            if (found) {
                found->ReplaceValue(value);
                return found->GetValue();
            } else {
                Node *toAdd = new(mNodeAlloc.Allocate(sizeof(Node))) Node((K&) key, (V&) value);
                toAdd->GetNext() = list.first;
                list.first = toAdd;
                mSize += 1;
                return toAdd->GetValue();
            }
        }

        void Add(const std::initializer_list<MapPair> &list) {
            for (auto &p: list) {
                Add(p.GetFirst(), p.GetSecond());
            }
        }

        void Add(const MapPair* pairs, uint64 count) {
            for (uint64 i = 0; i < count; i++) {
                Add(pairs[i].GetFirst(), pairs[i].GetSecond());
            }
        }

        bool Contains(const K &key) const {
            if (mRange == 0 || mSize == 0)
                return false;

            auto i = GetIndex(key);
            E equals;
            Node *current = mLists[i].first;
            while (current != nullptr) {
                if (equals(current->GetKey(), key)) {
                    return true;
                }

                current = current->GetNext();
            }

            return false;
        }

        bool Remove(const K &key) {
            if (mRange == 0 || mSize == 0)
                return false;

            E equals;
            List &list = mLists[GetIndex(key)];
            Node *current = list.first;

            if (current != nullptr) {
                if (equals(current->GetKey(), key)) {
                    list.first = current->GetNext();
                    current->~Node();
                    mNodeAlloc.Deallocate(current);
                    mSize -= 1;

                    return true;
                }

                Node *prev = current;
                current = current->GetNext();

                while (current != nullptr) {
                    if (equals(current->GetKey(), key)) {
                        prev->GetNext() = current->GetNext();
                        current->~Node();
                        mNodeAlloc.Deallocate(current);
                        mSize -= 1;

                        return true;
                    }

                    prev = current;
                    current = current->GetNext();
                }
            }

            return false;
        }

        void Clear() {
            for (uint64 i = 0; i < mRange; i++) {
                Node *current = mLists[i].first;
                mLists[i].first = nullptr;
                while (current != nullptr) {
                    auto next = current->GetNext();
                    current->~Node();
                    mNodeAlloc.Deallocate(current);
                    current = next;
                }
            }
            mSize = 0;
        }

        V* GetPtr(const K &key) {
            if (mRange == 0 || mSize == 0)
                return nullptr;

            auto i = GetIndex(key);
            E equals;
            Node *current = mLists[i].first;
            while (current != nullptr) {
                if (equals(current->GetKey(), key))
                    return &(current->GetValue());

                current = current->GetNext();
            }

            return nullptr;
        }

        const V* GetPtr(const K &key) const {
            if (mRange == 0 || mSize == 0)
                return {};

            auto i = GetIndex(key);
            E equals;
            Node *current = mLists[i].first;
            while (current != nullptr) {
                if (equals(*current->GetKey(), key))
                    return current->getValue();

                current = current->GetNext();
            }

            return nullptr;
        }

        V& operator[](const K &key) {
            auto* v = GetPtr(key);
            return v? *v: Emplace(key);
        }

        const V& operator[](const K &key) const {
            const auto* v = GetPtr(key);
            return v? *v: Emplace(key);
        }

        bool operator==(const HashTable &other) const {
            if (mSize != other.mSize)
                return false;

            for (const auto &pair: *this) {
                const V *value = other.getPtr(pair.GetFirst()).GetPtr();

                if (value == nullptr)
                    return false;

                if (!(pair.GetValue() != *value))
                    return false;
            }

            return true;
        }

        bool operator!=(const HashTable &other) const {
            return !(*this == other);
        }

        void GetKeys(Array<K> &keys) const {
            keys.EnsureToAdd(GetSize());
            for (const auto &pair: *this) {
                keys.Emplace(pair.GetFirst());
            }
        }

        void GetKeyValues(Array<MapPair> &keyValues) const {
            keyValues.EnsureToAdd(GetSize());
            for (const auto &pair: *this) {
                keyValues.Emplace(pair);
            }
        }

        uint64 GetSize() const {
            return mSize;
        }

        uint64 GetRange() const {
            return mRange;
        }

        bool IsEmpty() const {
            return mSize == 0;
        }

        bool IsNotEmpty() const {
            return mSize > 0;
        }

        float GetLoadFactor() const {
            return mRange > 0 ? (float) mSize / (float) mRange: 0.0f;
        }

        static constexpr uint64 GetSizeOfNode() {
            return sizeof(Node);
        }

        template<typename Tit>
        class TIterator {
        public:
            friend class HashTable<K,V,H,E,A>;

            TIterator(List* list, Node* current, uint64 listIndex, uint64 range) {
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
                        for (uint64 i = mListIndex + 1; i < mRange; i++) {
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
            uint64 mListIndex;
            uint64 mRange;
        };

        TIterator<MapPair> Remove(const TIterator<MapPair> &element) {
            assert(element != end());
            assert(element.mLists == mLists);
            assert(element.mRange == mRange);
            assert(element.mListIndex < mRange);
            assert(element.mCurrent);

            auto next = element;
            ++next;

            RemoveNode(element.mCurrent, element.mListIndex);

            return next;
        }

        TIterator<MapPair> begin() {
            if (GetSize() == 0)
                return end();

            Node* node;
            uint64 listIndex;
            GetFirstNodeNotNull(node, listIndex);
            return TIterator<MapPair>(mLists, node, listIndex, mRange);
        }

        TIterator<MapPair> end() {
            return TIterator<MapPair>(nullptr, nullptr, 0, 0);
        }

        TIterator<const MapPair> begin() const {
            if (GetSize() == 0)
                return end();

            Node* node;
            uint64 listIndex;
            GetFirstNodeNotNull(node, listIndex);
            return TIterator<const MapPair>(mLists, node, listIndex, mRange);
        }

        TIterator<const MapPair> end() const {
            return TIterator<const MapPair>(nullptr, nullptr, 0, 0);
        }

    private:

        void RemoveNode(Node* toRemove, uint64 listIndex) {
            List &list = mLists[listIndex];
            Node *current = list.first;

            if (current != nullptr) {
                if (current == toRemove) {
                    list.first = current->GetNext();
                    current->~Node();
                    mNodeAlloc.Deallocate(current);
                    mSize -= 1;

                    return;
                }

                Node *prev = current;
                current = current->GetNext();

                while (current != nullptr) {
                    if (current == toRemove) {
                        prev->GetNext() = current->GetNext();
                        current->~Node();
                        mNodeAlloc.Deallocate(current);
                        mSize -= 1;
                    }

                    prev = current;
                    current = current->GetNext();
                }
            }
        }

        void Expand() {
            // Allocate initial lists array if needed.
            // It prevents allocating dynamic memory for empty maps
            if (mLists == nullptr) {
                mRange = INITIAL_RANGE;
                mLists = (List *) GetAllocator().Allocate(mRange * sizeof(List));

                for (uint64 i = 0; i < mRange; i++) {
                    mLists[i].first = nullptr;
                }
            // Otherwise expand if needed
            // All elements are relinked to the new range lists (so exposed values pointers will remain valid)
            } else if (GetLoadFactor() > (float) LOAD_FACTOR_PERCENT / 100.0f ) {
                uint64 newRange = mRange * FACTOR;
                List *newLists = (List *) GetAllocator().Allocate(newRange * sizeof(List));

                for (uint64 i = 0; i < newRange; i++) {
                    newLists[i].first = nullptr;
                }

                auto newIndex = [&](const K &key) {
                    H hash;
                    return static_cast<uint32>(hash(key)) % static_cast<uint32>(newRange);
                };

                // Attach old nodes to new lists
                for (uint64 i = 0; i < mRange; i++) {
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

                GetAllocator().Deallocate(mLists);
                mRange = newRange;
                mLists = newLists;
            }
        }

        uint32 GetIndex(const K &key) const {
            H hash;
            return static_cast<uint32>(hash(key)) % static_cast<uint32>(mRange);
        }

        void GetFirstNodeNotNull(Node *&node, uint64 &listIndex) const {
            for (uint64 i = 0; i < mRange; i++) {
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
        PoolAllocator<Alloc> mNodeAlloc;
        List *mLists = nullptr;
        uint64 mRange = 0;
        uint64 mSize = 0;
    };

}

#endif //BERSERK_HASHTABLE_HPP