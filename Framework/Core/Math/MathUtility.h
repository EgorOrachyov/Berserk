//
// Created by Egor Orachyov on 26.06.2018.
//

#ifndef BERSERKENGINE_MATHUTILITY_H
#define BERSERKENGINE_MATHUTILITY_H

#include "Essential/Types.h"

namespace Berserk
{

    /**
     * Implements common math helper functions for the engine.
     * Uses as an wrapper fo standard C lib math functions.
     * Provides utilities primary for work with float values.
     */
    class Math
    {
    public:

        /** Float64 pi const (from c math lib) */
        static const FLOAT64 PI;

        /** Float64 pi/2 const (from c math lib) */
        static const FLOAT64 PI2;

        /** Float64 pi/4 const (from c math lib) */
        static const FLOAT64 PI4;

        /** Float64 sqrt(2) const (from c math lib) */
        static const FLOAT64 SQRT2;

        /** Float64 e const (from c math lib) */
        static const FLOAT64 E;

    public:

        /**
         * Calculate sin of angle rad in radians
         */
        static FLOAT32 sin(FLOAT32 rad);

        /**
         * Calculate cos of angle rad in radians
         */
        static FLOAT32 cos(FLOAT32 rad);

        /**
         * Calculate tg of angle rad in radians
         */
        static FLOAT32 tg(FLOAT32 rad);

        /**
         * Calculate ctg of angle rad in radians
         */
        static FLOAT32 ctg(FLOAT32 rad);

        /**
         * Calculate arcsin in radians of sin value
         */
        static FLOAT32 arcsin(FLOAT32 sin);

        /**
         * Calculate arccos in radians of cos value
         */
        static FLOAT32 arccos(FLOAT32 cos);

        /**
         * Calculate arctg in radians of tg value
         */
        static FLOAT32 arctg(FLOAT32 tg);

        /**
         * Calculate arcctg in radians of ctg value
         */
        static FLOAT32 arcctg(FLOAT32 ctg);

        /**
         * Get min value from a and b
         */
        static UINT32 min(UINT32 a, UINT32 b);

        /**
         * Get min value from a and b
         */
        static INT32 min(INT32 a, INT32 b);

        /**
         * Get min value from a and b
         */
        static UINT64 min(UINT64 a, UINT64 b);

        /**
         * Get min value from a and b
         */
        static INT64 min(INT64 a, INT64 b);

        /**
         * Get min value from a and b
         */
        static FLOAT32 min(FLOAT32 a, FLOAT32 b);

        /**
         * Get min value from a and b
         */
        static FLOAT64 min(FLOAT64 a, FLOAT64 b);

        /**
         * Get max value from a and b
         */
        static UINT32 max(UINT32 a, UINT32 b);

        /**
         * Get max value from a and b
         */
        static INT32 max(INT32 a, INT32 b);

        /**
         * Get max value from a and b
         */
        static UINT64 max(UINT64 a, UINT64 b);

        /**
         * Get max value from a and b
         */
        static INT64 max(INT64 a, INT64 b);

        /**
         * Get max value from a and b
         */
        static FLOAT32 max(FLOAT32 a, FLOAT32 b);

        /**
         * Get max value from a and b
         */
        static FLOAT64 max(FLOAT64 a, FLOAT64 b);

        /**
         * Convert radians to degrees
         */
        static FLOAT32 degrees(FLOAT32 radians);

        /**
         * Convert radians to degrees
         */
        static FLOAT64 degrees(FLOAT64 radians);

        /**
         * Convert degrees to radians
         */
        static FLOAT32 radians(FLOAT32 degrees);

        /**
         * Convert degrees to radians
         */
        static FLOAT64 radians(FLOAT64 degrees);

        /**
         * Is value t between p1 and p2
         *
         * @param t Value to check borders
         * @param p1 Left border
         * @param p2 Right border
         * @return True if t in [p1; p2]
         */
        static bool between(FLOAT32 t, FLOAT32 p1, FLOAT32 p2);

        /**
         * Is value t between p1 and p2
         *
         * @param t Value to check borders
         * @param p1 Left border
         * @param p2 Right border
         * @return True if t in [p1; p2]
         */
        static bool between(FLOAT64 t, FLOAT64 p1, FLOAT64 p2);

        /**
         * Transform value t to be in [down; up]
         *
         * @param t Value to clamp
         * @param down Lower limit
         * @param up Upper limit
         * @return t if it is in (down; up) or down (t <= down) or up (t >= up)
         */
        static FLOAT32 clamp(FLOAT32 t, FLOAT32 down, FLOAT32 up);

        /**
         * Transform value t to be in [down; up]
         *
         * @param t Value to clamp
         * @param down Lower limit
         * @param up Upper limit
         * @return t if it is in (down; up) or down (t <= down) or up (t >= up)
         */
        static FLOAT64 clamp(FLOAT64 t, FLOAT64 down, FLOAT64 up);

        /**
         * Smooth step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static FLOAT32 smoothstep(FLOAT32 t, FLOAT32 p1, FLOAT32 p2);

        /**
         * Smooth step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static FLOAT64 smoothstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2);

        /**
         * Smoother step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static FLOAT32 smootherstep(FLOAT32 t, FLOAT32 p1, FLOAT32 p2);

        /**
         * Smoother step for t in [p1 p2]
         * @warning p2 should be more than p1
         *
         * @param t Value
         * @param p1 Lower limit
         * @param p2 Upper limit
         * @return Interpolated value t
         */
        static FLOAT64 smootherstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2);

        /**
         * Gen random in range [min;max]
         * @param min Lover limit
         * @param max Upper limit
         */
        static FLOAT32 random(FLOAT32 min, FLOAT32 max);

        /**
         * Gen random in range [min;max]
         * @param min Lover limit
         * @param max Upper limit
         */
        static FLOAT64 random(FLOAT64 min, FLOAT64 max);

        /**
         * Random numbers generator setup
         * @note Should be called for each program's launch
         * to get different rendom func results
         */
        static void randomize();

    };

} // namespace Berserk

#endif //BERSERKENGINE_MATHUTILITY_H
