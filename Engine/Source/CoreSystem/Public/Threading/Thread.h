//
// Created by Egor Orachyov on 27.06.2019.
//

#ifndef BERSERK_THREAD_H
#define BERSERK_THREAD_H

#include <Strings/String.h>
#include <Threading/IRunnable.h>
#include <Resource/TSharedPtr.h>
#include <Logging/DebugLogMacros.h>
#include <thread>

namespace Berserk
{

    /**
     * Thread abstraction which provides functionality to get
     * additional CPU execution thread resource to run an arbitrary tasks.
     *
     * Pattern 1            | Pattern 2
     *                      |
     * Thread t;            | Thread t(daemon = true);
     * t.run(task);         |
     * t.join();            |
     *
     * Need explicit join   | daemon thread will automatically shutdown
     *                      | when MAIN thread finish executing
     *
     */
    class ENGINE_API Thread
    {
    public:

        GENERATE_NEW_DELETE(Thread);

        /**
         * Initialize and run task in the new CPU thread
         * @param runnable Task to run
         * @param id Id of the thread (must be unique)
         * @param name Name of the thread for debug purposes
         * @param makeDaemon Set in true to create thread as daemon
         */
        explicit Thread(const TSharedPtr<IRunnable>& runnable, uint32 id = 0,
               const char* name = "", bool makeDaemon = false)
                : mThreadName(name),
                  mThreadID(id),
                  mRunnable(runnable),
                  mIsDaemon(makeDaemon),
                  mThread(threadRunner, mRunnable.operator->(), this)
        {
            if (mIsDaemon)
            {
                mThread.detach();
                mIsJoinable = mThread.joinable();
            }
        }

        Thread(const Thread& other) : mThread()
        {
            assertion_dev(false);
        }

        Thread(const Thread&& other) = delete;

        /** Join with current thread */
        void join()
        {
            mIsJoinable = mThread.joinable();
            if (mIsJoinable) mThread.join();
        }

        /** @return Name of the thread */
        const char* getName() const
        {
            return mThreadName.get();
        }

        /** @return ID of the thread */
        uint32 getId() const
        {
            return mThreadID;
        }

        /** @return True if daemon */
        bool isDaemon() const
        {
            return mIsDaemon;
        }

        /** @return True if can join */
        bool isJoinable() const
        {
            return mIsJoinable;
        }

        /**
         * Provides a hint to the implementation to reschedule
         * the execution of threads, allowing other threads to run.
         */
        static void yield()
        {
            std::this_thread::yield();
        }

        /**
         * @return Number of cores on CPU (if hyper-threading is
         *         available returns number of logical cores)
         */
        static uint32 numberOfCores()
        {
            return std::thread::hardware_concurrency();
        }

    private:

        /** Private runner of the thread to support exceptions logging */
        static void threadRunner(IRunnable* runnable, Thread* thread)
        {
            uint32 returnCode = 0;

            try
            {
                returnCode = runnable->run();
            }
            catch (Exception& e)
            {
                DEBUG_LOG_ERROR("Thread: [name: %s] [id: %u] [daemon: %i] [joinable: %i]",
                          thread->getName(), thread->getId(), thread->isDaemon(), thread->isJoinable());
                DEBUG_LOG_ERROR("%s", e.what());

                assertion_dev(false);
            }

            assertion_dev(returnCode == 0);
        }

    private:

        /** Name of this thread (for debug purposes) */
        String mThreadName;

        /** Function to execute */
        TSharedPtr<IRunnable> mRunnable;

        /** ID for simple look-up */
        volatile uint32 mThreadID = 0;

        /** If we want to run thread as daemon (kill, when main stop) */
        bool mIsDaemon = false;

        /** Can join this thread with some other thread */
        bool mIsJoinable = true;

        /** Actual thread */
        std::thread mThread;

    };

} // namespace Berserk

#endif //BERSERK_THREAD_H