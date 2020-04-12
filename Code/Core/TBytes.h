/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TBYTES_H
#define BERSERK_TBYTES_H

#include <Typedefs.h>

namespace Berserk {

    /** Generic bytes operating utility */
    class TBytes {
    public:

        template <typename T>
        static T reorder(T input) {
            const uint32 N = sizeof(T);

            union {
                T value;
                uint8 bytes[N];
            } in, out;

            in.value = input;

            for (uint32 i = 0; i < N; i++) {
                out.bytes[N - i - 1] = in.bytes[i];
            }

            return out.value;
        }

    };

}

#endif //BERSERK_TBYTES_H