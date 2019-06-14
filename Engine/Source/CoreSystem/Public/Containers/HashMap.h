//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_HASHMAP_H
#define BERSERK_HASHMAP_H

#include "Misc/Crc32.h"
#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Object/NewDelete.h"
#include "Memory/Allocator.h"
#include "Memory/IAllocator.h"
#include "Memory/PoolAllocator.h"
#include "Containers/SharedList.h"
#include "Profiling/ProfilingMacro.h"

namespace Berserk
{

    /**
     *
     * @tparam K
     * @tparam V
     */
    template <typename K, typename V>
    class HashNode
    {
    public:

        HashNode(const K& key, const V& value) : mKey(key), mValue(value) {}

        ~HashNode() = default;

        void set(const K& key, const V& value)
        {
            memcpy(&mKey,   &key,   sizeof(K));
            memcpy(&mValue, &value, sizeof(V));
        }

        void update(const V& value)
        {
            memcpy(&mValue, &value, sizeof(V));
        }

        K& key()
        {
            return mKey;
        }

        V& value()
        {
            return mValue;
        }

    private:

        K mKey;
        V mValue;

    };

    /**
     *
     * @tparam K
     * @tparam V
     */
    template <typename K, typename V>
    class HashMap
    {
    private:

        typedef HashNode<K,V> Node;

    public:

        /** Does not support small ranges for hashing efficiency */
        static const uint32 MIN_HASH_RANGE = 16;

        /** Default range for hashing efficiency */
        static const uint32 DEFAULT_HASH_RANGE = 64;

        /** Default hashing method via casting key to the array of chars */
        static uint32 defaultHashing(const void* key)
        {
            auto ptr = (char*)(&key);
            auto hash = Crc32::hash(ptr, sizeof(K));

            return hash;
        }

    public:

        HashMap() : mRange(0),
                    mSize(0),
                    mIteratorBucket(0),
                    mIterator(0),
                    mList(nullptr),
                    mHashing(nullptr),
                    mPool(nullptr),
                    mAllocator(nullptr)
        {

        }

        GENERATE_NEW_DELETE(HashMap)

    public:

        /**
         * Initializes internal pool and lists and prepares map
         * @param hashing Pointer to custom hashing method or nullptr if chosen default
         * @param range   Range of hashing (must be more than MIN_HASH_RANGE)
         */
        explicit HashMap(Crc32::Hashing hashing, PoolAllocator* pool, uint32 range = DEFAULT_HASH_RANGE, IAllocator* allocator = nullptr);

        ~HashMap();

        /**
         * Removes from map element with key if it exists
         * @param key
         */
        void remove(const K& key);

        /** Deletes all the elements from the map */
        void empty();

        void nullify()
        {
            mRange = 0;
            mSize = 0;
            mIteratorBucket = 0;
            mIterator = 0;
            mList = nullptr;
            mPool = nullptr;
            mAllocator = nullptr;
        }

        /**
         * Add element in the map and replace old value whether it exists
         * @param key   Key of element
         * @param value New (First) value
         */
        void add(const K& key, const V& value);

        /**
         * @param  key Key of the element
         * @return Pointer to the element whether it exists or nullptr
         */
        V*   operator [] (const K& key) ;

        /** @return Start iterating through map an get first element */
        HashNode<K,V>* iterate();

        /** @return Next element in iteration or nullptr */
        HashNode<K,V>* next();

        /** @return Key of the current iterable element or nullptr */
        K* key();

        /** @return Value of the current iterable element or nullptr */
        V* value();

        /** @return Number of element in the map */
        uint32 getSize() const;

        /** @return Range of the hashing */
        uint32 getRange() const;

        static uint32 getNodeSize()
        {
            return SharedList<Node>::getNodeSize();
        }

        /** @return Memory cost of this resource (on CPU side only) */
        uint32 getMemoryUsage() const;

        /** @return Load factor = mSize / mRange */
        float32 getLoadFactor() const;

    private:

        uint32 hash(const K& key);

    private:

        uint32  mRange;
        uint32  mSize;
        uint32  mIteratorBucket;
        Node*   mIterator;
        Crc32::Hashing mHashing;
        SharedList<Node>* mList;
        PoolAllocator*    mPool;
        IAllocator*  mAllocator;

    };

