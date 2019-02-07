//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_THREADPOOL_H
#define BERSERK_THREADPOOL_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Buffers.h"
#include "Public/Misc/Platform.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Memory/LinearAllocator.h"
#include "Public/Threading/Thread.h"
#include "Public/Threading/Future.h"
#include "Public/Threading/IRunnable.h"
#include "Public/Threading/ConcurrentLinkedQueue.h"

namespace Berserk
{

    /**
     * Frame based thread pool for executing task in one frame specialization.
     * Has internal buffer for allocating memory for future features and runnables with
     * different size to properly submit task.
     *
     * That pool should be refreshed each frame to free internal buffer and prepare it
     * for next frame allocations.
     *
     * @todo: Remove internal buffer
     *        Add tagged heap allocator
     */
    class CORE_API ThreadPool
    {
    public:

        /** Initial number of tasks to preallocate in the queue */
        static const uint32 INITIAL_TASKS_COUNT = Buffers::SIZE_128;

        /** Number of thread to employ in the pool - hardcoded */
        static const uint32 THREADS_COUNT = TARGET_PHYSICAL_CORES_COUNT;

        /** Buffer size for allocating memory for runnable objects in one frame [10 MiB] */
        static const uint32 ALLOCATION_BUFFER_SIZE = Buffers::MiB * 10;

    public:

        /**
         * Creates pool, initializes threads
         * @param size Initial queue size for tasks
         */
        ThreadPool(uint32 size = INITIAL_TASKS_COUNT);

        ~ThreadPool();

    public:  // concurrent

        /** @return Submit new task in pool and get pointer to its future */
        Future* submit(IRunnable* runnable);

        /** @return Pointer to allocated memory region of chosen size for runnable */
        IRunnable* alloc(uint32 size);

        /** Wait until all submitted tasks will be finished */
        void join();

        /** Prepare for the next frame */
        void refresh();

        /** Finish all tasks and close pool */
        void shutdown();

        /** Close pool immediately without waiting for finishing submitted task */
        void terminate();

    private:

        struct TaskInfo
        {
            TaskInfo() : runnable(nullptr), future(nullptr) { }

            TaskInfo(IRunnable* runnable, Future* future)
                    : runnable(runnable), future(future) { }

            IRunnable* runnable;
            Future* future;
        };

        typedef ConcurrentLinkedQueue<TaskInfo> TaskQueue;

        class Worker : public IRunnable
        {
        public:

            Worker()
                : mQueue(nullptr) , mShutdown(nullptr) { }

            int32 run() override
            {
                while (!(*mShutdown))
                {
                    TaskInfo info;
                    bool notEmpty;

                    mQueue->pop(&info, &notEmpty);

                    if (notEmpty)
                    {
                        auto result = info.runnable->run();

                        info.future->mDone = true;
                        info.future->mResult = result;
                    }
                    else
                    {
                        Thread::yield();
                    }
                }

                return 0;
            }

        public:

            volatile bool* mShutdown;
            TaskQueue* mQueue;

        };

    private:

        std::mutex mMutex;
        TaskQueue mQueue;
        volatile bool mShutdown;
        LinearAllocator mAllocator;
        Thread mThreads[THREADS_COUNT];
        Worker mWorkers[THREADS_COUNT];

    };

} // namespace Berserk

#endif //BERSERK_THREADPOOL_H