/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_COMMANDQUEUE_HPP
#define BERSERK_COMMANDQUEUE_HPP

#include <BerserkCore/Templates/MessageQueue.hpp>

namespace Berserk {

    /**
     * Command queue which allows to enqueue callable commands and call the later with TArgs arguments.
     * @tparam TArgs Type of arguments, passed to callable commands.
     */
    template<typename ... TArgs>
    class CommandQueue {
    public:

        class Command {
        public:
            virtual ~Command() = default;
            virtual void Execute(TArgs&& ... args) const = 0;
        };

        using A = Allocator;
        using QueueType = MessageQueue<Command, A>;
        static const size_t BUFFER_SIZE = QueueType::BUFFER_SIZE;
        static const size_t BUFFER_ALIGNMENT = QueueType::BUFFER_ALIGNMENT;

        explicit CommandQueue(size_t bufferSize = BUFFER_SIZE, size_t alignment = BUFFER_ALIGNMENT, A&& alloc = A())
                : mQueue(bufferSize, alignment, std::move(alloc)) {

        }

        /**
         * Enqueue callable object into
         *
         * @tparam Callable Object type, which cann be called by operator() with TArgs arguments
         * @param callable Object instance to call
         */
        template<typename Callable>
        void Enqueue(Callable&& callable) {
            class CallableCommand: public Command {
            public:
                explicit CallableCommand(Callable&& callable) : mCallable(std::forward<Callable>(callable)) {}
                ~CallableCommand() override = default;
                void Execute(TArgs&& ... args) const override { mCallable(std::forward<TArgs>(args)...); }
            private:
                Callable mCallable;
            };

            CallableCommand cmd{std::forward<Callable>(callable)};
            mQueue.Move(std::move(cmd));
        }

        /** Execute all commands in the queue */
        void Execute(TArgs&& ... args) {
            mQueue.ForEach([&](const Command& cmd) {
                cmd.Execute(std::forward<TArgs>(args)...);
            });
        }

        void Clear() {
            mQueue.Clear();
        }

        size_t GetSize() const {
            return mQueue.GetSize();
        }

        bool IsEmpty() const {
            return mQueue.IsEmpty();
        }

        bool IsNotEmpty() const {
            return mQueue.IsNotEmpty();
        }

    private:
        QueueType mQueue;
    };

    /** Default queue with simple callback functions */
    using CallbackQueue = CommandQueue<>;

}

#endif // BERSERK_COMMANDQUEUE_HPP