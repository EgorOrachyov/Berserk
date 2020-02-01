/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TPAIR_H
#define BERSERK_TPAIR_H

#include <Typedefs.h>
#include <Platform/Memory.h>

namespace Berserk {

    template <typename A, typename B>
    class TPair {
    public:
        TPair() {
            new (mBufferA) A();
            new (mBufferB) B();
        }
        TPair(const A& a, const B& b) {
            new (mBufferA) A(a);
            new (mBufferB) B(b);
        }
        ~TPair() {
            first()->~A();
            second()->~B();
        }

        A& first() { return *((A*)mBufferA); }
        const A& first() const { return *((A*)mBufferA); }

        B& second() { return *((B*)mBufferB); }
        const B& second() const { return *((B*)mBufferB); }

    private:
        uint8 mBufferA[sizeof(A)] = {};
        uint8 mBufferB[sizeof(B)] = {};
    };

}

#endif //BERSERK_TPAIR_H