//
// Created by Egor Orachyov on 2019-08-28.
//

#include <Threading/Thread.h>

namespace Berserk
{

    Thread::Thread(TSharedPtr<IRunnable> runnable, uint32 id, String name, bool makeDaemon)
        : mThreadName(std::move(name)),
          mThreadID(id),
          mRunnable(std::move(runnable)),
          mIsDaemon(makeDaemon),
          mThread(_threadRunner, mRunnable.pointer(), this)
    {
        if (mIsDaemon)
        {
            mThread.detach();
            mIsJoinable = mThread.joinable();
        }
    }

    Thread::Thread(const Thread& other) : mThread()
    {
        assertion_dev(false);
    }


    /** Join with current thread */
    void Thread::join()
    {
        mIsJoinable = mThread.joinable();
        if (mIsJoinable) mThread.join();
    }

    /** @return Name of the thread */
    const String & Thread::getName() const
    {
        return mThreadName;
    }

    /** @return ID of the thread */
    uint32 Thread::getId() const
    {
        return mThreadID;
    }

    /** @return True if daemon */
    bool Thread::isDaemon() const
    {
        return mIsDaemon;
    }

    /** @return True if can join */
    bool Thread::isJoinable() const
    {
        return mIsJoinable;
    }

    void Thread::yield()
    {
        std::this_thread::yield();
    }

    /**
     * @return Number of cores on CPU (if hyper-threading is
     *         available returns number of logical cores)
     */
    uint32 Thread::numberOfCores()
    {
        return std::thread::hardware_concurrency();
    }

    void Thread::_threadRunner(Berserk::IRunnable *runnable, Berserk::Thread *thread)
    {
        uint32 returnCode = 0;

        try
        {
            returnCode = runnable->run();
        }
        catch (Exception& e)
        {
            DEBUG_LOG_ERROR("Thread: [name: %s] [id: %u] [daemon: %i] [joinable: %i]",
                            thread->getName().get(), thread->getId(), thread->isDaemon(), thread->isJoinable());
            DEBUG_LOG_ERROR("%s", e.what());

            assertion_dev(false);
        }

        assertion_dev(returnCode == 0);
    }

} // namespace Berserk
