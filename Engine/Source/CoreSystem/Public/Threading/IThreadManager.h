//
// Created by Egor Orachyov on 2019-07-04.
//

#ifndef BERSERK_ITHREADMANAGER_H
#define BERSERK_ITHREADMANAGER_H

#include <Threading/Thread.h>

namespace Berserk
{

    /**
     * Manages all the created threads in the engine. Needed to
     * limit number of spawned threads because of the performance.
     *
     * @note Thread-Safe
     */
    class IThreadManager
    {
    public:

        virtual ~IThreadManager() = default;

        /**
         * Creates and runs new thread
         * @param name Name of the thread to create
         * @param runnable Task to immediately run on the thread
         * @param daemon If thread should be daemon
         * @return Pointer to created thread
         */
        virtual Thread* createThread(const char* name, const TSharedPtr<IRunnable> & runnable, bool daemon) = 0;

        /**
         * Returns thread via its id
         * @param threadId Id of the thread to find
         * @return Pointer to the thread or nullptr, if it is not found
         */
        virtual Thread* getThread(uint32 threadId) const = 0;

        /**
         * @return Total number of created threads
         */
        virtual uint32 getTotalNumOfThreads() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_ITHREADMANAGER_H