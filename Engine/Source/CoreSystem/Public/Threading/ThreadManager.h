//
// Created by Egor Orachyov on 2019-07-04.
//

#ifndef BERSERK_THREADMANAGER_H
#define BERSERK_THREADMANAGER_H

#include <Threading/IThreadManager.h>
#include <Containers/TArray.h>

namespace Berserk
{

    /** Default engine thread manager */
    class ThreadManager : public IThreadManager
    {
    public:

        /**
         * Specify the number of the threads to limit created threads number
         * @param maxThreadsLimit Max number of possibly created threads
         * @param allocator Allocator for internal usage
         */
        explicit ThreadManager(uint32 maxThreadsLimit = DEFAULT_THREADS_LIMIT, IAllocator& allocator = Allocator::get());

        ~ThreadManager() override;

        /** @copydoc IThreadManager::createThread() */
        Thread *createThread(const char *name, const TSharedPtr<IRunnable> &runnable, bool daemon) override;

        /** @copydoc IThreadManager::getThread() */
        Thread *getThread(uint32 threadId) const override;

        /** @copydoc IThreadManager::getTotalNumOfThreads() */
        uint32 getTotalNumOfThreads() const override;

    private:

        /** Allows only 4 threads to be created */
        static const uint32 DEFAULT_THREADS_LIMIT = 4;

        /** All the created threads */
        TArray<Thread> mThreads;

        /** Multi-threaded access */
        mutable Mutex mMutex;

        /** Upper bound */
        uint32 mMaxThreadsToCreate;

    };

} // namespace Berserk

#endif //BERSERK_THREADMANAGER_H