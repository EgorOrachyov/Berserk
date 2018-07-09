//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_HASHTABLE_H
#define BERSERKENGINE_HASHTABLE_H

#include "SharedList.h"
#include "Essential/Types.h"
#include "HashFunctions/CRC32.h"
#include "Memory/PoolAllocator.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{

    typedef UINT32 (*HashFunc)(const CHAR *buffer, INT32 len);

    template <typename Key, typename Data> class HashTable
    {
    private:

        struct Node
        {
        public:

            Node(const Key& key, const Data& data)
            {
                mKey = key;
                mData = data;
            }

            ~Node()
            {
                mKey.~Key();
                mData.~Data();
            }

            const bool operator == (const Node& node)
            {
                return (this->mKey == node.mKey && this->mData == node.mData);
            }

            Key mKey;
            Data mData;
        };

    public:

        HashTable();
        ~HashTable();

        void init(UINT32 range = 64);
        void init(HashFunc func, UINT32 range = 64);
        void destroy();

        void empty();

        void add(const Key& key, const Data& data);

        void setDefaultHashFunction();
        void setHashFunction(HashFunc func);

        Data& get(const Key& key) const;
        void  remove(const Key& key);
        bool contains(const Key& key) const;

        bool isInitialized() const;

        UINT32 getSize() const;
        UINT32 getRange() const;
        UINT32 getCapacity() const;
        FLOAT32 getLoadFactor() const;

    private:

        UINT32 mSize;
        UINT32 mRange;
        UINT32 mHashLen;
        HashFunc mFunc;
        PoolAllocator mPool;
        SharedList<Node>* mLists;

    };

    template <typename Key, typename Data>
    HashTable<Key, Data>::HashTable()
    {
        mSize = 0;
        mRange = 0;
        mFunc = NULL;
        mLists = NULL;
    }

    template <typename Key, typename Data>
    HashTable<Key, Data>::~HashTable()
    {
        PUSH("Delete hash table %p with shared lists buffer %p\n", this, mLists);
        destroy();
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::init(UINT32 range)
    {
        if (isInitialized())
        {
            WARNING("Hash table has been initialized");
            return;
        }

        mSize = 0;
        mRange = range;
        mFunc = hashCRC32;
        mHashLen = (sizeof(Key) / sizeof(CHAR));

        mLists = (SharedList<Node>*)mem_calloc(range, sizeof(SharedList<Node>));

        mPool.init((UINT16)mLists[0].getSizeOfNode(), 64);

        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].init(&mPool);
        }
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::init(HashFunc func, UINT32 range)
    {
        if (isInitialized())
        {
            WARNING("Hash table has been initialized");
            return;
        }

        mSize = 0;
        mRange = range;
        mFunc = func;
        mHashLen = (sizeof(Key) / sizeof(CHAR));

        mLists = (SharedList<Node>*)mem_calloc(range, sizeof(SharedList<Node>));

        mPool.init((UINT16)mLists[0].getSizeOfNode(), 64);

        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].init(&mPool);
        }
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::destroy()
    {
        if (isInitialized())
        {
            for(UINT32 i = 0; i < mRange; i++)
            {
                mLists[i].empty();
                mLists[i].~SharedList();
            }

            mem_free(mLists);
            mPool.reset();
        }

        mSize = 0;
        mRange = 0;
        mFunc = NULL;
        mLists = NULL;
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::empty()
    {
        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].empty();
        }
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::add(const Key& key, const Data& data)
    {
        CHAR* rkey = (CHAR*)&key;
        UINT32 hash = mFunc(rkey, mHashLen) % mRange;

        bool isFound = false;
        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mData == data)
            {
                isFound = true;
                list.getCurrent().mData = data;
                return;
            }
        }

        list.add(Node(key,data));
        mSize += 1;
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::setDefaultHashFunction()
    {
        mFunc = hashCRC32;
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::setHashFunction(HashFunc func)
    {
        ASSERT(func, "NULL pointer to hash function");
        mFunc = func;
    }

    template <typename Key, typename Data>
    Data& HashTable<Key, Data>::get(const Key& key) const
    {
        CHAR* rkey = (CHAR*)&key;
        UINT32 hash = mFunc(rkey, mHashLen) % mRange;

        bool isFound = false;
        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return list.getCurrent().mData;
            }
        }

        return *(Data*)(NULL);
    }

    template <typename Key, typename Data>
    void HashTable<Key, Data>::remove(const Key& key)
    {
        CHAR* rkey = (CHAR*)&key;
        UINT32 hash = mFunc(rkey, mHashLen) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                Node& node = list.getCurrent();
                list.remove(node);
                mSize -= 1;

                return;
            }
        }
    }

    template <typename Key, typename Data>
    bool HashTable<Key, Data>::contains(const Key& key) const
    {
        CHAR* rkey = (CHAR*)&key;
        UINT32 hash = mFunc(rkey, mHashLen) % mRange;

        bool isFound = false;
        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                isFound = true;
                return isFound;
            }
        }

        return isFound;
    }

    template <typename Key, typename Data>
    bool HashTable<Key, Data>::isInitialized() const
    {
        return (mLists != NULL);
    }

    template <typename Key, typename Data>
    UINT32 HashTable<Key, Data>::getSize() const
    {
        return mSize;
    }

    template <typename Key, typename Data>
    UINT32 HashTable<Key, Data>::getRange() const
    {
        return mRange;
    }

    template <typename Key, typename Data>
    UINT32 HashTable<Key, Data>::getCapacity() const
    {
        return mPool.getCapacity();
    }

    template <typename Key, typename Data>
    FLOAT32 HashTable<Key, Data>::getLoadFactor() const
    {
        INT32 notEmptyLists = 0;
        for(UINT32 i = 0; i < mRange; i++)
        {
            notEmptyLists += (mLists[i].getSize() > 0? 1 : 0);
        }

        printf("%i\n", notEmptyLists);

        if (notEmptyLists == 0) notEmptyLists = 0;
        return (FLOAT32)mSize / notEmptyLists;
    }

} // namespace Berserk

#endif //BERSERKENGINE_HASHTABLE_H