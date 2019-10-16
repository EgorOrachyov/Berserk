/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

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
        static uint32 min(uint32 a, uint32 b);

        /** @return */
        static uint32 max(uint32 a, uint32 b);

    };

}



#endif //BERSERK_MATHUTILS_H
