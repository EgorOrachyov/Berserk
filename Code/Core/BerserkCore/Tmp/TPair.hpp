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

#ifndef BERSERK_TPAIR_HPP
#define BERSERK_TPAIR_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    template <typename A, typename B>
    class TPair final {
    public:
        TPair() = default;
        TPair(const A& a, const B& b) : mA(a), mB(b) { }
        TPair(A&& a, B&& b) : mA(std::move(a)), mB(std::move(b)) { }
        TPair(const TPair& other) : mA(other.mA), mB(other.mB)  { }
        TPair(TPair&& other) noexcept : mA(std::move(other.mA)), mB(std::move(other.mB)) { }
        ~TPair() = default;

        TPair& operator=(const TPair& other) {
            this->~TPair();
            new (this) TPair(other);
            return *this;
        }

        TPair& operator=(TPair&& other) noexcept {
            this->~TPair();
            new (this) TPair(std::move(other));
            return *this;
        }

        A& GetFirst() { return mA; }
        B& GetSecond() { return mB; }

        const A& GetFirst() const { return mA; }
        const B& GetSecond() const { return mB; }

    private:
        A mA;
        B mB;
    };

}

#endif //BERSERK_TPAIR_HPP