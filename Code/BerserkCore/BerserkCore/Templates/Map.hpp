/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MAP_HPP
#define BERSERK_MAP_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/Pair.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Memory/PoolAllocator.hpp>

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
    template<typename K, typename V, typename H = Hash<K>, typename E = Equals<K>, typename A = Allocator>
    class Map final {
    private:

        using MapPair = Pair<K, V>;
        using Alloc = A;

        class Node {
        public:
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

        static const size_t INITIAL_RANGE = 8;
        static const size_t FACTOR = 2;
        static const size_t LOAD_FACTOR_PERCENT = 75;

        // If in the table elementsCount / range > LOAD_FACTOR_PERCENT / 100
        // then will expand by the FACTOR.
        // Initially table always have INITIAL_RANGE as range

        explicit Map(A&& alloc = A()) noexcept
            : mNodeAlloc(GetSizeOfNode(), PoolAllocator<A>::INITIAL_CHUNKS_COUNT, std::move(alloc)) {

        }

        Map(const std::initializer_list<MapPair> &list) : Map<K,V,H,E,A>() {
            Add(list);
        }

        Map(const Map &other)
            : mNodeAlloc(GetSizeOfNode(), other.GetAllocator()) {
            for (const auto &p: other) {
                Add(p.first(), p.second());
            }
        }

        Map(Map &&other) noexcept
                : mNodeAlloc(std::move(other.mNodeAlloc)) {
            mLists = other.mLists;
            mRange = other.mRange;
            mSize = other.mSize;

            other.mLists = nullptr;
            other.mRange = 0;
            other.mSize = 0;
        }

        ~Map() {
            if (mLists) {
                Clear();
                GetAllocator().Deallocate(mLists);
                mLists = nullptr;
                mRange = 0;
            }
        }

        Map &operator=(const Map &other) {
            if (this == &other) {
                return *this;
            }

            this->~Map();
            new(this) Map(other);
            return *this;
        }

        Map &operator=(Map &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~Map();
            new(this) Map(std::move(other));
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
            size_t nodes = 0;
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

        void Add(const std::initializer_list<MapPair> &list) {
            for (auto &p: list) {
                Add(p.GetFirst(), p.GetSecond());
            }
        }

        void Add(const MapPair* pairs, size_t count) {
            for (size_t i = 0; i < count; i++) {
                Add(pairs[i].GetFirst(), pairs[i].GetSecond());
            }
        }

        bool Contains(const K &key) const {
            if (mRange == 0)
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
            if (mRange == 0)
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
            for (size_t i = 0; i < mRange; i++) {
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
            if (mRange == 0)
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
            if (mRange == 0)
                return {};

            auto i = GetIndex(key);
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
                auto i = GetIndex(key);
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
                auto i = GetIndex(key);
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

        bool operator==(const Map &other) const {
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

        bool operator!=(const Map &other) const {
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

        size_t GetSize() const {
            return mSize;
        }

        size_t GetRange() const {
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

        static constexpr size_t GetSizeOfNode() {
            return sizeof(Node);
        }

        template<typename Tit>
        class TIterator {
        public:

            TIterator(List* list, Node* current, size_t listIndex, size_t range) {
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
                        for (size_t i = mListIndex + 1; i < mRange; i++) {
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
            size_t mListIndex;
            size_t mRange;
        };

        TIterator<MapPair> begin() {
            Node* node;
            size_t listIndex;
            GetFirstNodeNotNull(node, listIndex);
            return TIterator<MapPair>(mLists, node, listIndex, mRange);
        }

        TIterator<MapPair> end() {
            return TIterator<MapPair>(nullptr, nullptr, 0, 0);
        }

        TIterator<const MapPair> begin() const {
            Node* node;
            size_t listIndex;
            GetFirstNodeNotNull(node, listIndex);
            return TIterator<const MapPair>(mLists, node, listIndex, mRange);
        }

        TIterator<const MapPair> end() const {
            return TIterator<const MapPair>(nullptr, nullptr, 0, 0);
        }

    private:

        void Expand() {
            // Allocate initial lists array if needed.
            // It prevents allocating dynamic memory for empty maps
            if (mLists == nullptr) {
                mRange = INITIAL_RANGE;
                mLists = (List *) GetAllocator().Allocate(mRange * sizeof(List));

                for (size_t i = 0; i < mRange; i++) {
                    mLists[i].first = nullptr;
                }
            // Otherwise expand if needed
            // All elements are relinked to the new range lists (so exposed values pointers will remain valid)
            } else if (GetLoadFactor() > (float) LOAD_FACTOR_PERCENT / 100.0f ) {
                size_t newRange = mRange * FACTOR;
                List *newLists = (List *) GetAllocator().Allocate(newRange * sizeof(List));

                for (size_t i = 0; i < newRange; i++) {
                    newLists[i].first = nullptr;
                }

                auto newIndex = [&](const K &key) {
                    H hash;
                    return hash(key) % newRange;
                };

                // Attach old nodes to new lists
                for (size_t i = 0; i < mRange; i++) {
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

        size_t GetIndex(const K &key) const {
            H hash;
            return hash(key) % mRange;
        }

        void GetFirstNodeNotNull(Node *&node, size_t &listIndex) const {
            for (size_t i = 0; i < mRange; i++) {
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
        size_t mRange = 0;
        size_t mSize = 0;
    };

}

#endif //BERSERK_MAP_HPP