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

#ifndef BERSERK_HASHSET_HPP
#define BERSERK_HASHSET_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/Pair.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <initializer_list>

namespace Berserk {

    /**
     * @brief Hash set with open hashing and power of 2 grow.
     *
     * Hash set with open-addressing, linear probe and "tombstones" based elements deletion.
     *
     * @tparam K Generic type of the key
     * @tparam H Hashing predicate used to hash keys
     * @tparam E Equals predicate used to compare keys
     * @tparam A Allocator used for dynamic allocations
     */
    template<typename K, typename H = Hash<K>, typename E = Equals<K>, typename A = Allocator>
    class HashSet final {
    public:
        using Marker = uint8;
        using Alloc = A;
        static const Marker TOMBSTONE = 3;
        static const Marker USED = 2;
        static const Marker UNUSED = 1;

        static const uint64 INITIAL_RANGE = 8;
        static const uint64 FACTOR = 2;
        static const uint64 LOAD_FACTOR_PERCENT = 65;
        static const uint64 TOMBSTONES_FACTOR_PERCENT = 40;

        // If in the table elementsCount / range > LOAD_FACTOR_PERCENT / 100
        // then will expand by the FACTOR.
        // Initially table always have INITIAL_RANGE as range

        explicit HashSet(A&& alloc = A()) noexcept
                : mAlloc(std::move(alloc)) {

        }

        HashSet(const std::initializer_list<K> &list) : HashSet<K,H,E,A>() {
            Add(list);
        }

        HashSet(const HashSet &other) : mAlloc(other.mAlloc) {
            for (const auto &p: other) {
                Add(p);
            }
        }

        HashSet(HashSet &&other) noexcept  : mAlloc(std::move(other.mAlloc)) {
            mRange = other.mRange;
            mSize = other.mSize;
            mData = other.mData;
            mUsageMap = other.mUsageMap;

            other.mRange = 0;
            other.mSize = 0;
            other.mData = nullptr;
            other.mUsageMap = nullptr;
        }

        ~HashSet() {
            if (mData) {
                Clear();
                mAlloc.Deallocate(mData);
                mAlloc.Deallocate(mUsageMap);
                mRange = 0;
                mData = nullptr;
                mUsageMap = nullptr;
            }
        }

        HashSet &operator=(const HashSet &other) {
            if (this == &other) {
                return *this;
            }

            HashSet tmp = other;
            *this = std::move(tmp);

            return *this;
        }

        HashSet &operator=(HashSet &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~HashSet();
            new(this) HashSet(std::move(other));
            return *this;
        }

        void Add(const K &key) {
            ExpandAndClear();

            uint32 pos;
            bool found = GetKeyPosition(key, pos);

            if (!found) {
                new (&mData[pos]) K(key);
                mUsageMap[pos] = USED;
                mSize += 1;
            }
        }

        K& Move(K &key) {
            ExpandAndClear();

            uint32 pos;
            bool found = GetKeyPosition(key, pos);

            if (!found) {
                new (&mData[pos]) K(std::move(key));
                mUsageMap[pos] = USED;
                mSize += 1;
            }

            return mData[pos];
        }

        void Add(const std::initializer_list<K> &list) {
            for (auto &p: list) {
                Add(p);
            }
        }

        void Add(const K* keys, uint64 count) {
            for (uint64 i = 0; i < count; i++) {
                Add(keys[i]);
            }
        }

        bool Contains(const K &key) const {
            uint32 pos;
            return GetKeyPosition(key, pos);
        }

        bool Remove(const K &key) {
            uint32 pos;
            bool found = GetKeyPosition(key, pos);

            if (found) {
                mData[pos].~K();
                mUsageMap[pos] = TOMBSTONE;
                mTombstones += 1;
                mSize -= 1;

                return true;
            }

            return false;
        }

        void Clear() {
            for (uint64 i = 0; i < mRange; i++) {
                if (mUsageMap[i] == USED) {
                    mData[i].~K();
                }

                mUsageMap[i] = UNUSED;
            }

            mSize = 0;
            mTombstones = 0;
        }

        K* GetPtr(const K &key) {
            return GetPtrInternal(key);
        }

        const K* GetPtr(const K &key) const {
            return GetPtrInternal(key);
        }

        K& operator[](const K &key) {
            auto* v = GetPtr(key);
            if (v)
                return *v;
            else {
                K k = key;
                return Move(k);
            }
        }

        bool operator==(const HashSet &other) const {
            if (mSize != other.mSize)
                return false;

            for (const auto &key: *this) {
                if (!other.Contains(key))
                    return false;
            }

            return true;
        }

