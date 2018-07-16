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
     * @addtogroup Containers
     *
     * @{
     */

    /**
     *  Hash function used for hashing data in table
     */
    typedef UINT32 (*HashFunc)(const CHAR *buffer, INT32 len);

    /**
     * Hash table for elements with key and data fields. Stores keys and data in the internal
     * buffer. Do not require use additional buffers.
     *
     * @warning Require defined == operator for key
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

    /**
     * @}
     */

} // namespace Berserk

#include "Source/HashTable.cpp"

#endif //BERSERKENGINE_HASHTABLE_H