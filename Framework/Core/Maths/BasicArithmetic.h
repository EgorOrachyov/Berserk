//
// Created by Egor Orachyov on 26.06.2018.
//

#ifndef BERSERKENGINE_BASICARITHMETIC_H
#define BERSERKENGINE_BASICARITHMETIC_H

#include "../Essential/Types.h"

namespace Berserk
{

    uint32  min(uint32 a, uint32 b);
    int32   min(int32 a, int32 b);
    uint64  min(uint64 a, uint64 b);
    int64   min(int64 a, int64 b);

    uint32  max(uint32 a, uint32 b);
    int32   max(int32 a, int32 b);
    uint64  max(uint64 a, uint64 b);
    int64   max(int64 a, int64 b);

}

#endif //BERSERKENGINE_BASICARITHMETIC_H
