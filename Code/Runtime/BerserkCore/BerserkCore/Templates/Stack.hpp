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

#ifndef BERSERK_STACK_HPP
#define BERSERK_STACK_HPP

#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    template<typename T, typename A = Allocator>
    class Stack {
    public:
        explicit Stack(A&& alloc = A()) : mBuffer(std::move(alloc)) {}
        Stack(const Stack& other) = default;
        Stack(Stack&& other) noexcept = default;
        ~Stack() = default;

        Stack& operator=(const Stack& other) = default;
        Stack& operator=(Stack&& other) noexcept = default;

        void Push(const T& value) {
            mBuffer.Add(value);
        }

        void Push(T&& value) {
            mBuffer.Emplace(std::move(value));
        }

        bool Pop(T& value) {
            if (mBuffer.IsEmpty())
                return false;

            auto index = GetSize() - 1;
            value = std::move(mBuffer[index]);
            mBuffer.Remove(index);

            return true;
        }

        bool Peek(T& value) {
            if (mBuffer.IsEmpty())
                return false;

            auto index = GetSize() - 1;
            value = mBuffer[index];

            return true;
        }

        T& PeekTop() {
            assert(mBuffer.IsNotEmpty());
            return mBuffer[GetSize() - 1];
        }

        bool IsEmpty() const {
            return mBuffer.IsEmpty();
        }

        bool IsNotEmpty() const {
            return mBuffer.IsNotEmpty();
        }

        uint64 GetSize() const {
            return mBuffer.GetSize();
        }

    private:
        Array<T> mBuffer;
    };

}

#endif //BERSERK_STACK_HPP