/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TPREDICATES_H
#define BERSERK_TPREDICATES_H

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Crc32.h>

namespace Berserk {

    template <typename T>
    struct TEquals {
        bool operator()(const T& a, const T& b) const {
            return a == b;
        }
    };

    template <typename T>
    struct THash {
        uint32 operator()(const T& a) const {
            return a.hash();
        }
    };

    template <typename T>
    struct THashRaw {
        uint32 operator()(const T& a) const {
            return Crc32::hash(&a, sizeof(T));
        }
    };

    template <typename T>
    struct TCompareLessEq {
        bool operator()(const T& a, const T& b) const {
            return a <= b;
        }
    };

}

#endif //BERSERK_TPREDICATES_H