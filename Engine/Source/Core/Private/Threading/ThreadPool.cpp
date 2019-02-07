//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Public/Logging/LogMacros.h"
#include "Public/Threading/ThreadPool.h"

namespace Berserk
{

    ThreadPool::ThreadPool(uint32 size)
            : mAllocator(ALLOCATION_BUFFER_SIZE),
              mQueue(size),
              mShutdown(false)
    {
        for (uint32 i = 0; i < THREADS_COUNT; i++)
        {
            mWorkers[i].mQueue = &mQueue;
            mWorkers[i].mShutdown = &mShutdown;
            mThreads[i].run(&mWorkers[i]);
        }
    }

    ThreadPool::~ThreadPool()
    {
        PUSH("Thread Pool: delete %p", this);

        if (!mShutdown)
        {
            shutdown();
        }
    }

    Future* ThreadPool::submit(IRunnable *runnable)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        auto future = (Future*) mAllocator.alloc(sizeof(Future));

        future->mDone = false;
        future->mResult = 0;
        future->mRunnable = runnable;

        mQueue.push(TaskInfo(runnable, future));

        return future;
    }

    IRunnable* ThreadPool::alloc(uint32 size)
    {
        return (IRunnable*) mAllocator.alloc(size);
    }

    void ThreadPool::refresh()
    {
        mAllocator.free();
    }

    void ThreadPool::shutdown()
    {
        mShutdown = true;

        for(uint32 i = 0; i < THREADS_COUNT; i++)
        {
            mThreads[i].join();
        }
    }

    void ThreadPool::terminate()
    {
        for(uint32 i = 0; i < THREADS_COUNT; i++)
        {
            mThreads[i].daemon(true);
        }
    }



} // namespace Berserk