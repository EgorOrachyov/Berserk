//
// Created by Egor Orachyov on 11.07.2018.
//

namespace Berserk
{

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
            if (list.getCurrent().mKey == key)
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