    template <typename K, typename V>
    HashMap<K,V>::HashMap(Crc32::Hashing hashing, PoolAllocator* pool, uint32 range, IAllocator* allocator)
    {
        FAIL(range >= MIN_HASH_RANGE, "Range must be more than %u", MIN_HASH_RANGE);

        if (hashing) mHashing = hashing;
        else mHashing = defaultHashing;

        mRange = range;
        mSize = 0;
        mIterator = nullptr;
        mIteratorBucket = 0;
        mPool = pool;

        if (allocator) mAllocator = allocator;
        else mAllocator = &Allocator::getSingleton();

        mList = (SharedList<Node>*) mAllocator->allocate(mRange * sizeof(SharedList<Node>));

        for (uint32 i = 0; i < mRange; i++)
        {
            new(&mList[i]) SharedList<Node>(mPool);
        }
    }

    template <typename K, typename V>
    HashMap<K,V>::~HashMap()
    {
        if (mList)
        {
#if PROFILE_HASH_MAP
            PUSH("Hash map: delete with range: %u | list: %p", mRange, mList);
#endif

            empty();
            mAllocator->free(mList);
            mList = nullptr;
            mPool = nullptr;
            mAllocator = nullptr;
        }
    }

    template <typename K, typename V>
    void HashMap<K,V>::remove(const K &key)
    {
        auto h = hash(key);
        SharedList<Node>& bucket = mList[h];

        uint32 i = 0;

        for (auto e = bucket.iterate(); e != nullptr; e = bucket.next())
        {
            if (e->key() == key)
            {
                bucket.remove(i);
                mSize -= 1;
                return;
            }

            i += 1;
        }
    }

    template <typename K, typename V>
    void HashMap<K,V>::empty()
    {
        for (uint32 i = 0; i < mRange; i++)
        {
            mList[i].empty();
        }

        mSize = 0;
    }

    template <typename K, typename V>
    void HashMap<K,V>::add(const K &key, const V &value)
    {
        auto h = hash(key);
        SharedList<Node>& bucket = mList[h];

        for (auto e = bucket.iterate(); e != nullptr; e = bucket.next())
        {
            if (e->key() == key)
            {
                e->update(value);
                return;
            }
        }

        bucket += Node(key, value);
        mSize += 1;
    }

    template <typename K, typename V>
    V* HashMap<K,V>::operator[](const K &key)
    {
        auto h = hash(key);
        SharedList<Node>& bucket = mList[h];

        for (auto e = bucket.iterate(); e != nullptr; e = bucket.next())
        {
            if (e->key() == key)
            {
                return &(e->value());
            }
        }

        return nullptr;
    }

    template <typename K, typename V>
    HashNode<K,V>* HashMap<K,V>::iterate()
    {
        mIterator = nullptr;
        mIteratorBucket = 0;

        while (mIteratorBucket < mRange)
        {
            mIterator = mList[mIteratorBucket].iterate();
            if (mIterator) break;
            else mIteratorBucket += 1;
        }

        return mIterator;
    }

    template <typename K, typename V>
    HashNode<K,V>* HashMap<K,V>::next()
    {
        if (mIterator)
        {
            mIterator = mList[mIteratorBucket].next();
            if (mIterator) return mIterator;
        }

        mIteratorBucket += 1;

        if (mIteratorBucket >= mRange)
        {
            return nullptr;
        }
        else
        {
            mIterator = nullptr;

            while (mIteratorBucket < mRange)
            {
                mIterator = mList[mIteratorBucket].iterate();
                if (mIterator) break;
                else mIteratorBucket += 1;
            }

            return mIterator;
        }
    }

    template <typename K, typename V>
    K* HashMap<K,V>::key()
    {
        return (mIterator ? &mIterator->key() : nullptr);
    }

    template <typename K, typename V>
    V* HashMap<K,V>::value()
    {
        return (mIterator ? &mIterator->value() : nullptr);
    }

    template <typename K, typename V>
    uint32 HashMap<K,V>::getSize() const
    {
        return mSize;
    }

    template <typename K, typename V>
    uint32 HashMap<K,V>::getRange() const
    {
        return mRange;
    }

    template <typename K, typename V>
    uint32 HashMap<K,V>::getMemoryUsage() const
    {
        return mSize * sizeof(HashNode<K,V>) + mRange * sizeof(SharedList<Node>);
    }

    template <typename K, typename V>
    float32 HashMap<K,V>::getLoadFactor() const
    {
        return (float32)mSize / (float32)mRange;
    }

    template <typename K, typename V>
    uint32 HashMap<K,V>::hash(const K &key)
    {
        auto dirty = mHashing((void*)(&key));
        return (dirty % mRange);
    }

} // namespace Berserk

#endif //BERSERK_HASHMAP_H