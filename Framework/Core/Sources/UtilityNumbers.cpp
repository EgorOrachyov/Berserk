//
// Created by Egor Orachyov on 26.06.2018.
//

#include "../Maths/UtilityNumbers.h"
#include "../Essential/Assert.h"
#include <cmath>

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

    float32 min(float32 a, float32 b)
    {
        return (a > b? b : a);
    }

    float64 min(float64 a, float64 b)
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

    float32 max(float32 a, float32 b)
    {
        return (a > b? a : b);
    }

    float64 max(float64 a, float64 b)
    {
        return (a > b? a : b);
    }

    float64 toDegrees(float64 radians)
    {
        return radians / M_PI * 180;
    }

    float64 toRadians(float64 degrees)
    {
        return degrees / 180 * M_PI;
    }

    bool between(float64 t, float64 p1, float64 p2)
    {
        return (t >= p1 && t <= p2);
    }

    float64 clamp(float64 t, float64 down, float64 up)
    {
        ASSERT(up >= down, "Upper limit should be more than lower limit");

        t = min(t, up);
        t = max(t, down);
        return t;
    }

    float64 smoothstep(float64 t, float64 p1, float64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return 2 * t * t * (1.5 - t);
    }

    float64 smootherstep(float64 t, float64 p1, float64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

} // namespace Berserk