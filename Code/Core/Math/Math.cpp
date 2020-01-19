/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Math/Math.h>

namespace Berserk {
    const float32 Math::BIG_NUMBER_FLOAT32 = 3.4e+38f;
    const float32 Math::SMALL_NUMBER_FLOAT32 = 1.e-8f;

    const float32 Math::THRESH_FLOAT32 = 0.0001f;
    const float32 Math::THRESH_POINT_ON_PLANE = 0.10f;
    const float32 Math::THRESH_POINTS_ARE_SAME = 0.00002f;
    const float32 Math::THRESH_ZERO_NORM_SQUARED = 0.0001f;

    const float32 Math::THRESH_COMPARE_FLOAT32 = 0.0001f;
    const float32 Math::THRESH_COMPARE_FLOAT64 = 0.000001f;

    const float64 Math::PI = M_PI;
    const float64 Math::HALF_PI = M_PI_2;
    const float64 Math::QUARTER_PI = M_PI_4;
    const float64 Math::SQRT2 = M_SQRT2;
    const float64 Math::E = M_E;

    const float32 Math::PIf = (float32) M_PI;
    const float32 Math::HALF_PIf = (float32) M_PI_2;
    const float32 Math::QUARTER_PIf = (float32) M_PI_4;
    const float32 Math::SQRT2f = (float32) M_SQRT2;
    const float32 Math::Ef = (float32) M_E;
}