//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_MATHUTILS_H
#define BERSERK_MATHUTILS_H

#include <HAL/Platform.h>

namespace Berserk {

    /** Wraps default and specific math operations */
    class MathUtils {
    public:

        /** Float32 really big value */
        static const float32 BIG_NUMBERf;

        /** Float32 really small (near by 0.0f) value */
        static const float32 SMALL_NUMBERf;

        /** Float calculations mistake */
        static const float32 THRESH_FLOAT32f;

        /** Thickness of plane for front/back/inside test */
        static const float32 THRESH_POINT_ON_PLANEf;

        /** Two points are same if within this distance */
        static const float32 THRESH_POINTS_ARE_SAMEf;

        /** Size of a unit normal that is considered "zero", squared */
        static const float32 THRESH_ZERO_NORM_SQUAREDf;

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

        /** @return */
        static float32 sinf(float32 radians);

        /** @return */
        static float32 cosf(float32 radians);

        /** @return */
        static float32 tgf(float32 radians);

        /** @return */
        static float32 ctg(float32 radians);

        /** @return */
        static float32 arcsinf(float32 value);

        /** @return */
        static float32 arccosf(float32 value);

        /** @return */
        static float32 arctgf(float32 value);

        /** @return */
        static float32 arcctgf(float32 value);

        /** @return */
        static float32 absf(float32 value);

        /** @return */
        static uint32 min(uint32 a, uint32 b) {}

    };

}



#endif //BERSERK_MATHUTILS_H
