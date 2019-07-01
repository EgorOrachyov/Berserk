//
// Created by Egor Orachyov on 25.06.2019.
//

#include "Strings/StringManager.h"
#include <Misc/Compilation.h>

namespace Berserk
{

    StringManager::StringManager(uint32 charTypeSize) : mMemoryPool(EStringTypes::Total)
    {
        uint32 nodesCount[EStringTypes::Total] = { 32, 16, 8, 4 , 2 };
        uint32 nodesSizes[EStringTypes::Total] =
        {
                StringInfo::nodeSize(32  * charTypeSize),
                StringInfo::nodeSize(64  * charTypeSize),
                StringInfo::nodeSize(128 * charTypeSize),
                StringInfo::nodeSize(256 * charTypeSize),
                StringInfo::nodeSize(512 * charTypeSize),
        };

        /** Initialize our pools in the array */
        for (uint32 i = 0; i < EStringTypes::Total; i++)
        {
            mMemoryPool.emplace(nodesSizes[i], nodesCount[i]);
        }

        mDefaultEmptyString = new (mMemoryPool.get(EStringTypes::Size_32).allocate(0)) StringInfo(32);
        mDefaultEmptyString->incReference();
        mStringsUsage += 1;
        mTotalStringsCreated += 1;
    }

    StringManager::~StringManager()
    {
        /** Allocated by this manager string */
        deleteNode(mDefaultEmptyString);

#ifdef DEBUG
        printf("StringManager: [usage: %u] [total created: %u] [total destroyed: %u] \n",
               mStringsUsage, mTotalStringsCreated, mTotalStringsDestroyed);
#endif

        /** All strings should be destroyed */
        assertion_dev_msg(mStringsUsage == 0, "StringManager: [usage: %u] [total created: %u] [total destroyed: %u]",
                          mStringsUsage, mTotalStringsCreated, mTotalStringsDestroyed);
    }

    StringManager::StringInfo* StringManager::emptyNode()
    {
        CriticalSection section(mMutex);
        mDefaultEmptyString->incReference();
        return mDefaultEmptyString;
    }

    StringManager::StringInfo* StringManager::createNode(uint32 size)
    {
        CriticalSection section(mMutex);
        EStringTypes id = bestFit(size);
        PoolAllocator& pool = mMemoryPool.get(id);
        StringInfo* node = new (pool.allocate(0)) StringInfo(stringSize(id));
        node->incReference();
        mStringsUsage += 1;
        mTotalStringsCreated += 1;
        return node;
    }

    void StringManager::incReferences(StringInfo* node)
    {
        CriticalSection section(mMutex);
        node->incReference();
    }

    void StringManager::deleteNode(StringInfo* node)
    {
        CriticalSection section(mMutex);
        node->decReference();
        if (!node->hasReferences())
        {
            EStringTypes id = bestFit(node->size());
            PoolAllocator& pool = mMemoryPool.get(id);
            pool.free(node);
            mStringsUsage -= 1;
            mTotalStringsDestroyed += 1;
        }
    }

    StringManager::EStringTypes StringManager::bestFit(uint32 size)
    {
        if (size <= 32)
        {
            return EStringTypes::Size_32;
        }
        else if (size <= 64)
        {
            return EStringTypes::Size_64;
        }
        else if (size <= 128)
        {
            return EStringTypes::Size_128;
        }
        else if (size <= 256)
        {
            return EStringTypes::Size_256;
        }
        else if (size <= 512)
        {
            return EStringTypes::Size_512;
        }
        else
        {
            assertion_dev_msg(false, "StringManager: Cannot allocate string [size: %u]", size);
        }

        return EStringTypes::Total;
    }

    uint32 StringManager::stringSize(EStringTypes type)
    {
        int32 base = 32;
        base = base << type;
        return (uint32) base;
    }

    StringManager& StringManager::getCharStringManager()
    {
        static StringManager manager(sizeof(char));
        return manager;
    }

    StringManager& StringManager::getWCharStringManager()
    {
        static StringManager manager(sizeof(wchar_t));
        return manager;
    }

} // namespace Berserk