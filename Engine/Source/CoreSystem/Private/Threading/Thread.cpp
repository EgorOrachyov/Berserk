//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Threading/Old/Thread.h"

namespace Berserk
{

    Thread::Thread() : mRunnable(nullptr)
    {
        /* Thread does not start and acquire OS resource */

        mId = THREAD_COUNTER.load();
        THREAD_COUNTER.operator+=(1);
    }

    Thread::~Thread()
    {
        //PUSH("Thread: delete IRunnable: %p | Thread: %p | Id: %i", mRunnable, this, mId);
    }

    void Thread::run(IRunnable *runnable)
    {
        mRunnable = runnable;
        mThread = std::thread(&thread_runner, runnable, this);
    }

    void Thread::join()
    {
        if (mRunnable && mThread.joinable()) mThread.join();
    }

    void Thread::daemon(bool daemon)
    {
        if (daemon && mRunnable) mThread.detach();
    }

    void Thread::yield()
    {
        std::this_thread::yield();
    }

    uint32 Thread::numberOfCores()
    {
        return std::thread::hardware_concurrency();
    }

    void Thread::thread_runner(void *runnable, void *thread)
    {
        int32 result = ((IRunnable*)runnable)->run();
        FAIL(result == EXIT_SUCCESS, "Thread: exit code %u for IRunnable: %p | Thread: %p | Id: %i",
             result, runnable, thread, ((Thread*)thread)->mId);
    }

    /** Consider: MAIN Thread should have id 0 */
    std::atomic_int Thread::THREAD_COUNTER(1);

} // namespace Berserk