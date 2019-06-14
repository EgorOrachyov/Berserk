//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Logging/LogMacros.h"
#include "Threading/ThreadPool.h"

namespace Berserk
{

    ThreadPool::ThreadPool(uint32 size)
            : mQueue(size),
              mShutdown(false)
    {
        for (uint32 i = 0; i < THREADS_COUNT; i++)
        {
            mWorker.mQueue = &mQueue;
            mWorker.mShutdown = &mShutdown;
            mThreads[i].run(&mWorker);
        }
    }

    ThreadPool::~ThreadPool()
    {
        //PUSH("Thread Pool: delete %p", this);

        if (!mShutdown)
        {
            shutdown();
        }
    }

    void ThreadPool::submit(IRunnable *runnable, Future* future)
    {
        if (future)
        {
            future->mDone = false;
            future->mResult = 0;
            future->mRunnable = runnable;
        }

        mQueue.push(TaskInfo(runnable, future));
    }

    void ThreadPool::join()
    {
        while (true)
        {
            auto size = mQueue.getSize();
            if (size == 0) break;
            else Thread::yield();
        }
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