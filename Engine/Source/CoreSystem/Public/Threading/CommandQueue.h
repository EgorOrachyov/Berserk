//
// Created by Egor Orachyov on 2019-08-28.
//

#ifndef BERSERK_COMMANDQUEUE_H
#define BERSERK_COMMANDQUEUE_H

#include <Containers/TArray.h>
#include <Threading/AtomicTypes.h>
#include <Misc/Allocatable.h>
#include <Threading/Async.h>

namespace Berserk
{

    /** Single command for the queue */
    class QueueCommand : public Allocatable
    {
    public:


    private:

        Async mAsync;

    };

    /** Single queue stores commands in the buffer */
    typedef TArray<QueueCommand> CommandQueueBuffer;

    class CommandQueue {

    };

} // namespace Berserk




#endif //BERSERK_COMMANDQUEUE_H
