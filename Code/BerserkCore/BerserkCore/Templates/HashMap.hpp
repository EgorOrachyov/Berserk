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
#include <BerserkCore/Templates/HashSet.hpp>
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

        struct KeyEquals {
            bool operator()(const MapPair& a, const MapPair& b) const {
                E equals;
                return equals(a.GetFirst(), b.GetFirst());
            }
        };

        struct KeyHash {
            uint32 operator()(const MapPair& pair) const {
                H hash;
                return hash(pair.GetFirst());
            }
        };

        using HashSetType = HashSet<MapPair, KeyHash, KeyEquals, A>;


        explicit HashMap(A&& alloc = A()) noexcept
            : mPairs(std::move(alloc)) {

        }

        HashMap(const std::initializer_list<MapPair> &list) : HashMap<K,V,H,E,A>() {
            Add(list);
        }

        HashMap(const HashMap &other) = default;
        HashMap(HashMap &&other) noexcept = default;
        ~HashMap() = default;

        HashMap &operator=(const HashMap &other) = default;
        HashMap &operator=(HashMap &&other) noexcept = default;

        void Add(const K &key, const V &value) {
            MapPair pair(key, value);
            mPairs[pair] = std::move(pair);
        }

        void AddIfNotPresent(const K &key, const V &value) {
            MapPair pair(key, value);

            if (!mPairs.GetPtr(pair))
                mPairs.Move(pair);
        }

        template<typename ... TArgs>
        V &Emplace(const K &key, TArgs &&... args) {
            V valueData(std::forward<TArgs>(args)...);
            K keyData = key;

            MapPair pair(std::move(keyData), std::move(valueData));
            MapPair& inSet = mPairs[pair];
            inSet = std::move(pair);

            return inSet.GetSecond();
        }

        V& Move(K &key, V &value) {
            MapPair pair(std::move(key), std::move(value));
            MapPair& inSet = mPairs[pair];
            inSet = std::move(pair);

            return inSet.GetSecond();
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
            uint8 mem[sizeof(MapPair)];
            auto lookUp = (MapPair*) mem;
            auto tmpKey = new (&lookUp->GetFirst()) K(key);

            auto ptr = mPairs.GetPtr(*lookUp);
            tmpKey->~K();

            return ptr != nullptr;
        }

        bool Remove(const K &key) {
            uint8 mem[sizeof(MapPair)];
            auto lookUp = (MapPair*) mem;
            auto tmpKey = new (&lookUp->GetFirst()) K(key);

            auto ptr = mPairs.GetPtr(*lookUp);
            tmpKey->~K();

            if (ptr) {
                mPairs.Remove(*ptr);
                return true;
            }

            return false;
        }

        void Clear() {
            mPairs.Clear();
        }

        V* GetPtr(const K &key) {
            uint8 mem[sizeof(MapPair)];
            auto lookUp = (MapPair*) mem;
            auto tmpKey = new (&lookUp->GetFirst()) K(key);

            auto ptr = mPairs.GetPtr(*lookUp);
            tmpKey->~K();

            return ptr? &ptr->GetSecond(): nullptr;
        }


        const V* GetPtr(const K &key) const {
            uint8 mem[sizeof(MapPair)];
            auto lookUp = (MapPair*) mem;
            auto tmpKey = new (&lookUp->GetFirst()) K(key);

            auto ptr = mPairs.GetPtr(*lookUp);
            tmpKey->~K();

            return ptr? &ptr->GetSecond(): nullptr;
        }

        V& operator[](const K &key) {
            auto* v = GetPtr(key);
            return v? *v: Emplace(key);
        }

        bool operator==(const HashMap &other) const {
            return mPairs == other.mPairs;
        }

        bool operator!=(const HashMap &other) const {
            return mPairs != other.mPairs;
        }

        void GetKeys(Array<K> &keys) const {
            keys.EnsureToAdd(GetSize());
            for (const auto &pair: mPairs) {
                keys.Emplace(pair.GetFirst());
            }
        }

        void GetKeyValues(Array<MapPair> &keyValues) const {
            keyValues.EnsureToAdd(GetSize());
            for (const auto &pair: mPairs) {
                keyValues.Emplace(pair);
            }
        }

        size_t GetSize() const {
            return mPairs.GetSize();
        }

        size_t GetRange() const {
            return mPairs.GetRange();
        }

        bool IsEmpty() const {
            return mPairs.IsEmpty();
        }

        bool IsNotEmpty() const {
            return mPairs.IsNotEmpty();
        }

        float GetLoadFactor() const {
            return mPairs.GetLoadFactor();
        }

        static constexpr size_t GetSizeOfEntry() {
            return sizeof(MapPair);
        }

        template<typename Tit>
        using TIterator = typename HashSetType::template TIterator<Tit>;

        TIterator<MapPair> Remove(const TIterator<MapPair> &element) {
            return mPairs.Remove(element);
        }

        TIterator<MapPair> begin() {
            return mPairs.begin();
        }

        TIterator<MapPair> end() {
            return mPairs.end();
        }

        TIterator<const MapPair> begin() const {
            return mPairs.begin();
        }

        TIterator<const MapPair> end() const {
            return mPairs.end();
        }

    private:
        HashSetType mPairs;
    };

}

#endif //BERSERK_HASHMAP_HPP
