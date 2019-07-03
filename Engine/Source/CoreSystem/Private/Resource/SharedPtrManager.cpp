//
// Created by Egor Orachyov on 25.06.2019.
//

#include "Resource/SharedPtrManager.h"
#include <Misc/AssertDev.h>
#include <Misc/Compilation.h>

namespace Berserk
{

    SharedPtrManager::SharedPtrManager()
        : mMemoryPool(sizeof(SharedPtrInfo), DEFAULT_EXPANDING_COUNT)
    {
        mDefaultEmpty = new (mMemoryPool.allocate(0)) SharedPtrInfo(nullptr);
        mDefaultEmpty->incReference();
        mPtrUsage += 1;
        mTotalPtrCreated += 1;
    }

    SharedPtrManager::~SharedPtrManager()
    {
        deleteNode(nullptr, [](void*a, IAllocator* b){}, mDefaultEmpty);

        /** All ptr should be destroyed */
        assertion_dev_msg(mPtrUsage == 0, "SharedPtrManager: [usage: %u] [total created: %u] [total destroyed: %u]",
                          mPtrUsage, mTotalPtrCreated, mTotalPtrDestroyed);

#ifdef DEBUG
        printf("SharedPtrManager: [usage: %u] [total created: %u] [total destroyed: %u]\n",
               mPtrUsage, mTotalPtrCreated, mTotalPtrDestroyed);
#endif
    }

    SharedPtrManager::SharedPtrInfo* SharedPtrManager::emptyNode()
    {
        CriticalSection section(mMutex);
        mDefaultEmpty->incReference();
        return mDefaultEmpty;
    }

    SharedPtrManager::SharedPtrInfo * SharedPtrManager::createNode(IAllocator *allocator)
    {
        CriticalSection section(mMutex);
        SharedPtrInfo* node = new (mMemoryPool.allocate(0)) SharedPtrInfo(allocator);
        node->incReference();
        mPtrUsage += 1;
        mTotalPtrCreated += 1;
        return node;
    }

    void SharedPtrManager::incReference(SharedPtrInfo *node)
    {
        CriticalSection section(mMutex);
        node->incReference();
    }

    void SharedPtrManager::deleteNode(void *source, DeleteSource fun, SharedPtrInfo *node)
    {
        CriticalSection section(mMutex);
        node->decReference();
        if (!node->hasReferences())
        {
            fun(source, node->allocator());
            mMemoryPool.free(node);
            mPtrUsage -= 1;
            mTotalPtrDestroyed += 1;
        }
    }

    SharedPtrManager& SharedPtrManager::get()
    {
        static SharedPtrManager manager;
        return manager;
    }

} // namespace Berserk