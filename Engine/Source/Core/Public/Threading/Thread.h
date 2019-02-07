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

    class Thread
    {
    public:

        /**
         * Does not actually initializes and starts thread.
         * Only creates this wrapper class
         */
        Thread();

        ~Thread() = default;

        /**
         * Creates, initializes and runs this thread with task IRunnable
         * @param runnable Submitted task for running on that threads
         */
        void run(IRunnable* runnable);

        /**
         * Waits until `that` thread object finish his task
         */
        void join();

        /**
         * Provides a hint to the implementation to reschedule
         * the execution of threads, allowing other threads to run.
         */
        static void yield();

    public:

        const IRunnable* runnable() const { return mRunnable; }

    private:

        static void thread_runner(void* runnable, void* thread)
        {
            int32 result = ((IRunnable*)runnable)->run();
            FAIL(result == EXIT_SUCCESS, "Thread: exit code %u for IRunnable: %p | Thread: %p", result, runnable, thread);
        }

    private:

        IRunnable*  mRunnable;
        std::thread mThread;

    };

    Thread::Thread() : mRunnable(nullptr)
    {

    }

    void Thread::run(IRunnable *runnable)
    {
        mRunnable = runnable;
        mThread = std::thread(&thread_runner, runnable, this);
    }

    void Thread::join()
    {
        if (mRunnable && mThread.joinable())
        { mThread.join(); }
    }

    void Thread::yield()
    {
        std::this_thread::yield();
    }

} // namespace Berserk

#endif //BERSERK_THREAD_H