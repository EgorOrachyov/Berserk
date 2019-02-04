//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_HASHMAP_H
#define BERSERK_HASHMAP_H

#include "Public/Misc/Crc32.h"
#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Memory/Allocator.h"
#include "Public/Memory/PoolAllocator.h"
#include "Public/Containers/SharedList.h"


namespace Berserk
{

    template <typename K, typename V>
    class HashMap
    {
    private:

        struct Node
        {
        public:

            void set(const K& key, const V& value)
            {
                memcpy(&mKey,   &key,   sizeof(K));
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

    public:

        /** Type of hashing method for the key */
        typedef uint32 (*Hashing)(const K& key);

        /** Does not support small ranges for hashing efficiency */
        static const uint32 MIN_HASH_RANGE = 64;

        /** Default hashing method via casting key to the array of chars */
        static uint32 defaultHashing(const K& key)
        {
            auto ptr = (char*)key;
            auto hash = Crc32::hash(ptr, sizeof(K));

            return hash;
        }

    public:

        explicit HashMap(Hashing hashing = nullptr, uint32 range = MIN_HASH_RANGE);

        ~HashMap();

        void remove(const K& key);

        void empty();

        void add(const K& key, const V& value);

        V*   operator [] (const K& key) const;

        V*   iterate();

        V*   next();

        uint32  getSize() const;

        uint32  getRange() const;

        float32 getLoadFactor() const;

    private:

        uint32  mRange;
        uint32  mSize;
        Node*   mIterator;
        Hashing mHashing;
        SharedList<Node>* mList;
        PoolAllocator     mPool;

    };

    template <typename K, typename V>
    HashMap<K,V>::HashMap(Hashing hashing, uint32 range) : mPool(SharedList<Node>::getNodeSize(), PoolAllocator::MIN_CHUNK_COUNT)
    {
        FAIL(mRange >= MIN_HASH_RANGE, "Range must be more than %u", MIN_HASH_RANGE);

        if (hashing) mHashing = hashing;
        else mHashing = defaultHashing;

        mRange = range;
        mSize = 0;
        mIterator = nullptr;

        mList = (SharedList<Node>*) Allocator::getSingleton().memoryCAllocate(mRange, sizeof(SharedList<Node>));

        for (uint32 i = 0; i < mRange; i++)
        {
            mList[i].Init(mPool);
        }
    }


} // namespace Berserk

#endif //BERSERK_HASHMAP_H