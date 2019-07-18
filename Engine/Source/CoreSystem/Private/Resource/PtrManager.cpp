//
// Created by Egor Orachyov on 25.06.2019.
//

#include "Resource/PtrManager.h"
#include <Misc/AssertDev.h>
#include <Misc/Compilation.h>

namespace Berserk
{

    PtrManager::PtrManager()
        : mMemoryPool(sizeof(SharedPtrInfo), DEFAULT_EXPANDING_COUNT)
    {

    }

    PtrManager::~PtrManager()
    {

        /** All ptr should be destroyed */
        assertion_dev_msg(mPtrUsage == 0, "PtrManager: [usage: %u] [total created: %u] [total destroyed: %u]",
                          mPtrUsage, mTotalPtrCreated, mTotalPtrDestroyed);

#ifdef DEBUG
        printf("PtrManager: [usage: %u] [total created: %u] [total destroyed: %u]\n",
               mPtrUsage, mTotalPtrCreated, mTotalPtrDestroyed);
#endif
    }

    PtrManager::SharedPtrInfo * PtrManager::createNode(IAllocator *allocator)
    {
        CriticalSection section(mMutex);

        auto node = new (mMemoryPool.allocate(0)) SharedPtrInfo(allocator);
        node->incRefShared();
        mPtrUsage += 1;
        mTotalPtrCreated += 1;

        return node;
    }

    void PtrManager::deleteNode_CallBySharedPtr(void *source, DeleteSource fun, SharedPtrInfo *node)
    {
        /** Free object*/
        fun(source, node->allocator());

        /** If no weak refs - free shared info */
        if (!node->hasWeakRefs())
        {
            CriticalSection section(mMutex);
            mMemoryPool.free(node);
            mPtrUsage -= 1;
            mTotalPtrDestroyed += 1;
        }
    }

    void PtrManager::deleteNode_CallByWeakPtr(PtrManager::SharedPtrInfo *node)
    {
        CriticalSection section(mMutex);

        /** Free shared info */
        mMemoryPool.free(node);
        mPtrUsage -= 1;
        mTotalPtrDestroyed += 1;
    }

    PtrManager& PtrManager::get()
    {
        static PtrManager manager;
        return manager;
    }

} // namespace Berserk