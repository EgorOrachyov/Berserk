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

#ifndef BERSERK_MESSAGEQUEUE_HPP
#define BERSERK_MESSAGEQUEUE_HPP

#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Memory/LinearAllocator.hpp>
#include <type_traits>

namespace Berserk {

    /**
     * Message queue which allows to store messages with generic T interface
     *
     * @tparam T Base type of stored message
     * @tparam A Allocator used for messages allocation
     */
    template<typename T, typename A = Allocator>
    class MessageQueue {
    public:
        static const size_t BUFFER_SIZE = Memory::KiB * 10;
        static const size_t BUFFER_ALIGNMENT = Memory::ALIGNMENT;
        static_assert(std::has_virtual_destructor<T>::value, "T must have virtual destructor to be stored in the queue");

        explicit MessageQueue(size_t bufferSize = BUFFER_SIZE, size_t  alignment = BUFFER_ALIGNMENT, A&& alloc = A())
            : mMessageAllocator(bufferSize, alignment, std::move(alloc)) {

        }

        ~MessageQueue() {
            Clear();
        }

        template<typename M>
        void Move(M&& message) {
            static_assert(std::is_base_of<T, M>::value, "T is not a base class of M");
            auto messageSize = sizeof(M);
            auto* memory = mMessageAllocator.Allocate(messageSize);

            assert(memory);

            auto* entry = new (memory) M(std::forward<M>(message));
            mMessages.Add(entry);
        }

        /** Iterate over queue messages */
        template<typename Function>
        void ForEach(Function&& function) const {
            for (const T* message: mMessages) {
                function(*message);
            }
        }

        /** Clear all enqueued messages */
        void Clear() {
            for (auto message: mMessages)
                message->~T();

            mMessageAllocator.Reset();
            mMessages.Clear();
        }

        /** @return Messages count in the queue */
        size_t GetSize() const {
            return mMessages.GetSize();
        }

        /** @return True if no messages in the queue */
        bool IsEmpty() const {
            return mMessages.IsEmpty();
        }

        /** @return True if has some messages in the queue */
        bool IsNotEmpty() const {
            return mMessages.IsNotEmpty();
        }

    private:
        LinearAllocator<A> mMessageAllocator;
        Array<T*> mMessages;
    };

}


#endif //BERSERK_MESSAGEQUEUE_HPP
