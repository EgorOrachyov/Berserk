//
// Created by Egor Orachyov on 2019-07-04.
//

#include "Threading/ThreadManager.h"

namespace Berserk
{
    ThreadManager::ThreadManager(Berserk::uint32 maxThreadsLimit, Berserk::IAllocator &allocator)
        : mMaxThreadsToCreate(maxThreadsLimit), mThreads(maxThreadsLimit, allocator)
    {

    }

    ThreadManager::~ThreadManager()
    {
        CriticalSection section(mMutex);

        for (auto thread = mThreads.begin(); thread != nullptr; thread = mThreads.next())
        {
            if (thread->isJoinable())
            {
                thread->join();
            }
        }
    }

    Thread *ThreadManager::createThread(const char *name, const TSharedPtr<IRunnable> &runnable, bool daemon)
    {
        CriticalSection section(mMutex);

        uint32 size = mThreads.getSize();

        if (size == mMaxThreadsToCreate)
        {
            return nullptr;
        }

        mThreads.emplace(runnable, size, name, daemon);
        return &mThreads.get(size);
    }

    Thread *ThreadManager::getThread(uint32 threadId) const
    {
        CriticalSection section(mMutex);

        if (threadId >= mThreads.getSize())
        {
            return nullptr;
        }

        return &mThreads.get(threadId);
    }

    uint32 ThreadManager::getTotalNumOfThreads() const
    {
        CriticalSection section(mMutex);
        uint32 size = mThreads.getSize();
        return size;
    }

} // namespace Berserk