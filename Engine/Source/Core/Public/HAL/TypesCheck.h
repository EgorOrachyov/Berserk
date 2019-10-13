//
// Created by Egor Orachyov on 2019-10-13.
//

#ifndef BERSERK_TYPESCHECK_H
#define BERSERK_TYPESCHECK_H

namespace Berserk {

#define SIZE_MESSAGE "Inappropriate type size for platform"

    static_assert(sizeof(float32) == 4, SIZE_MESSAGE);
    static_assert(sizeof(float64) == 8, SIZE_MESSAGE);

    static_assert(sizeof(int8)   == 1, SIZE_MESSAGE);
    static_assert(sizeof(int16)  == 2, SIZE_MESSAGE);
    static_assert(sizeof(int32)  == 4, SIZE_MESSAGE);
    static_assert(sizeof(int64)  == 8, SIZE_MESSAGE);

    static_assert(sizeof(uint8)  == 1, SIZE_MESSAGE);
    static_assert(sizeof(uint16) == 2, SIZE_MESSAGE);
    static_assert(sizeof(uint32) == 4, SIZE_MESSAGE);
    static_assert(sizeof(uint64) == 8, SIZE_MESSAGE);

#undef SIZE_MESSAGE

}

#endif //BERSERK_TYPESCHECK_H