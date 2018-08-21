//
// Created by Egor Orachyov on 26.06.2018.
//

#include "Math/UtilityNumbers.h"
#include "Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    UINT32  min(UINT32 a, UINT32 b)
    {
        return (a > b? b : a);
    }

    INT32   min(INT32 a, INT32 b)
    {
        return (a > b? b : a);
    }

    UINT64  min(UINT64 a, UINT64 b)
    {
        return (a > b? b : a);
    }

    INT64   min(INT64 a, INT64 b)
    {
        return (a > b? b : a);
    }

    FLOAT32 min(FLOAT32 a, FLOAT32 b)
    {
        return (a > b? b : a);
    }

    FLOAT64 min(FLOAT64 a, FLOAT64 b)
    {
        return (a > b? b : a);
    }

    UINT32  max(UINT32 a, UINT32 b)
    {
        return (a > b? a : b);
    }

    INT32   max(INT32 a, INT32 b)
    {
        return (a > b? a : b);
    }

    UINT64  max(UINT64 a, UINT64 b)
    {
        return (a > b? a : b);
    }

    INT64   max(INT64 a, INT64 b)
    {
        return (a > b? a : b);
    }

    FLOAT32 max(FLOAT32 a, FLOAT32 b)
    {
        return (a > b? a : b);
    }

    FLOAT64 max(FLOAT64 a, FLOAT64 b)
    {
        return (a > b? a : b);
    }

    FLOAT64 toDegrees(FLOAT64 radians)
    {
        return radians / M_PI * 180;
    }

    FLOAT64 toRadians(FLOAT64 degrees)
    {
        return degrees / 180 * M_PI;
    }

    bool between(FLOAT64 t, FLOAT64 p1, FLOAT64 p2)
    {
        return (t >= p1 && t <= p2);
    }

    FLOAT64 clamp(FLOAT64 t, FLOAT64 down, FLOAT64 up)
    {
        ASSERT(up >= down, "Upper limit should be more than lower limit");

        t = min(t, up);
        t = max(t, down);
        return t;
    }

    FLOAT64 smoothstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return 2 * t * t * (1.5 - t);
    }

    FLOAT64 smootherstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    FLOAT32 random(FLOAT32 min, FLOAT32 max)
    {
        ASSERT(min < max, "Upper limit should be more than lower limit");
        FLOAT32 value = (static_cast<FLOAT32>(rand()) / static_cast<FLOAT32>(RAND_MAX));
        FLOAT32 range = max - min;
        return min + value * range;
    }

    FLOAT64 random(FLOAT64 min, FLOAT64 max)
    {
        ASSERT(min < max, "Upper limit should be more than lower limit");
        FLOAT64 value = (static_cast<FLOAT64>(rand()) / static_cast<FLOAT64>(RAND_MAX));
        FLOAT64 range = max - min;
        return min + value * range;
    }

    void randomize()
    {
        srand(static_cast<UINT32>(time(nullptr)));
    }

} // namespace Berserk