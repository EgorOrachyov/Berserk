//
// Created by Egor Orachyov on 26.06.2018.
//

#include "../Core/Maths/BasicArithmetic.h"

namespace Berserk
{

    uint32  min(uint32 a, uint32 b)
    {
        return (a > b? b : a);
    }

    int32   min(int32 a, int32 b)
    {
        return (a > b? b : a);
    }

    uint64  min(uint64 a, uint64 b)
    {
        return (a > b? b : a);
    }

    int64   min(int64 a, int64 b)
    {
        return (a > b? b : a);
    }

    uint32  max(uint32 a, uint32 b)
    {
        return (a > b? b : a);
    }

    int32   max(int32 a, int32 b)
    {
        return (a > b? a : b);
    }

    uint64  max(uint64 a, uint64 b)
    {
        return (a > b? a : b);
    }

    int64   max(int64 a, int64 b)
    {
        return (a > b? a : b);
    }

} // namespace Berserk