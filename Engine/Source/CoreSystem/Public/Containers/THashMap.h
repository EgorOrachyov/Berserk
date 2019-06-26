//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_THASHMAP_H
#define BERSERK_THASHMAP_H

#include <Misc/Crc32.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <Containers/TLinkedList.h>

namespace Berserk
{

    template <typename K, typename V>
    class CORE_EXPORT THashMap : public TMap<K,V>
    {
    public:

        /** Hashing method signature to provide custom and default hashing algos for keys */
        typedef uint32 (*HashFunction)(const K& key);

        /** Comparetor used to compare keys when put values or check contains */
        typedef bool (*CompareFunction)(const K& a, const K& b);

        /** Actual buckets withs pairs */
        typedef TArray<TLinkedList<TPair<K,V>>> BucketsList;

        /** List with pairs for one hash value mapping */
        typedef TLinkedList<TPair<K,V>> Bucket;

        /** Initial size of list (range) */
        static const uint32 INITIAL_LIST_SIZE = 8;

        /** Value to expand whether load factor is greater than */
        static const float32 LOAD_FACTOR_TO_EXPAND;

        /** How to scale range */
        static const float32 EXPAND_FACTOR;

    public:

        GENERATE_NEW_DELETE(THashMap);

        explicit THashMap(IAllocator& listAllocator = Allocator::get(),
                          IAllocator& bucketsAllocator = Allocator::get())
                : mBucketsAllocator(bucketsAllocator),
                  mBucketsList(listAllocator),
                  mIterator(createIterator())
        {

        }

        explicit THashMap(uint32 range,
                          IAllocator& listAllocator = Allocator::get(),
                          IAllocator& bucketsAllocator = Allocator::get())
                : mBucketsAllocator(bucketsAllocator),
                  mBucketsList(listAllocator),
                  mIterator(createIterator()),
                  mRange(range)
        {
            assertion_dev(range > INITIAL_LIST_SIZE);
            for (uint32 i = 0; i < mRange; i++)
            {
                mBucketsList.emplace(mBucketsAllocator);
            }
        }

        /** Prohibited */
        THashMap(const THashMap& array) = delete;

        /** Prohibited */
        THashMap(const THashMap&& array) = delete;

        ~THashMap() override
        {
            mSize = 0;
            mRange = 0;
        }

        bool contains(const K &key) const override
        {
            return get(key) != nullptr;
        }

        void put(const K &key, const V &value) override
        {
            expand();

            V* found = nullptr;

            Bucket& bucket = getBucket(key);
            auto iterator = bucket.createIterator();
            for (auto pair = iterator.begin(); pair != nullptr; pair = iterator.next())
            {
                if (mCompare(*pair->key(), key))
                {
                    found = pair->value();
                    break;
                }
            }

            if (found == nullptr)
            {
                mUsedBuckets += (bucket.getSize() == 0 ? 1 : 0);
                mSize += 1;
                mLoadFactor = (float32) mSize / (float32) mUsedBuckets;
                bucket.emplace(key, value);
            }
            else
            {
                memcpy(found, &value, sizeof(V));
            }
        }

        V *get(const K &key) const override
        {
            if (mSize == 0) return nullptr;

            Bucket& bucket = getBucket(key);
            auto iterator = bucket.createIterator();
            for (auto pair = iterator.begin(); pair != nullptr; pair = iterator.next())
            {
                if (mCompare(*pair->key(), key))
                {
                    return pair->value();
                }
            }

            return nullptr;
        }

        void setHashFunction(HashFunction function)
        {
            mHashing = function;
        }

        void setCompareFunction(CompareFunction function)
        {
            mCompare = function;
        }

        uint32 getRange() const
        {
            return mRange;
        }

        uint32 getUsedBuckets() const
        {
            return mUsedBuckets;
        }

        float32 getLoadFactor() const
        {
            return mLoadFactor;
        }

        uint32 getSize() const override
        {
            return mSize;
        }

        /** @return Size of node in the map (size of pair (key,value)) */
        static uint32 getNodeSize()
        {
            return TLinkedList<TPair<K,V>>::getNodeSize();
        }

        uint32 getMemoryUsage() const override
        {
            return mBucketsList.getMemoryUsage() +
                   Bucket::getNodeSize() * mSize;
        }

        TPair<K, V> *begin() const override
        {
            return mIterator.begin();
        }

        TPair<K, V> *next() const override
        {
            return mIterator.next();
        }

    protected:

