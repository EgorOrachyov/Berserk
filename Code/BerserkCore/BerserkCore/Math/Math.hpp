/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MATH_HPP
#define BERSERK_MATH_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    class Math {
    public:

        static const float BIG_NUMBER_FLOAT32;
        static const float SMALL_NUMBER_FLOAT32;

        /** Float calculations mistake */
        static const float THRESH_FLOAT32;
        static const float THRESH_POINT_ON_PLANE;
        static const float THRESH_POINTS_ARE_SAME;
        static const float THRESH_ZERO_NORM_SQUARED;

        static const float THRESH_COMPARE_FLOAT32;
        static const float THRESH_COMPARE_FLOAT64;

        /** (from c math lib) */
        static const double PI;
        static const double HALF_PI;
        static const double QUARTER_PI;
        static const double SQRT2;
        static const double E;

        static const float PIf;
        static const float HALF_PIf;
        static const float QUARTER_PIf;
        static const float SQRT2f;
        static const float Ef;

        static void split(float v, float& integer, float& fractional) {
            fractional = ::modff(v, &integer);

        }
        static void split(double v, double& integer, double& fractional) {
            fractional = ::modf(v, &integer);
        }

        static float min(float a, float b) { return ::fminf(a, b); }
        static double min(double a, double b) { return ::fmin(a, b); }

        static float max(float a, float b) { return ::fmaxf(a, b); }
        static double max(double a, double b) { return ::fmax(a, b); }

        static float sin(float a) { return ::sinf(a); }
        static double sin(double a) { return ::sin(a); }

        static float cos(float a) { return ::cosf(a); }
        static double cos(double a) { return ::cos(a); }

        static float tan(float a) { return ::tanf(a); }
        static double tan(double a) { return ::tan(a); }

        static float asin(float a) { return ::asinf(a); }
        static double asin(double a) { return ::asin(a); }

        static float acos(float a) { return ::acosf(a); }
        static double acos(double a) { return ::acos(a); }

        static float atan(float a) { return ::atanf(a); }
        static double atan(double a) { return ::atan(a); }

        static float sqrt(float a) { return ::sqrtf(a); }
        static double sqrt(double a) { return ::sqrt(a); }

        static float exp(float a) { return ::expf(a); }
        static double exp(double a) { return ::exp(a); }

        static float pow(float a, float p) { return ::powf(a,p); }
        static double pow(double a, double p) { return ::pow(a,p); }

        static float log(float a) { return ::logf(a); }
        static double log(double a) { return ::log(a); }

        static float log2(float a) { return ::log2f(a); }
        static double log2(double a) { return ::log2(a); }

        static float log10(float a) { return ::log10f(a); }
        static double log10(double a) { return ::log10(a); }

        static float abs(float a) { return ::fabs(a); }
        static double abs(double a) { return ::abs(a); }

        static float degToRad(float a) { return a / 180.0f * PIf; }
        static double degToRad(double a) { return a / 180.0 * PI; }

        static float radToDeg(float a) { return a * 180.0f / PIf; }
        static double radToDeg(double a) { return a * 180.0 / PI; }

        /* Math round to nearest int type  */
        static float round(float a) { return ::roundf(a); }
        static double round(double a) { return ::round(a); }

        /* Math up round to int type  */
        static float ceil(float a) { return ::ceilf(a); }
        static double ceil(double a) { return ::ceil(a); }

        /* Math down round to int type  */
        static float floor(float a) { return ::floorf(a); }
        static double floor(double a) { return ::floor(a); }

        static bool same(float a, float b) {
            return abs(a - b) < THRESH_COMPARE_FLOAT32;
        }

        static bool same(double a, double b) {
            return abs(a - b) < THRESH_COMPARE_FLOAT64;
        }

        static float lerp(float t, float left, float right) {
            return left * (1.0f - t) + right * t;
        }

        static double lerp(double t, double left, double right) {
            return left * (1.0 - t) + right * t;
        }

        static float smoothstep(float t, float left, float right) {
            t = clamp((t - left) / (right - left), 0.0f, 1.0f);
            return 2.0f * t * t * (1.5f - t);
        }

        static double smoothstep(double t, double left, double right) {
            t = clamp((t - left) / (right - left), 0.0, 1.0);
            return 2.0 * t * t * (1.5 - t);
        }

        static float smootherstep(float t, float left, float right) {
            t = clamp((t - left) / (right - left), 0.0f, 1.0f);
            return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        }

        static double smootherstep(double t, double left, double right) {
            t = clamp((t - left) / (right - left), 0.0, 1.0);
            return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
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

        template <typename T, T a>
        static constexpr T const_max() noexcept { return a; };

        template <typename T, T a, T b, T ... args>
        static constexpr T const_max() noexcept {
            return (a > b? const_max<T,a,args...>() : const_max<T,b,args...>());
        };

        template <typename T, T a>
        static constexpr T const_min() noexcept { return a; };

        template <typename T, T a, T b, T ... args>
        static constexpr T const_min() noexcept {
            return (a < b? const_max<T,a,args...>() : const_max<T,b,args...>());
        };

    };

}

#endif //BERSERK_MATH_HPP