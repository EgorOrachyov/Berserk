//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_THREAD_H
#define BERSERK_THREAD_H

#include <thread>
#include "Public/Misc/Assert.h"
#include "Public/Threading/IRunnable.h"

namespace Berserk
{

    /**
     * Wrapper for std::thread with support of IRunnable task submitting.
     * The following rules of using could be defined as:
     *
     * Pattern 1            | Pattern 2
     *                      |
     * Thread t;            | Thread t;
     * t.run(task);         | t.run(task);
     * t.join();            | t.daemon(true);
     *
     * Need explicit join   | daemon thread will automatically shutdown
     *                      | when MAIN thread finish executing
     */
    class CORE_API Thread
    {
    public:

        /**
         * Does not actually initializes and starts thread.
         * Only creates this wrapper class
         */
        Thread();

        ~Thread();

        /**
         * Creates, initializes and runs this thread with task IRunnable
         * @param runnable Submitted task for running on that threads
         */
        void run(IRunnable* runnable);

        /** Waits until `that` thread object finish his execution */
        void join();

        /** Set that thread in daemon - independent thread */
        void daemon(bool daemon = true);

        /**
         * Provides a hint to the implementation to reschedule
         * the execution of threads, allowing other threads to run.
         */
        static void yield();

    public:

        /** @return True if thread is daemon */
        bool isDaemon() const { return !mThread.joinable(); }

        /** @return True if can join this thread with its parent */
        bool isJoinable() const { return mThread.joinable(); }

        /** @return Pointer to its runnable or nullptr */
        const IRunnable* runnable() const { return mRunnable; }

        static uint32 numberOfCores();

    private:

        /** Counter used for explicit threads marking in terms of one engine start */
        static std::atomic_int THREAD_COUNTER;

        /** Internal function used for abstraction from IRunnable interface */
        static void thread_runner(void* runnable, void* thread);

    private:

        int32       mId;
        IRunnable*  mRunnable;
        std::thread mThread;

    };

} // namespace Berserk

#endif //BERSERK_THREAD_H