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
    uint32  min(uint32 a, uint32 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    int32   min(int32 a, int32 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    uint64  min(uint64 a, uint64 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    int64   min(int64 a, int64 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    float32 min(float32 a, float32 b);

    /**
     * Get min value from a and b
     *
     * @param a
     * @param b
     * @return min
     */
    float64 min(float64 a, float64 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    uint32  max(uint32 a, uint32 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    int32   max(int32 a, int32 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    uint64  max(uint64 a, uint64 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    int64   max(int64 a, int64 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    float32 max(float32 a, float32 b);

    /**
     * Get max value from a and b
     *
     * @param a
     * @param b
     * @return max
     */
    float64 max(float64 a, float64 b);

    /**
     * Is value t between p1 and p2
     *
     * @param t Value to check borders
     * @param p1 Left border
     * @param p2 Right border
     * @return True if t in [p1; p2]
     */
    bool between(float64 t, float64 p1, float64 p2);

    /**
     * Transform value t to be in [down; up]
     *
     * @param t Value to clamp
     * @param down Lower limit
     * @param up Upper limit
     * @return t if it is in (down; up) or down (t <= down) or up (t >= up)
     */
    float64 clamp(float64 t, float64 down, float64 up);

    /**
     * Smooth step for t in [p1 p2]
     * @warning p2 should be more than p1
     *
     * @param t Value
     * @param p1 Lower limit
     * @param p2 Upper limit
     * @return Interpolated value t
     */
    float64 smoothstep(float64 t, float64 p1, float64 p2);

    /**
     * Smoother step for t in [p1 p2]
     * @warning p2 should be more than p1
     *
     * @param t Value
     * @param p1 Lower limit
     * @param p2 Upper limit
     * @return Interpolated value t
     */
    float64 smootherstep(float64 t, float64 p1, float64 p2);
}

#endif //BERSERKENGINE_BASICARITHMETIC_H
