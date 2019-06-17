//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_THREADPOOL_H
#define BERSERK_THREADPOOL_H

#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/Platform.h"
#include "Misc/UsageDescriptors.h"
#include "Memory/Old/LinearAllocator.h"
#include "Threading/Thread.h"
#include "Threading/Future.h"
#include "Threading/IRunnable.h"
#include "Threading/ConcurrentLinkedQueue.h"

namespace Berserk
{

    /**
     * Frame based thread pool for executing task in one frame specialization.
     * Allows to wait until all submitted task are completed to start submitting
     * new ones in the next frame.
     */
    class CORE_API ThreadPool
    {
    public:

        /** Initial number of tasks to preallocate in the queue */
        static const uint32 INITIAL_TASKS_COUNT = Buffers::SIZE_128;

        /** Number of thread to employ in the pool - hardcoded */
        static const uint32 THREADS_COUNT = TARGET_PHYSICAL_CORES_COUNT - 1;

    public:

        /**
         * Creates pool, initializes threads
         * @param size Initial queue size for tasks
         */
        explicit ThreadPool(uint32 size = INITIAL_TASKS_COUNT);

        ~ThreadPool();

    public:

        /**
         * Submit new task in pool with pointer to its future or nullptr
         * @param runnable Pointer to runnable task to run it
         * @param future   Pointer to future to store done flag and result when task is finished
         */
        void submit(IRunnable* runnable, Future* future = nullptr);

        /** Wait until all submitted tasks will be finished */
        void join();

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

                        if (info.future)
                        {
                            info.future->mDone = true;
                            info.future->mResult = result;
                        }
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

        Worker mWorker;                     // Worker instace of
        TaskQueue mQueue;                   //
        volatile bool mShutdown;            //
        Thread mThreads[THREADS_COUNT];     //

    };

} // namespace Berserk

#endif //BERSERK_THREADPOOL_H