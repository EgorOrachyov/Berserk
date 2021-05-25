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

#ifndef BERSERK_HASHMAP_HPP
#define BERSERK_HASHMAP_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/Pair.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <initializer_list>

namespace Berserk {

    /**
     * @brief Hash map with open hashing and power of 2 grow.
     *
     * Hash map with open-addressing, linear probe and "tombstones" based elements deletion.
     *
     * @tparam K Generic type of the key
     * @tparam V Generic type of the value
     * @tparam H Hashing predicate used to hash keys
     * @tparam E Equals predicate used to compare keys
     * @tparam A Allocator used for dynamic allocations
     */
    template<typename K, typename V, typename H = Hash<K>, typename E = Equals<K>, typename A = Allocator>
    class HashMap final {
    public:
        using MapPair = Pair<K,V>;
        using Marker = uint8;
        using Alloc = A;
        static const Marker TOMBSTONE = 3;
        static const Marker USED = 2;
        static const Marker UNUSED = 1;

        static const size_t INITIAL_RANGE = 8;
        static const size_t FACTOR = 2;
        static const size_t LOAD_FACTOR_PERCENT = 65;
        static const size_t TOMBSTONES_FACTOR_PERCENT = 40;

        // If in the table elementsCount / range > LOAD_FACTOR_PERCENT / 100
        // then will expand by the FACTOR.
        // Initially table always have INITIAL_RANGE as range

        explicit HashMap(A&& alloc = A()) noexcept
            : mAlloc(std::move(alloc)) {

        }

        HashMap(const std::initializer_list<MapPair> &list) : HashMap<K,V,H,E,A>() {
            Add(list);
        }

        HashMap(const HashMap &other) : mAlloc(other.mAlloc) {
            for (const auto &p: other) {
                Add(p.first(), p.second());
            }
        }

        HashMap(HashMap &&other) noexcept  : mAlloc(std::move(other.mAlloc)) {
            mRange = other.mRange;
            mSize = other.mSize;
            mData = other.mData;
            mUsageMap = other.mUsageMap;

            other.mRange = 0;
            other.mSize = 0;
            other.mData = nullptr;
            other.mUsageMap = nullptr;
        }

        ~HashMap() {
            if (mData) {
                Clear();
                mAlloc.Deallocate(mData);
                mAlloc.Deallocate(mUsageMap);
                mRange = 0;
                mData = nullptr;
                mUsageMap = nullptr;
            }
        }

        HashMap &operator=(const HashMap &other) {
            if (this == &other) {
                return *this;
            }

            HashMap tmp = other;
            *this = std::move(tmp);

            return *this;
        }

        HashMap &operator=(HashMap &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~HashMap();
            new(this) HashMap(std::move(other));
            return *this;
        }

        void Add(const K &key, const V &value) {
            ExpandAndClear();

            E equals;
            auto index = GetIndex(key);

            while (true) {
                // Add new key value
                if (mUsageMap[index] == UNUSED) {
                    new (&mData[index]) MapPair(key, value);
                    mUsageMap[index] = USED;
                    mSize += 1;
                    break;
                }
                // Replace old value, preserve key
                else if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    if (equals(entry.GetFirst(), key)) {
                        auto& v = entry.GetSecond();
                        v.~V();
                        new (&v) V(value);
                        break;
                    }
                }

                index = (index + 1) % mRange;
            }
        }

