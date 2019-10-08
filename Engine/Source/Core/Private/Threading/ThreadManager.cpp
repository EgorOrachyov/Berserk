//
// Created by Egor Orachyov on 2019-07-04.
//

#include "Threading/ThreadManager.h"
#include <Misc/ExceptionMacros.h>

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

    Thread& ThreadManager::createThread(String name, TSharedPtr <IRunnable> runnable, bool daemon)
    {
        CriticalSection section(mMutex);

        uint32 size = mThreads.getSize();

        if (size == mMaxThreadsToCreate)
        {
            engine_exception("ThreadManager: cannot create another thread [name: %s]", name.get());
        }

        return mThreads.emplace(runnable, size, name, daemon);
    }

    Thread& ThreadManager::findThread(uint32 threadId) const
    {
        Thread* thread = findThreadPrt(threadId);

        if (thread == nullptr)
        {
            engine_exception("ThreadManager: cannot find thread [id: %u]", threadId);
        }

        return *thread;
    }

    Thread *ThreadManager::findThreadPrt(uint32 threadId) const
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