/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TPREDICATES_H
#define BERSERK_TPREDICATES_H

#include <Typedefs.h>
#include <Crc32.h>

namespace Berserk {

    template <typename T>
    struct Equals {
        bool operator()(const T& a, const T& b) const {
            return a == b;
        }
    };

    template <typename T>
    struct Hash {
        uint32 operator()(const T& a) const {
            return a.hash();
        }
    };

    template <typename T>
    struct HashRaw {
        uint32 operator()(const T& a) const {
            return Crc32::hash(&a, sizeof(T));
        }
    };

}

#endif //BERSERK_TPREDICATES_H