        void AddIfNotPresent(const K &key, const V &value) {
            ExpandAndClear();

            E equals;
            auto index = GetIndex(key);

            while (true) {
                // Add new key value
                if (mUsageMap[index] == UNUSED) {
                    new (&mData[index]) MapPair(key, value);
                    mUsageMap[index] = USED;
                    mSize += 1;
                    break;
                }
                // Check that we already have key
                else if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    if (equals(entry.GetFirst(), key)) {
                        return;
                    }
                }

                index = (index + 1) % mRange;
            }
        }

        template<typename ... TArgs>
        V &Emplace(const K &key, TArgs &&... args) {
            V valueData(std::forward<TArgs>(args)...);
            K keyData = key;

            return Move(keyData, valueData);
        }

        V& Move(K &key, V &value) {
            ExpandAndClear();

            E equals;
            auto index = GetIndex(key);

            while (true) {
                // Add new key value
                if (mUsageMap[index] == UNUSED) {
                    new (&mData[index]) MapPair(std::move(key), std::move(value));
                    mUsageMap[index] = USED;
                    mSize += 1;
                    break;
                }
                    // Replace old value, preserve key
                else if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    if (equals(entry.GetFirst(), key)) {
                        auto& v = entry.GetSecond();
                        v.~V();
                        new (&v) V(std::move(value));
                        break;
                    }
                }

                index = (index + 1) % mRange;
            }

            return mData[index].GetSecond();
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
            if (mRange == 0 || mSize == 0)
                return false;

            E equals;
            auto index = GetIndex(key);

            while (mUsageMap[index] != UNUSED) {
                // Check possible value
                if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    if (equals(entry.GetFirst(), key)) {
                        return true;
                    }
                }

                index = (index + 1) % mRange;
            }

            return false;
        }

        bool Remove(const K &key) {
            if (mRange == 0 || mSize == 0)
                return false;

            E equals;
            auto index = GetIndex(key);

            while (mUsageMap[index] != UNUSED) {
                // Check possible value
                if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    // Remove entry
                    if (equals(entry.GetFirst(), key)) {
                        mData[index].~MapPair();
                        mUsageMap[index] = TOMBSTONE;
                        mTombstones += 1;
                        mSize -= 1;

                        return true;
                    }
                }

                index = (index + 1) % mRange;
            }

            return false;
        }

        void Clear() {
            for (size_t i = 0; i < mRange; i++) {
                if (mUsageMap[i] == USED) {
                    mData[i].~MapPair();
                }

                mUsageMap[i] = UNUSED;
            }

            mSize = 0;
            mTombstones = 0;
        }

        V* GetPtr(const K &key) {
            if (mRange == 0 || mSize == 0)
                return nullptr;

            E equals;
            auto index = GetIndex(key);

            while (mUsageMap[index] != UNUSED) {
                // Check possible value
                if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    if (equals(entry.GetFirst(), key)) {
                        return &mData[index].GetSecond();
                    }
                }

                index = (index + 1) % mRange;
            }

            return nullptr;
        }


        const V* GetPtr(const K &key) const {
            if (mRange == 0 || mSize == 0)
                return nullptr;

            E equals;
            auto index = GetIndex(key);

            while (mUsageMap[index] != UNUSED) {
                // Check possible value
                if (mUsageMap[index] == USED) {
                    auto& entry = mData[index];

                    if (equals(entry.GetFirst(), key)) {
                        return &mData[index].GetSecond();
                    }
                }

                index = (index + 1) % mRange;
            }

            return nullptr;
        }

        V& operator[](const K &key) {
            auto* v = GetPtr(key);
            return v? *v: Emplace(key);
        }

        bool operator==(const HashMap &other) const {
            if (mSize != other.mSize)
                return false;

            for (const auto &pair: *this) {
                const V *value = other.getPtr(pair.first()).GetPtr();

                if (value == nullptr)
                    return false;

                if (!(pair.GetValue() != *value))
                    return false;
            }

            return true;
        }

        bool operator!=(const HashMap &other) const {
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
            return mRange > 0? (float) mSize / (float) mRange: 0.0f;
        }

        static constexpr size_t GetSizeOfEntry() {
            return sizeof(MapPair);
        }

        template<typename Tit>
        class TIterator {
        public:
            friend class HashMap<K,V,H,E,A>;

            TIterator(size_t current, size_t range, MapPair* data, Marker* usageMap) {
                mCurrent = current;
                mRange = range;
                mData = data;
                mUsageMap = usageMap;
            }

            bool operator==(const TIterator &other) const {
                return mCurrent == other.mCurrent && mData == other.mData;
            }

            bool operator!=(const TIterator &other) const {
                return mCurrent != other.mCurrent || mData != other.mData;
            }

            void operator++() {
                if (mData) {
                    for (size_t i = mCurrent + 1; i < mRange; i++) {
                        if (mUsageMap[i] == USED) {
                            mCurrent = i;
                            return;
                        }
                    }

                    mCurrent = 0;
                    mRange = 0;
                    mData = nullptr;
                    mUsageMap = nullptr;
                }
            }

            Tit &operator*() {
                return mData[mCurrent];
            }

        protected:
            size_t mCurrent;
            size_t mRange;
            MapPair* mData;
            Marker* mUsageMap;
        };

        TIterator<MapPair> Remove(const TIterator<MapPair> &element) {
            assert(element != end());
            assert(element.mCurrent < mRange);
            assert(element.mRange == mRange);
            assert(element.mData == mData);
            assert(element.mUsageMap == mUsageMap);

            auto next = element;
            ++next;

            auto index = element.mCurrent;
            assert(mUsageMap[index] == USED);
            mData[index].~MapPair();
            mUsageMap[index] = TOMBSTONE;
            mSize -= 1;

            return next;
        }

        TIterator<MapPair> begin() {
            if (GetSize() == 0)
                return end();

            auto current = GetFirstEntry();
            return TIterator<MapPair>(current, mRange, mData, mUsageMap);
        }

        TIterator<MapPair> end() {
            return TIterator<MapPair>(0, 0, nullptr, nullptr);
        }

        TIterator<const MapPair> begin() const {
            if (GetSize() == 0)
                return end();

            auto current = GetFirstEntry();
            return TIterator<const MapPair>(current, mRange, mData, mUsageMap);
        }

        TIterator<const MapPair> end() const {
            return TIterator<const MapPair>(0, 0, nullptr, nullptr);
        }

    private:

        void ExpandAndClear() {
            if (mData == nullptr) {
                mRange = INITIAL_RANGE;
                mData = (MapPair*) mAlloc.Allocate(sizeof(MapPair) * mRange);
                mUsageMap = (Marker*) mAlloc.Allocate(sizeof(Marker) * mRange);

                // Mark unused
                Memory::Set(mUsageMap, UNUSED, sizeof(Marker) * mRange);
            }
            else if (GetLoadFactor() > (float) LOAD_FACTOR_PERCENT / 100.0f) {
                size_t newRange = mRange * FACTOR;
                auto* newData = (MapPair*) mAlloc.Allocate(sizeof(MapPair) * newRange);
                auto* newUsageMap = (Marker*) mAlloc.Allocate(sizeof(Marker) * newRange);

                // Mark all as unused
                Memory::Set(newUsageMap, UNUSED, sizeof(Marker) * newRange);

                // Fill data into new place
                Rebuild(newRange, newData, newUsageMap);

                // Release previous data
                mAlloc.Deallocate(mData);
                mAlloc.Deallocate(mUsageMap);

                mTombstones = 0;
                mRange = newRange;
                mData = newData;
                mUsageMap = newUsageMap;
            }
            else if (GetTotalLoadFactor() > (float) LOAD_FACTOR_PERCENT / 100.0f ||
                     GetTombstonesLoadFactor() > (float) TOMBSTONES_FACTOR_PERCENT / 100.0f) {
                size_t newRange = mRange;
                auto* newData = (MapPair*) mAlloc.Allocate(sizeof(MapPair) * newRange);
                auto* newUsageMap = (Marker*) mAlloc.Allocate(sizeof(Marker) * newRange);

                // Mark all as unused
                Memory::Set(newUsageMap, UNUSED, sizeof(Marker) * newRange);

                // Fill data into new place
                Rebuild(newRange, newData, newUsageMap);

                // Release previous data
                mAlloc.Deallocate(mData);
                mAlloc.Deallocate(mUsageMap);

                mTombstones = 0;
                mRange = newRange;
                mData = newData;
                mUsageMap = newUsageMap;
            }
        }

        void Rebuild(size_t newRange, MapPair* newData, Marker* newUsageMap) {
            auto newIndex = [&](const K &key) {
                H hash;
                return hash(key) % newRange;
            };

            for (size_t i = 0; i < mRange; i++) {
                if (mUsageMap[i] == USED) {
                    auto index = newIndex(mData[i].GetFirst());

                    while (newUsageMap[index] != UNUSED) {
                        index = (index + 1) % newRange;
                    }

                    Memory::Copy(&newData[index], &mData[i], sizeof(MapPair));
                    newUsageMap[index] = USED;
                }
            }
        }

        float GetTotalLoadFactor() const {
            return mRange > 0? (float) (mSize + mTombstones) / (float) mRange: 0;
        }

        float GetTombstonesLoadFactor() const {
            return mRange > 0? (float) (mTombstones) / (float) mRange: 0;
        }

        size_t GetIndex(const K& key) const {
            H hash;
            return hash(key) % mRange;
        }

        size_t GetFirstEntry() const {
            for (size_t i = 0; i < mRange; i++) {
                if (mUsageMap[i] == USED) {
                    return i;
                }
            }

            return 0;
        }

    private:
        Alloc mAlloc;
        size_t mRange = 0;
        size_t mSize = 0;
        size_t mTombstones = 0;
        MapPair* mData = nullptr;
        Marker* mUsageMap = nullptr;
    };

}

#endif //BERSERK_HASHMAP_HPP
