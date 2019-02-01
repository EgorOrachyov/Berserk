//
// Created by Egor Orachyov on 26.01.2019.
//

#include "Public/Misc/Assert.h"
#include "Public/Math/MathUtility.h"

namespace Berserk
{

    const float32 Math::BIG_NUMBER = 3.4e+38f;

    const float32 Math::SMALL_NUMBER = 1.e-8f;

    const float32 Math::THRESH_FLOAT32 = 0.0001f;

    const float32 Math::THRESH_POINT_ON_PLANE = 0.10f;

    const float32 Math::THRESH_POINTS_ARE_SAME = 0.00002f;

    const float32 Math::THRESH_ZERO_NORM_SQUARED = 0.0001f;

    const float64 Math::PI = M_PI;

    const float64 Math::HALF_PI = M_PI_2;

    const float64 Math::QUARTER_PI = M_PI_4;

    const float64 Math::SQRT2 = M_SQRT2;

    const float64 Math::E = M_E;

    float32 Math::sin(float32 rad)
    {
        return sinf(rad);
    }

    float32 Math::cos(float32 rad)
    {
        return cosf(rad);
    }

    float32 Math::tg(float32 rad)
    {
        return tanf(rad);
    }

    float32 Math::ctg(float32 rad)
    {
        return 1.0f / tanf(rad);
    }

    float32 Math::arcsin(float32 sin)
    {
        return asinf(sin);
    }

    float32 Math::arccos(float32 cos)
    {
        return acosf(cos);
    }

    float32 Math::arctg(float32 tg)
    {
        return atanf(tg);
    }

    float32 Math::arcctg(float32 ctg)
    {
        return atanf(1.0f / ctg);
    }

    float32 Math::sqrt(float32 x)
    {
        return sqrtf(x);
    }

    uint32 Math::min(uint32 a, uint32 b)
    {
        return (a > b? b : a);
    }

    int32 Math::min(int32 a, int32 b)
    {
        return (a > b? b : a);
    }

    uint64 Math::min(uint64 a, uint64 b)
    {
        return (a > b? b : a);
    }

    int64 Math::min(int64 a, int64 b)
    {
        return (a > b? b : a);
    }

    float32 Math::min(float32 a, float32 b)
    {
        return (a > b? b : a);
    }

    float64 Math::min(float64 a, float64 b)
    {
        return (a > b? b : a);
    }

    uint32 Math::max(uint32 a, uint32 b)
    {
        return (a > b? a : b);
    }

    int32 Math::max(int32 a, int32 b)
    {
        return (a > b? a : b);
    }

    uint64 Math::max(uint64 a, uint64 b)
    {
        return (a > b? a : b);
    }

    int64 Math::max(int64 a, int64 b)
    {
        return (a > b? a : b);
    }

    float32 Math::max(float32 a, float32 b)
    {
        return (a > b? a : b);
    }

    float64 Math::max(float64 a, float64 b)
    {
        return (a > b? a : b);
    }

    float32 Math::degrees(float32 radians)
    {
        return radians / (float32)(M_PI) * 180;
    }

    float64 Math::degrees(float64 radians)
    {
        return radians / M_PI * 180;
    }

    float32 Math::radians(float32 degrees)
    {
        return degrees / 180 * (float32)(M_PI);
    }

    float64 Math::radians(float64 degrees)
    {
        return degrees / 180 * M_PI;
    }

    bool Math::between(float32 t, float32 p1, float32 p2)
    {
        return (t >= p1 && t <= p2);
    }

    bool Math::between(float64 t, float64 p1, float64 p2)
    {
        return (t >= p1 && t <= p2);
    }

    float32 Math::clamp(float32 t, float32 down, float32 up)
    {
        ASSERT(up >= down, "Upper limit should be more than lower limit");

        t = min(t, up);
        t = max(t, down);
        return t;
    }

    float64 Math::clamp(float64 t, float64 down, float64 up)
    {
        ASSERT(up >= down, "Upper limit should be more than lower limit");

        t = min(t, up);
        t = max(t, down);
        return t;
    }

    float32 Math::smoothstep(float32 t, float32 p1, float32 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((float32)((t - p1) / (p2 - p1)), 0.0f, 1.0f);
        return (float32)(2 * t * t * (1.5 - t));
    }

    float64 Math::smoothstep(float64 t, float64 p1, float64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return 2 * t * t * (1.5 - t);
    }

    float32 Math::smootherstep(float32 t, float32 p1, float32 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((float32)((t - p1) / (p2 - p1)), 0.0f, 1.0f);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float64 Math::smootherstep(float64 t, float64 p1, float64 p2)
    {
        ASSERT(p2 > p1, "Upper limit should be more than lower limit");
        ASSERT((t >= p1 && t <= p2), "Param t should be in [p1; p2]");

        t = clamp((t - p1) / (p2 - p1), 0.0, 1.0);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float32 Math::random(float32 min, float32 max)
    {
        ASSERT(min < max, "Upper limit should be more than lower limit");
        float32 value = (static_cast<float32>(rand()) / static_cast<float32>(RAND_MAX));
        float32 range = max - min;
        return min + value * range;
    }

    float64 Math::random(float64 min, float64 max)
    {
        ASSERT(min < max, "Upper limit should be more than lower limit");
        float64 value = (static_cast<float64>(rand()) / static_cast<float64>(RAND_MAX));
        float64 range = max - min;
        return min + value * range;
    }

    void Math::randomize()
    {
        srand(static_cast<uint32>(time(nullptr)));
    }

} // namespace Berserk