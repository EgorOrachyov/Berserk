/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATH_H
#define BERSERK_MATH_H

#include <Typedefs.h>
#include <math.h>
#include <float.h>

namespace Berserk {

    class Math {
    public:

        static const float32 BIG_NUMBER_FLOAT32;
        static const float32 SMALL_NUMBER_FLOAT32;

        /** Float calculations mistake */
        static const float32 THRESH_FLOAT32;
        static const float32 THRESH_POINT_ON_PLANE;
        static const float32 THRESH_POINTS_ARE_SAME;
        static const float32 THRESH_ZERO_NORM_SQUARED;

        static const float32 THRESH_COMPARE_FLOAT32;
        static const float32 THRESH_COMPARE_FLOAT64;

        /** (from c math lib) */
        static const float64 PI;
        static const float64 HALF_PI;
        static const float64 QUARTER_PI;
        static const float64 SQRT2;
        static const float64 E;

        static const float32 PIf;
        static const float32 HALF_PIf;
        static const float32 QUARTER_PIf;
        static const float32 SQRT2f;
        static const float32 Ef;

        static float32 min(float32 a, float32 b) { return ::fminf(a, b); }
        static float64 min(float64 a, float64 b) { return ::fmin(a, b); }

        static float32 max(float32 a, float32 b) { return ::fmaxf(a, b); }
        static float64 max(float64 a, float64 b) { return ::fmax(a, b); }

        static float32 sin(float32 a) { return ::sinf(a); }
        static float64 sin(float64 a) { return ::sin(a); }

        static float32 cos(float32 a) { return ::cosf(a); }
        static float64 cos(float64 a) { return ::cos(a); }

        static float32 tan(float32 a) { return ::tanf(a); }
        static float64 tan(float64 a) { return ::tan(a); }

        static float32 asin(float32 a) { return ::asinf(a); }
        static float64 asin(float64 a) { return ::asin(a); }

        static float32 acos(float32 a) { return ::acosf(a); }
        static float64 acos(float64 a) { return ::acos(a); }

        static float32 atan(float32 a) { return ::atanf(a); }
        static float64 atan(float64 a) { return ::atan(a); }

        static float32 sqrt(float32 a) { return ::sqrtf(a); }
        static float64 sqrt(float64 a) { return ::sqrt(a); }

        static float32 exp(float32 a) { return ::expf(a); }
        static float64 exp(float64 a) { return ::exp(a); }

        static float32 log(float32 a) { return ::logf(a); }
        static float64 log(float64 a) { return ::log(a); }

        static float32 abs(float32 a) { return ::fabs(a); }
        static float64 abs(float64 a) { return ::abs(a); }

        static float32 degToRad(float32 a) { return a / 180.0f * PIf; }
        static float64 degToRad(float64 a) { return a / 180.0 * PI; }

        static float32 radToDeg(float32 a) { return a * 180.0f / PIf; }
        static float64 radToDeg(float64 a) { return a * 180.0 / PI; }

        /* Math round to nearest int type  */
        static float32 round(float32 a) { return ::roundf(a); }
        static float64 round(float64 a) { return ::round(a); }

        /* Math up round to int type  */
        static float32 ceil(float32 a) { return ::ceilf(a); }
        static float64 ceil(float64 a) { return ::ceil(a); }

        /* Math down round to int type  */
        static float32 floor(float32 a) { return ::floorf(a); }
        static float64 floor(float64 a) { return ::floor(a); }

        static bool same(float32 a, float32 b) {
            return abs(a - b) < THRESH_COMPARE_FLOAT32;
        }

        static bool same(float64 a, float64 b) {
            return abs(a - b) < THRESH_COMPARE_FLOAT64;
        }

        static float32 lerp(float32 t, float32 left, float32 right) {
            return left * (1.0f - t) + right * t;
        }
        static float64 lerp(float64 t, float64 left, float64 right) {
            return left * (1.0f - t) + right * t;
        }

        static float32 smoothstep(float32 t, float32 left, float32 right) {
            t = clamp((t - left) / (right - left), 0.0f, 1.0f);
            return 2 * t * t * (1.5f - t);
        }
        static float64 smoothstep(float64 t, float64 left, float64 right) {
            t = clamp((t - left) / (right - left), 0.0, 1.0);
            return 2 * t * t * (1.5f - t);
        }

        static float32 smootherstep(float32 t, float32 left, float32 right) {
            t = clamp((t - left) / (right - left), 0.0f, 1.0f);
            return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        }
        static float64 smootherstep(float64 t, float64 left, float64 right) {
            t = clamp((t - left) / (right - left), 0.0, 1.0);
            return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        }

        template <typename T>
        static bool between(T t, T left, T right) { return (t >= left) && (t <= right); }

        template <typename T>
        static T clamp(T t, T left, T right) { return (t < left ? left : (t > right ? right : t)); }

        template <typename T>
        static T min(T a, T b) { return (a < b ? a : b); }

        template <typename T>
        static T max(T a, T b) { return (a > b ? a : b); }

        template <typename T>
        static T abs(T a) { return (a > 0 ? a : -a); }

    };

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

#endif //BERSERK_MATH_H