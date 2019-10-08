//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_ICOMMANDQUEUE_H
#define BERSERK_ICOMMANDQUEUE_H

#include <HAL/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Pointer/TSharedPtr.h>
namespace Berserk
{

    /**
     * Command queue buffer for thread safe systems
     * communication via sending and receiving commands
     * @tparam Cmd Type of commands to send
     */
    template <typename Cmd>
    class ENGINE_API ICommandQueue
    {
    public:

        typedef TSharedPtr<Cmd> CmdRef;

    public:

        virtual ~ICommandQueue() = default;

        virtual void push(const CmdRef& command) = 0;

    };

} // namespace Berserk

#endif //BERSERK_ICOMMANDQUEUE_H