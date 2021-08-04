/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
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

#include <BerserkCore/Math/Utils.hpp>

#ifndef BERSERK_MATH_PI
#define BERSERK_MATH_PI 3.14159265359
#endif

#ifndef BERSERK_MATH_PI_2
#define BERSERK_MATH_PI_2 1.57079632679
#endif

#ifndef BERSERK_MATH_PI_4
#define BERSERK_MATH_PI_4 0.78539816339
#endif

#ifndef BERSERK_MATH_SQRT2
#define BERSERK_MATH_SQRT2 1.41421356237
#endif

#ifndef BERSERK_MATH_E
#define BERSERK_MATH_E 2.71828182846
#endif

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

        const double Utils::PI = BERSERK_MATH_PI;
        const double Utils::HALF_PI = BERSERK_MATH_PI_2;
        const double Utils::QUARTER_PI = BERSERK_MATH_PI_4;
        const double Utils::SQRT2 = BERSERK_MATH_SQRT2;
        const double Utils::E = BERSERK_MATH_E;

        const float Utils::PIf = (float) BERSERK_MATH_PI;
        const float Utils::HALF_PIf = (float) BERSERK_MATH_PI_2;
        const float Utils::QUARTER_PIf = (float) BERSERK_MATH_PI_4;
        const float Utils::SQRT2f = (float) BERSERK_MATH_SQRT2;
        const float Utils::Ef = (float) BERSERK_MATH_E;

    }
}