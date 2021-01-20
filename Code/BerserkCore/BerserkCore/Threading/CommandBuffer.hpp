/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_COMMANDBUFFER_HPP
#define BERSERK_COMMANDBUFFER_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Misc/Ref.hpp>
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkCore/Memory/LinearAllocator.hpp>

namespace Berserk {

    /**
     * @brief Command Buffer
     *
     * Command buffer is capable of enqueuing executable/callable commands for
     * later ordered execution. Actual commands are allocated and stored inside
     * buffer, for fast and efficient memory allocations linear allocator is
     * used with fixed and predefined memory buffer size.
     */
    class CommandBuffer {
    public:

        class Command {
        public:
            virtual ~Command() = default;
            virtual void Execute() = 0;
        };

        /**
         * Creates command buffer with fixed internal buffer for commands allocation.
         * @param bufferSize Size in bytes of the internal memory buffer for actual commands allocation.
         */
        explicit CommandBuffer(size_t bufferSize = 10 * Platform::Memory::KiB);
        ~CommandBuffer();

        /**
         * Enqueues new executable command into command buffer.
         *
         * @tparam Callable Type of the callable command, which must implement operator().
         * @param callable Actual command to be enqueued.
         */
        template<typename Callable>
        void Enqueue(Callable&& callable) {

            class CallableCommand: public Command {
            public:
                explicit CallableCommand(Callable&& callable) : mCallable(std::forward<Callable>(callable)) {}
                ~CallableCommand() override = default;

                void Execute() override {
                    // Execute actual command
                    mCallable();
                }

            private:
                Callable mCallable;
            };

            auto commandSize = sizeof(CallableCommand);
            auto memory = mCommandsAlloc.Allocate(commandSize);

            auto command = new (memory) CallableCommand(std::forward<Callable>(callable));
            mCommands.Add(command);
        }

        /** Release commands and empty internal buffers. */
        void Clear();

        const Array<Command*> &GetCommands() const {
            return mCommands;
        }

        size_t GetCommandsCount() const {
            return mCommands.GetSize();
        }

        bool IsEmpty() const {
            return mCommands.IsEmpty();
        }

        bool IsNotEmpty() const {
            return mCommands.IsNotEmpty();
        }

    private:
        Array<Command*> mCommands;
        LinearAllocator<> mCommandsAlloc;
    };

}

#endif //BERSERK_COMMANDBUFFER_HPP