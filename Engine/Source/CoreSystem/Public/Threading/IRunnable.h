//
// Created by Egor Orachyov on 2019-07-04.
//

#ifndef BERSERK_IRUNNABLE_H
#define BERSERK_IRUNNABLE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * An object to run on an arbitrary thread. Primary used for internal engine
     * usage. Used mapping - one thread - one runnable class.
     */
    class CORE_API IRunnable
    {
    public:

        virtual ~IRunnable() = default;

        /**
         * Actually run this task on the thread
         * @return Return code of the execution. 0 - Success, otherwise error.
         */
        virtual uint32 run() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRUNNABLE_H