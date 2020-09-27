/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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