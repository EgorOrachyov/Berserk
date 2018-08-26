//
// Created by Egor Orachyov on 26.06.2018.
//

#ifndef BERSERKENGINE_BASICARITHMETIC_H
#define BERSERKENGINE_BASICARITHMETIC_H

#include "../Essential/Types.h"

namespace Berserk
{
    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    UINT32  min(UINT32 a, UINT32 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    INT32   min(INT32 a, INT32 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    UINT64  min(UINT64 a, UINT64 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    INT64   min(INT64 a, INT64 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    FLOAT32 min(FLOAT32 a, FLOAT32 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    FLOAT64 min(FLOAT64 a, FLOAT64 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    UINT32  max(UINT32 a, UINT32 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    INT32   max(INT32 a, INT32 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    UINT64  max(UINT64 a, UINT64 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    INT64   max(INT64 a, INT64 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    FLOAT32 max(FLOAT32 a, FLOAT32 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    FLOAT64 max(FLOAT64 a, FLOAT64 b);

    /**
     * Convert radians to degrees
     *
     * @param radians
     * @return
     */
    FLOAT32 toDegrees(FLOAT32 radians);

    /**
     * Convert radians to degrees
     *
     * @param radians
     * @return
     */
    FLOAT64 toDegrees(FLOAT64 radians);

    /**
     * Convert degrees to radians
     *
     * @param degrees
     * @return
     */
    FLOAT32 toRadians(FLOAT32 degrees);

    /**
     * Convert degrees to radians
     *
     * @param degrees
     * @return
     */
    FLOAT64 toRadians(FLOAT64 degrees);

    /**
     * Is value t between p1 and p2
     *
     * @param t Value to check borders
     * @param p1 Left border
     * @param p2 Right border
     * @return True if t in [p1; p2]
     */
    bool between(FLOAT64 t, FLOAT64 p1, FLOAT64 p2);

    /**
     * Transform value t to be in [down; up]
     *
     * @param t Value to clamp
     * @param down Lower limit
     * @param up Upper limit
     * @return t if it is in (down; up) or down (t <= down) or up (t >= up)
     */
    FLOAT64 clamp(FLOAT64 t, FLOAT64 down, FLOAT64 up);

    /**
     * Smooth step for t in [p1 p2]
     * @warning p2 should be more than p1
     *
     * @param t Value
     * @param p1 Lower limit
     * @param p2 Upper limit
     * @return Interpolated value t
     */
    FLOAT64 smoothstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2);

    /**
     * Smoother step for t in [p1 p2]
     * @warning p2 should be more than p1
     *
     * @param t Value
     * @param p1 Lower limit
     * @param p2 Upper limit
     * @return Interpolated value t
     */
    FLOAT64 smootherstep(FLOAT64 t, FLOAT64 p1, FLOAT64 p2);

    /**
     * Gen random in range [min;max]
     * @param min
     * @param max
     */
    FLOAT32 random(FLOAT32 min, FLOAT32 max);

    /**
     * Gen random in range [min;max]
     * @param min
     * @param max
     */
    FLOAT64 random(FLOAT64 min, FLOAT64 max);

    /**
     * Random numbers generator setup
     */
    void randomize();
}

#endif //BERSERKENGINE_BASICARITHMETIC_H
