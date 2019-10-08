//
// Created by Egor Orachyov on 27.06.2019.
//

#ifndef BERSERK_THREAD_H
#define BERSERK_THREAD_H

#include <Strings/String.h>
#include <Threading/IRunnable.h>
#include <Pointer/TSharedPtr.h>
#include <Logging/DebugLogMacros.h>
#include <thread>
#include <Misc/Allocatable.h>

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
    class ENGINE_API Thread : public Allocatable
    {
    public:

        /**
         * Initialize and run task in the new CPU thread
         * @param runnable Task to run
         * @param id Id of the thread (must be unique)
         * @param name Name of the thread for debug purposes
         * @param makeDaemon Set in true to create thread as daemon
         */
        explicit Thread(TSharedPtr<IRunnable> runnable, uint32 id = 0, String name = String(), bool makeDaemon = false);

        Thread(const Thread& other);

        Thread(const Thread&& other) = delete;

        /** Join with current thread */
        void join();

        /** @return Name of the thread */
        const String& getName() const;

        /** @return ID of the thread */
        uint32 getId() const;

        /** @return True if daemon */
        bool isDaemon() const;

        /** @return True if can join */
        bool isJoinable() const;

        /**
         * Provides a hint to the implementation to reschedule
         * the execution of threads, allowing other threads to run.
         */
        static void yield();

        /**
         * @return Number of cores on CPU (if hyper-threading is
         *         available returns number of logical cores)
         */
        static uint32 numberOfCores();

    private:

        /** Private runner of the thread to support exceptions logging */
        static void _threadRunner(IRunnable *runnable, Thread *thread);

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