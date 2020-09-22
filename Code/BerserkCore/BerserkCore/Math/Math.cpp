/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Math/Math.hpp>

namespace Berserk {
    const float Math::BIG_NUMBER_FLOAT32 = 3.4e+38f;
    const float Math::SMALL_NUMBER_FLOAT32 = 1.e-8f;

    const float Math::THRESH_FLOAT32 = 0.0001f;
    const float Math::THRESH_POINT_ON_PLANE = 0.10f;
    const float Math::THRESH_POINTS_ARE_SAME = 0.00002f;
    const float Math::THRESH_ZERO_NORM_SQUARED = 0.0001f;

    const float Math::THRESH_COMPARE_FLOAT32 = 0.0001f;
    const float Math::THRESH_COMPARE_FLOAT64 = 0.000001f;

    const double Math::PI = M_PI;
    const double Math::HALF_PI = M_PI_2;
    const double Math::QUARTER_PI = M_PI_4;
    const double Math::SQRT2 = M_SQRT2;
    const double Math::E = M_E;

    const float Math::PIf = (float) M_PI;
    const float Math::HALF_PIf = (float) M_PI_2;
    const float Math::QUARTER_PIf = (float) M_PI_4;
    const float Math::SQRT2f = (float) M_SQRT2;
    const float Math::Ef = (float) M_E;
}