        bool operator!=(const HashSet &other) const {
            return !(*this == other);
        }

        void GetKeys(Array<K> &keys) const {
            keys.EnsureToAdd(GetSize());
            for (const auto &key: *this) {
                keys.Emplace(key);
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
            return mRange > 0? (float) mSize / (float) mRange: 0.0f;
        }

        static constexpr uint64 GetSizeOfEntry() {
            return sizeof(K);
        }

        template<typename Tit>
        class TIterator {
        public:
            friend class HashSet<K,H,E,A>;

            TIterator(uint64 current, uint64 range, K* data, Marker* usageMap) {
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
                    for (uint64 i = mCurrent + 1; i < mRange; i++) {
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
            uint64 mCurrent;
            uint64 mRange;
            K* mData;
            Marker* mUsageMap;
        };

        TIterator<K> Remove(const TIterator<K> &element) {
            assert(element != end());
            assert(element.mCurrent < mRange);
            assert(element.mRange == mRange);
            assert(element.mData == mData);
            assert(element.mUsageMap == mUsageMap);

            auto next = element;
            ++next;

            auto index = element.mCurrent;
            assert(mUsageMap[index] == USED);
            mData[index].~K();
            mUsageMap[index] = TOMBSTONE;
            mSize -= 1;

            return next;
        }

        TIterator<K> begin() {
            if (GetSize() == 0)
                return end();

            auto current = GetFirstEntry();
            return TIterator<K>(current, mRange, mData, mUsageMap);
        }

        TIterator<K> end() {
            return TIterator<K>(0, 0, nullptr, nullptr);
        }

        TIterator<const K> begin() const {
            if (GetSize() == 0)
                return end();

            auto current = GetFirstEntry();
            return TIterator<const K>(current, mRange, mData, mUsageMap);
        }

        TIterator<const K> end() const {
            return TIterator<const K>(0, 0, nullptr, nullptr);
        }

    private:

        bool GetKeyPosition(const K& key, uint32& position) const {
            E equals;
            position = GetIndex(key);

            if (mRange == 0 || mSize == 0)
                return false;

            while (mUsageMap[position] != UNUSED) {
                // Check possible value
                if (mUsageMap[position] == USED) {
                    auto& entry = mData[position];

                    if (equals(entry, key)) {
                        return true;
                    }
                }

                position = (position + 1) % mRange;
            }

            return false;
        }

        K* GetPtrInternal(const K &key) const {
            if (mRange == 0)
                return nullptr;

            uint32 pos;
            bool found = GetKeyPosition(key, pos);

            return found? &mData[pos]: nullptr;
        }

        void ExpandAndClear() {
            if (mData == nullptr) {
                mRange = INITIAL_RANGE;
                mData = (K*) mAlloc.Allocate(sizeof(K) * mRange);
                mUsageMap = (Marker*) mAlloc.Allocate(sizeof(Marker) * mRange);

                // Mark unused
                Memory::Set(mUsageMap, UNUSED, sizeof(Marker) * mRange);
            }
            else if (GetLoadFactor() > (float) LOAD_FACTOR_PERCENT / 100.0f) {
                uint64 newRange = mRange * FACTOR;
                auto* newData = (K*) mAlloc.Allocate(sizeof(K) * newRange);
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
                uint64 newRange = mRange;
                auto* newData = (K*) mAlloc.Allocate(sizeof(K) * newRange);
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

        void Rebuild(uint64 newRange, K* newData, Marker* newUsageMap) {
            auto newIndex = [&](const K &key) {
                H hash;
                return hash(key) % newRange;
            };

            for (uint64 i = 0; i < mRange; i++) {
                if (mUsageMap[i] == USED) {
                    auto index = newIndex(mData[i]);

                    while (newUsageMap[index] != UNUSED) {
                        index = (index + 1) % newRange;
                    }

                    Memory::Copy(&newData[index], &mData[i], sizeof(K));
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

        uint64 GetIndex(const K& key) const {
            H hash;
            return hash(key) % mRange;
        }

        uint64 GetFirstEntry() const {
            for (uint64 i = 0; i < mRange; i++) {
                if (mUsageMap[i] == USED) {
                    return i;
                }
            }

            return 0;
        }

    private:
        Alloc mAlloc;
        uint64 mRange = 0;
        uint64 mSize = 0;
        uint64 mTombstones = 0;
        K* mData = nullptr;
        Marker* mUsageMap = nullptr;
    };

}

#endif //BERSERK_HASHSET_HPP