        TPair<K, V> *emplace_uninitialized(const K &key) override
        {
            expand();

            Bucket& bucket = getBucket(key);
            mUsedBuckets += (bucket.getSize() == 0 ? 1 : 0);
            mSize += 1;
            mLoadFactor = (float32) mSize / (float32) mUsedBuckets;
            TPair<K,V>* pair = bucket.addUninitialized();

            return pair;
        }

        /** @return Needed bucket via hash */
        Bucket& getBucket(const K& key) const
        {
            uint32 hash = mHashing(key) % mRange;
            return mBucketsList.get(hash);
        }

        void expand()
        {
            if (mRange == 0)
            {
                for (uint32 i = 0; i < INITIAL_LIST_SIZE; i++)
                {
                    mBucketsList.emplace(mBucketsAllocator);
                }

                mRange = INITIAL_LIST_SIZE;
            }
            else if (mLoadFactor >= LOAD_FACTOR_TO_EXPAND)
            {
                auto newRange = (uint32)(EXPAND_FACTOR * (float32)mRange);
                uint8 memory[sizeof(THashMap<K,V>)];
                THashMap* map = new (memory) THashMap<K,V>(newRange, mBucketsList.getAllocator(), mBucketsAllocator);

                map->setHashFunction(mHashing);

                for (uint32 i = 0; i < mBucketsList.getSize(); i++)
                {
                    Bucket& bucket = mBucketsList.get(i);
                    auto itr = bucket.createIterator();
                    for (auto pair = itr.begin(); pair != nullptr; pair = itr.next())
                    {
                        map->put(*pair->key(), *pair->value());
                    }
                    bucket.clearNoDestructorCall();
                }

                mBucketsList.clearNoDestructorCall();
                mBucketsList.~TArray();
                memcpy(this, map, sizeof(THashMap<K,V>));
            }
        }

    public:

        class Iterator : public TIterator<TPair<K,V>>
        {
        public:

            Iterator(const BucketsList& list) : mList(list)
            {

            }

            ~Iterator() override
            {

            }

            /** @copydoc TIterator::begin() */
            TPair<K, V> *begin() const override
            {
                mListIterator = mList.createIterator();
                for (Bucket* bucket = mListIterator.begin(); bucket != nullptr; bucket = mListIterator.next())
                {
                    auto tmp = bucket->createIterator();
                    auto pair = tmp.begin();
                    if (pair != nullptr)
                    {
                        mBucketIterator = tmp;
                        return pair;
                    }
                }

                return nullptr;
            }

            /** @copydoc TIterator::next() */
            TPair<K, V> *next() const override
            {
                auto pair = mBucketIterator.next();
                if (pair != nullptr)
                {
                    return pair;
                }

                for (Bucket* bucket = mListIterator.next(); bucket != nullptr; bucket = mListIterator.next())
                {
                    auto tmp = bucket->createIterator();
                    auto pair = tmp.begin();
                    if (pair != nullptr)
                    {
                        mBucketIterator = tmp;
                        return pair;
                    }
                }

                return nullptr;
            }

        private:

            const BucketsList& mList;
            mutable typename Bucket::Iterator mBucketIterator;
            mutable typename BucketsList::Iterator mListIterator;

        };

        /**
         * Creates special THashMap iterator
         * @return Instance (to be copied)
         */
        Iterator createIterator() const
        {
            return Iterator(mBucketsList);
        }

    private:

        /** For TLinkedLists in the array */
        IAllocator& mBucketsAllocator;

        /** Table content */
        BucketsList mBucketsList;

        /** Hashing range */
        uint32 mRange = 0;

        /** Current num of elements */
        uint32 mSize = 0;

        /** Number of non-empty buckets */
        uint32 mUsedBuckets = 0;

        /** How actually buckets filled (mSize / mUsed) */
        float32 mLoadFactor = 0;

        /** Hashing algorithm  */
        HashFunction mHashing = [](const K& key){ return Crc32::hash((char*)&key, sizeof(K)); };

        /** Compare func */
        CompareFunction mCompare = [](const K& a, const K& b){ return a == b; };

        /** Internal iterator for the map */
        mutable Iterator mIterator;

    };

    template <typename K, typename V>
    const float32 THashMap<K,V>::LOAD_FACTOR_TO_EXPAND = 1.6;

    template <typename K, typename V>
    const float32 THashMap<K,V>::EXPAND_FACTOR = 2.0;

} // namespace Berserk

#endif //BERSERK_THASHMAP_H