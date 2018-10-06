//
// Created by Egor Orachyov on 26.06.2018.
//

#include "Math/MathUtility.h"
#include "Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    const FLOAT32 Math::BIG_NUMBER = 3.4e+38f;

    const FLOAT32 Math::SMALL_NUMBER = 1.e-8f;

    const FLOAT32 Math::THRESH_FLOAT32 = 0.0001f;

    const FLOAT32 Math::THRESH_POINT_ON_PLANE = 0.10f;

    const FLOAT32 Math::THRESH_POINTS_ARE_SAME = 0.00002f;

    const FLOAT32 Math::THRESH_ZERO_NORM_SQUARED = 0.0001f;

    const FLOAT64 Math::PI = 3.14159265358979323846264338327950288;

    const FLOAT64 Math::HALF_PI = 1.57079632679489661923132169163975144;

    const FLOAT64 Math::QUARTER_PI = 0.785398163397448309615660845819875721;

    const FLOAT64 Math::SQRT2 = 1.41421356237309504880168872420969808;

    const FLOAT64 Math::E = 2.71828182845904523536028747135266250;

    FLOAT32 Math::sin(FLOAT32 rad)
    {
        return sinf(rad);
    }

    FLOAT32 Math::cos(FLOAT32 rad)
    {
        return cosf(rad);
    }

    FLOAT32 Math::tg(FLOAT32 rad)
    {
        return tanf(rad);
    }

    FLOAT32 Math::ctg(FLOAT32 rad)
    {
        return 1.0f / tanf(rad);
    }

    FLOAT32 Math::arcsin(FLOAT32 sin)
    {
        return asinf(sin);
    }

    FLOAT32 Math::arccos(FLOAT32 cos)
    {
        return acosf(cos);
    }

    FLOAT32 Math::arctg(FLOAT32 tg)
    {
        return atanf(tg);
    }

    FLOAT32 Math::arcctg(FLOAT32 ctg)
    {
        return atanf(1.0f / ctg);
    }

    UINT32 Math::min(UINT32 a, UINT32 b)
    {
        return (a > b? b : a);
    }

    INT32 Math::min(INT32 a, INT32 b)
    {
        return (a > b? b : a);
    }

    UINT64 Math::min(UINT64 a, UINT64 b)
    {
        return (a > b? b : a);
    }

    INT64 Math::min(INT64 a, INT64 b)
    {
        return (a > b? b : a);
    }

    FLOAT32 Math::min(FLOAT32 a, FLOAT32 b)
    {
        return (a > b? b : a);
    }

    FLOAT64 Math::min(FLOAT64 a, FLOAT64 b)
    {
        return (a > b? b : a);
    }

    UINT32 Math::max(UINT32 a, UINT32 b)
    {
        return (a > b? a : b);
    }

    INT32 Math::max(INT32 a, INT32 b)
    {
        return (a > b? a : b);
    }

    UINT64 Math::max(UINT64 a, UINT64 b)
    {
        return (a > b? a : b);
    }

    INT64 Math::max(INT64 a, INT64 b)
    {
        return (a > b? a : b);
    }

    FLOAT32 Math::max(FLOAT32 a, FLOAT32 b)
    {
        return (a > b? a : b);
    }

    FLOAT64 Math::max(FLOAT64 a, FLOAT64 b)
    {
        return (a > b? a : b);
    }

    FLOAT32 Math::degrees(FLOAT32 radians)
    {
        return radians / (FLOAT32)(M_PI) * 180;
    }

    FLOAT64 Math::degrees(FLOAT64 radians)
    {
        return radians / M_PI * 180;
    }

    FLOAT32 Math::radians(FLOAT32 degrees)
    {
        return degrees / 180 * (FLOAT32)(M_PI);
    }

    FLOAT64 Math::radians(FLOAT64 degrees)
    {
        return degrees / 180 * M_PI;
    }

    bool Math::between(FLOAT32 t, FLOAT32 p1, FLOAT32 p2)
    {
        return (t >= p1 && t <= p2);
    }

    bool Math::between(FLOAT64 t, FLOAT64 p1, FLOAT64 p2)
    {
        return (t >= p1 && t <= p2);
    }

    FLOAT32 Math::clamp(FLOAT32 t, FLOAT32 down, FLOAT32 up)
    {
        ASSERT(up >= down, "Upper limit should be more than lower limit");

        t = min(t, up);
        t = max(t, down);
        return t;
    }

    FLOAT64 Math::clamp(FLOAT64 t, FLOAT64 down, FLOAT64 up)
    {
        ASSERT(up >= down, "Upper limit should be more than lower limit");

        t = min(t, up);
        t = max(t, down);
        return t;
    }

    FLOAT32 Math::smoothstep(FLOAT32 t, FLOAT32 p1, FLOAT32 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((FLOAT32)((t - p1) / (p2 - p1)), 0.0f, 1.0f);
        return (FLOAT32)(2 * t * t * (1.5 - t));
    }

    FLOAT64 Math::smoothstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return 2 * t * t * (1.5 - t);
    }

    FLOAT32 Math::smootherstep(FLOAT32 t, FLOAT32 p1, FLOAT32 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((FLOAT32)((t - p1) / (p2 - p1)), 0.0f, 1.0f);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    FLOAT64 Math::smootherstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    FLOAT32 Math::random(FLOAT32 min, FLOAT32 max)
    {
        ASSERT(min < max, "Upper limit should be more than lower limit");
        FLOAT32 value = (static_cast<FLOAT32>(rand()) / static_cast<FLOAT32>(RAND_MAX));
        FLOAT32 range = max - min;
        return min + value * range;
    }

    FLOAT64 Math::random(FLOAT64 min, FLOAT64 max)
    {
        ASSERT(min < max, "Upper limit should be more than lower limit");
        FLOAT64 value = (static_cast<FLOAT64>(rand()) / static_cast<FLOAT64>(RAND_MAX));
        FLOAT64 range = max - min;
        return min + value * range;
    }

    void Math::randomize()
    {
        srand(static_cast<UINT32>(time(nullptr)));
    }

} // namespace Berserk