/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PAIR_HPP
#define BERSERK_PAIR_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Contracts.hpp>

namespace Berserk {

    template<typename A, typename B>
    class Pair {
    public:
        using First = A;
        using Second = B;

        Pair() = default;
        Pair(const A& a, const B& b) : mA(a), mB(b) { }
        Pair(A&& a, B&& b) : mA(std::move(a)), mB(std::move(b)) { }
        Pair(const Pair& other) : mA(other.mA), mB(other.mB)  { }
        Pair(Pair&& other) noexcept : mA(std::move(other.mA)), mB(std::move(other.mB)) { }

        ~Pair() = default;

        Pair& operator=(const Pair& other) {
            this->~Pair();
            new (this) Pair(other);
            return *this;
        }

        Pair& operator=(Pair&& other) noexcept {
            this->~Pair();
            new (this) Pair(std::move(other));
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

    template<typename A, typename B>
    class TextPrint<Pair<A,B>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Pair<A,B>& pair) const {
            TextPrint<A> printA;
            TextPrint<B> printB;

            stream.Add(BERSERK_TEXT("("));
            printA(stream, pair.GetFirst());
            stream.Add(BERSERK_TEXT(", "));
            printB(stream, pair.GetSecond());
            stream.Add(BERSERK_TEXT(")"));
        }
    };

}

#endif //BERSERK_PAIR_HPP