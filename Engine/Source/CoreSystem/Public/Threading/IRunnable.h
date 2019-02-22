//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_IRUNNABLE_H
#define BERSERK_IRUNNABLE_H

#include "Misc/Types.h"

namespace Berserk
{

    /**
     * Interface for an arbitrary work
     * (Primary for multi-thread execution)
     */
    class IRunnable
    {
    public:

        /**
         * Virtual function for any kind of work, which could be submitted for
         * thread executor or thread pool
         *
         * @return Return code [0] if the execution was successfully completed
         *         or any another value if there was any kind of error.
         *         If code != 0, then the thread executor will delete thread
         *         and raise and FATAL error
         */
        virtual int32 run() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRUNNABLE_H