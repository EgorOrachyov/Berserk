/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Math/Utils.hpp>

namespace Berserk {
    namespace Math {

        const float Utils::BIG_NUMBER_FLOAT32 = 3.4e+38f;
        const float Utils::SMALL_NUMBER_FLOAT32 = 1.e-8f;

        const float Utils::THRESH_FLOAT32 = 0.0001f;
        const float Utils::THRESH_POINT_ON_PLANE = 0.10f;
        const float Utils::THRESH_POINTS_ARE_SAME = 0.00002f;
        const float Utils::THRESH_ZERO_NORM_SQUARED = 0.0001f;

        const float Utils::THRESH_COMPARE_FLOAT32 = 0.0001f;
        const float Utils::THRESH_COMPARE_FLOAT64 = 0.000001f;

        const double Utils::PI = M_PI;
        const double Utils::HALF_PI = M_PI_2;
        const double Utils::QUARTER_PI = M_PI_4;
        const double Utils::SQRT2 = M_SQRT2;
        const double Utils::E = M_E;

        const float Utils::PIf = (float) M_PI;
        const float Utils::HALF_PIf = (float) M_PI_2;
        const float Utils::QUARTER_PIf = (float) M_PI_4;
        const float Utils::SQRT2f = (float) M_SQRT2;
        const float Utils::Ef = (float) M_E;

    }
}