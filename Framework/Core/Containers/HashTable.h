//
// Created by Egor Orachyov on 03.06.2018.
//

#ifndef BERSERKENGINE_HASHTABLE_H
#define BERSERKENGINE_HASHTABLE_H

#include "SharedList.h"
#include "Essential/Types.h"
#include "Essential/UsageDescriptors.h"
#include "HashFunctions/CRC32.h"
#include "Memory/PoolAllocator.h"
#include "Memory/MemoryAllocators.h"

namespace Berserk
{

    /**
     *  Hash function used for hashing data in table
     */
    typedef UINT32 (*HashFunc)(const CHAR *buffer, INT32 len);

    /**
     * Hash table for elements with key and data fields. Stores keys and data in the internal
     * buffer. Do not require use additional buffers.
     *
     * @warning Require defined == operator for key and data to compare elements
     *
     * @tparam Key Param to find element in table
     * @tparam Data Element stored data
     */
    template <typename Key, typename Data> class DATA_API HashTable
    {
    private:

        /**
         * Node to store keys and data in shared lists
         */
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

        /**
         * Initialize hash table with % hash range
         * @param range Default range = 64
         *
         * @note Default hash function CRC32
         */
        void init(UINT32 range = 64);

        /**
         * Initialize hash table with desired hash function and range size
         * @param func Pointer to hash function
         * @param range
         */
        void init(HashFunc func, UINT32 range = 64);

        /**
         * Call default destructors for keys and data, free all the used memory
         * and set internal values to 0 (delete internal buffer)
         */
        void destroy();

        /**
         * Delete all the elements by default destructor
         */
        void empty();

        /**
         * Add element
         *
         * @warning Rewrite data if element has been already stored in table
         *
         * @param key Key value
         * @param data Data to store in table
         */
        void add(const Key& key, const Data& data);

        /**
         * Set hash function to CRC32
         */
        void setDefaultHashFunction();

        /**
         * Set desired hash function
         * @param func Hash function
         */
        void setHashFunction(HashFunc func);

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        Data& get(const Key& key) const;

        /**
         * Remove element by its key
         * @param key Element key
         */
        void  remove(const Key& key);

        /**
         * Is table contains element with this key
         * @param key Element key to find
         * @return true if contains
         */
        bool contains(const Key& key) const;

        /**
         * Is table was successfully initialized
         * @return true if initalized
         */
        bool isInitialized() const;

        /**
         * @return Num of elemnet in table
         */
        UINT32 getSize() const;

        /**
         * @return Range of internal lists
         */
        UINT32 getRange() const;

        /**
         * @return Capacity without expansion
         */
        UINT32 getCapacity() const;

        /**
         * @return Load factor (Size / used lists)
         */
        FLOAT32 getLoadFactor() const;

    private:

        UINT32 mSize;                       // Num of elements in table
        UINT32 mRange;                      // Range of hash function
        UINT32 mHashLen;                    // Length of key in chars
        HashFunc mFunc;                     // Pointer to hash function
        PoolAllocator mPool;                // Buffer for data
        SharedList<Node>* mLists;           // Lists for handling hash->data

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
        ASSERT(mRange, "Table is not initialized");

        CHAR* rkey = (CHAR*)&key;
        UINT32 hash = mFunc(rkey, mHashLen) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mData == data)
            {
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
        ASSERT(mRange, "Table is not initialized");

        CHAR* rkey = (CHAR*)&key;
        UINT32 hash = mFunc(rkey, mHashLen) % mRange;

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
        ASSERT(mRange, "Table is not initialized");

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
        ASSERT(mRange, "Table is not initialized");

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
        ASSERT(mRange, "Table is not initialized");

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