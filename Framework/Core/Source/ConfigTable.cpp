//
// Created by Egor Orachyov on 12.07.2018.
//

#include "Config/ConfigTable.h"

namespace Berserk
{

    
    ConfigTable::ConfigTable()
    {
        mSize = 0;
        mRange = 0;
        mLists = NULL;
    }

     
    ConfigTable::~ConfigTable()
    {
        PUSH("Delete config table %p with shared lists buffer %p\n", this, mLists);
        destroy();
    }

    void ConfigTable::init(UINT32 range)
    {
        if (isInitialized())
        {
            WARNING("Config table has been initialized");
            return;
        }

        mSize = 0;
        mRange = range;

        mLists = (SharedList<Node>*)mem_calloc(range, sizeof(SharedList<Node>));

        mPool.init((UINT16)mLists[0].getSizeOfNode(), 64);
        mData.init((UINT16) sizeof(CStaticString), 32);

        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].init(&mPool);
        }
    }

    void  ConfigTable::destroy()
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
            mData.reset();
        }

        mSize = 0;
        mRange = 0;
        mLists = NULL;
    }

    void  ConfigTable::empty()
    {
        for(UINT32 i = 0; i < mRange; i++)
        {
            mLists[i].empty();
        }
    }

    void ConfigTable::add(const CStaticString& key, void* data, ParamType type)
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

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

        list.add(Node(key,data, type));
        mSize += 1;
    }

    void* ConfigTable::get(const CStaticString& key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return list.getCurrent().mData;
            }
        }

        return NULL;
    }

    INT16 ConfigTable::getInt16(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(INT16*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    UINT16 ConfigTable::getUInt16(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(UINT16*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    INT32 ConfigTable::getInt32(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(INT32*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    UINT32 ConfigTable::getUInt32(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(UINT32*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    FLOAT32 ConfigTable::getFloat32(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(FLOAT32*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    FLOAT64 ConfigTable::getFloat64(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(FLOAT64*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    CHAR* ConfigTable::getChar(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return (CHAR*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    void* ConfigTable::get(const CHAR *key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return list.getCurrent().mData;
            }
        }

        return NULL;
    }

    INT16 ConfigTable::getInt16(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(INT16*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    UINT16 ConfigTable::getUInt16(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(UINT16*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    INT32 ConfigTable::getInt32(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(INT32*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    UINT32 ConfigTable::getUInt32(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(UINT32*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    FLOAT32 ConfigTable::getFloat32(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(FLOAT32*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    FLOAT64 ConfigTable::getFloat64(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return *(FLOAT64*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    CHAR* ConfigTable::getChar(const CHAR* key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return (CHAR*)list.getCurrent().mData;
            }
        }

        return NULL;
    }

    ParamType ConfigTable::getType(const CStaticString &key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return list.getCurrent().mType;
            }
        }

        return ParamType::PT_UNDEFINED;
    }

    ParamType ConfigTable::getType(const CHAR *key) const
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return list.getCurrent().mType;
            }
        }

        return ParamType::PT_UNDEFINED;
    }

    void* ConfigTable::alloc()
    {
        return mData.allocBlock();
    }

    bool  ConfigTable::isInitialized() const
    {
        return (mLists != NULL);
    }

    bool ConfigTable::contains(const CStaticString &key)
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 hash = hashCRC32(key.getChars(), key.getSize()) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return true;
            }
        }

        return false;
    }

    bool ConfigTable::contains(const CHAR *key)
    {
        ASSERT(mRange, "Table is not initialized");

        UINT32 length = 0;
        while (key[length] != '\0') length++;

        UINT32 hash = hashCRC32(key, length) % mRange;

        SharedList<Node>& list = mLists[hash];

        list.iterate(true);
        while (list.iterate())
        {
            if (list.getCurrent().mKey == key)
            {
                return true;
            }
        }

        return false;
    }

    UINT32  ConfigTable::getSize() const
    {
        return mSize;
    }

    UINT32  ConfigTable::getRange() const
    {
        return mRange;
    }

    UINT32  ConfigTable::getCapacity() const
    {
        return mPool.getCapacity();
    }

    FLOAT32  ConfigTable::getLoadFactor() const
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