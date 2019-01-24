//
// Created by Egor Orachyov on 12.07.2018.
//

#ifndef BERSERKENGINE_CONFIGTABLE_H
#define BERSERKENGINE_CONFIGTABLE_H

#include "ConfigCommon.h"
#include "Strings/CStaticString.h"
#include "Containers/SharedList.h"
#include "Containers/HashTable.h"
#include "Memory/PoolAllocator.h"

namespace Berserk
{

    /**
     * @addtogroup Config
     *
     * @{
     */

    /**
     * Config table to store pairs of CStaticString key and void* pointer data (max
     * size of data equal to size of CStaticString).
     *
     * @note Uses for storing configuration values to setup application after initializing
     */
    class ConfigTable
    {
    private:

        /**
         * Node to store keys and data in shared lists
         */
        struct Node
        {
        public:

            Node(const CStaticString& key, void* data, ParamType type)
            {
                mType = type;
                mKey = key;
                mData = data;
            }

            ~Node()
            {
                mKey.~CStaticString();
            }

            Node operator = (const Node& node)
            {
                mType = node.mType;
                mKey = node.mKey;
                mData = node.mData;

                return *this;
            }

            const bool operator == (const Node& node)
            {
                return (this->mKey == node.mKey);
            }

            ParamType mType;
            CStaticString mKey;
            void* mData;
        };

    public:

        ConfigTable();
        ~ConfigTable();

        /**
         * Initialize hash table with % hash range
         * @param range Default range = 64
         *
         * @note Default hash function CRC32
         */
        void init(UINT32 range = 64);

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
         * @param key CStaticString value
         * @param data Data to store in table
         */
        void add(const CStaticString& key, void* data, ParamType type);

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        void* get(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        INT16 getInt16(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        UINT16 getUInt16(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        INT32 getInt32(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        UINT32 getUInt32(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        FLOAT32 getFloat32(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        FLOAT64 getFloat64(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        CHAR* getChar(const CStaticString& key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        void* get(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        INT16 getInt16(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        UINT16 getUInt16(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        INT32 getInt32(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        UINT32 getUInt32(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        FLOAT32 getFloat32(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        FLOAT64 getFloat64(const CHAR* key) const;

        /**
         * Get element by key
         * @param key Element key
         * @return Element data
         */
        CHAR* getChar(const CHAR* key) const;

        /**
         * Get type of value with name key
         * @param key Param name
         * @return type
         */
        ParamType getType(const CStaticString& key) const;

        /**
         * Get type of value with name key
         * @param key Param name
         * @return type
         */
        ParamType getType(const CHAR* key) const;

        /**
         * Get memory for cfg param data
         * @return
         */
        void* alloc();

        /**
         * Is table was successfully initialized
         * @return true if initalized
         */
        bool isInitialized() const;

        /**
         * @param key To find element
         * @return true if table stores element with this key
         */
        bool contains(const CStaticString& key);

        /**
         * @param key To find element
         * @return true if table stores element with this key
         */
        bool contains(const CHAR* key);

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
        PoolAllocator mPool;                // Buffer for hash table pairs key <-> value
        PoolAllocator mData;                // Buffer for values' data
        SharedList<Node>* mLists;           // Lists for handling hash->data

    };

    /**
     * @}
     */

} // namespace Berserk

#endif //BERSERKENGINE_CONFIGTABLE_H