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
     * Special kind thread pool
     */
    class CORE_API ThreadPool
    {
    public:

        /** Number of thread to employ in the pool - hardcoded */
        static const uint32 THREADS_COUNT = TARGET_PHYSICAL_CORES_COUNT;

        /** Buffer size for allocating memory for runnable objects in one frame [10 MiB] */
        static const uint32 ALLOCATION_BUFFER_SIZE = Buffers::MiB * 10;

    public:

        /** Creates pool, initializes threads */
        ThreadPool();

        ~ThreadPool();

    public:  // concurrent

        /** @return Submit new task in pool and get pointer to its future */
        Future* submit(IRunnable* runnable);

        /** Wait until all submitted tasks will be finished */
        void join();

        /** Prepare for the next frame */
        void refresh();

        /** Finish all tasks and close pool */
        void shutdown();

        /** Close pool immediately without waiting for finishing submitted task */
        void terminate();

    private:

        Thread mThreads[THREADS_COUNT];
        LinearAllocator mAllocator;

    };

} // namespace Berserk

#endif //BERSERK_THREADPOOL_H