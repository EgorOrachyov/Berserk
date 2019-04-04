//
// Created by Egor Orachyov on 26.01.2019.
//

#ifndef BERSERK_MATHUTILITY_H
#define BERSERK_MATHUTILITY_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{
    /** Forward declaration */

    class Degrees;
    class Radians;

    class Vec2f;
    class Vec3f;
    class Vec4f;

    class Mat2x2f;
    class Mat3x3f;
    class Mat4x4f;

    class Quatf;

    class AABB;
    class Plane;
    class Sphere;

    class Frustum;
    class Rotation;
    class Transform;

} // namespace Berserk

namespace Berserk
{

    /**
     * Implements common math helper functions for the engine.
     * Used as an wrapper fo standard C lib math functions.
     * Provides utilities primary for work with float values.
     */
    class CORE_EXPORT Math
    {
    public:

        /** Float32 really big value */
        static const float32 BIG_NUMBER;

        /** Float32 really small (near by 0.0f) value */
        static const float32 SMALL_NUMBER;

        /** Float calculations mistake */
        static const float32 THRESH_FLOAT32;

        /** Thickness of plane for front/back/inside test */
        static const float32 THRESH_POINT_ON_PLANE;

        /** Two points are same if within this distance */
        static const float32 THRESH_POINTS_ARE_SAME;

        /** Size of a unit normal that is considered "zero", squared */
        static const float32 THRESH_ZERO_NORM_SQUARED;

    public:

        /** Float64 pi const (from c math lib) */
        static const float64 PI;

        /** Float64 pi/2 const (from c math lib) */
        static const float64 HALF_PI;

        /** Float64 pi/4 const (from c math lib) */
        static const float64 QUARTER_PI;

        /** Float64 sqrt(2) const (from c math lib) */
        static const float64 SQRT2;

        /** Float64 e const (from c math lib) */
        static const float64 E;

        /** Float32 pi const (from c math lib) */
        static const float32 PIf;

        /** Float32 pi/2 const (from c math lib) */
        static const float32 HALF_PIf;

        /** Float32 pi/4 const (from c math lib) */
        static const float32 QUARTER_PIf;

        /** Float32 sqrt(2) const (from c math lib) */
        static const float32 SQRT2f;

        /** Float32 e const (from c math lib) */
        static const float32 Ef;

    public:

        /**
         * Absolute value of a
         */
        static float32 abs(float32 a);

        /**
         * Calculate sin of angle rad in radians
         */
        static float32 sin(float32 rad);

        /**
         * Calculate cos of angle rad in radians
         */
        static float32 cos(float32 rad);

        /**
         * Calculate tg of angle rad in radians
         */
        static float32 tg(float32 rad);

        /**
         * Calculate ctg of angle rad in radians
         */
        static float32 ctg(float32 rad);

        /**
         * Calculate arcsin in radians of sin value
         */
        static float32 arcsin(float32 sin);

        /**
         * Calculate arccos in radians of cos value
         */
        static float32 arccos(float32 cos);

        /**
         * Calculate arctg in radians of tg value
         */
        static float32 arctg(float32 tg);

        /**
         * Calculate arcctg in radians of ctg value
         */
        static float32 arcctg(float32 ctg);

        /**
         * Calculate square root from float x
         */
        static float32 sqrt(float32 x);

        /**
         * Get min value from a and b
         */
        static uint32 min(uint32 a, uint32 b);

        /**
         * Get min value from a and b
         */
        static int32 min(int32 a, int32 b);

        /**
         * Get min value from a and b
         */
        static uint64 min(uint64 a, uint64 b);

        /**
         * Get min value from a and b
         */
        static int64 min(int64 a, int64 b);

        /**
         * Get min value from a and b
         */
        static float32 min(float32 a, float32 b);

        /**
         * Get min value from a and b
         */
        static float64 min(float64 a, float64 b);

        /**
         * Get max value from a and b
         */
        static uint32 max(uint32 a, uint32 b);

        /**
         * Get max value from a and b
         */
        static int32 max(int32 a, int32 b);

        /**
         * Get max value from a and b
         */
        static uint64 max(uint64 a, uint64 b);

        /**
         * Get max value from a and b
         */
        static int64 max(int64 a, int64 b);

        /**
         * Get max value from a and b
         */
        static float32 max(float32 a, float32 b);

        /**
         * Get max value from a and b
         */
        static float64 max(float64 a, float64 b);

        /**
         * Convert radians to degrees
         */
        static float32 degrees(float32 radians);

        /**
         * Convert radians to degrees
         */
        static float64 degrees(float64 radians);

        /**
         * Convert degrees to radians
         */
        static float32 radians(float32 degrees);

        /**
         * Convert degrees to radians
         */
        static float64 radians(float64 degrees);

        /**
         * Is value t between p1 and p2
         *
         * @param t Value to check borders
         * @param p1 Left border
         * @param p2 Right border
         * @return True if t in [p1; p2]
         */
        static bool between(float32 t, float32 p1, float32 p2);

        /**
         * Is value t between p1 and p2
         *
         * @param t Value to check borders
         * @param p1 Left border
         * @param p2 Right border
         * @return True if t in [p1; p2]
         */
        static bool between(float64 t, float64 p1, float64 p2);

        /**
         * Transform value t to be in [down; up]
         *
         * @param t Value to clamp
         * @param down Lower limit
         * @param up Upper limit
         * @return t if it is in (down; up) or down (t <= down) or up (t >= up)
         */
        static float32 clamp(float32 t, float32 down, float32 up);

        /**
         * Transform value t to be in [down; up]
         *
         * @param t Value to clamp
         * @param down Lower limit
         * @param up Upper limit
         * @return t if it is in (down; up) or down (t <= down) or up (t >= up)
         */
        static float64 clamp(float64 t, float64 down, float64 up);

        /**
         * Smooth step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static float32 smoothstep(float32 t, float32 p1, float32 p2);

        /**
         * Smooth step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static float64 smoothstep(float64 t, float64 p1, float64 p2);

        /**
         * Smoother step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static float32 smootherstep(float32 t, float32 p1, float32 p2);

        /**
         * Smoother step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static float64 smootherstep(float64 t, float64 p1, float64 p2);

        /**
         * Gen random in range [min;max]
         * @param min Lover limit
         * @param max Upper limit
         */
        static float32 random(float32 min, float32 max);

        /**
         * Gen random in range [min;max]
         * @param min Lover limit
         * @param max Upper limit
         */
        static float64 random(float64 min, float64 max);

        /**
         * Random numbers generator setup
         * @note Should be called for each program's launch
         * to get different random func results
         */
        static void randomize();

    };

} // namespace Berserk

#endif //BERSERK_MATHUTILITY_H
