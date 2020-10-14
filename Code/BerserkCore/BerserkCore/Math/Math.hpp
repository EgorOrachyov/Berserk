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

        static void Split(float v, float& integer, float& fractional) {
            fractional = ::modff(v, &integer);

        }
        static void Split(double v, double& integer, double& fractional) {
            fractional = ::modf(v, &integer);
        }

        static float Min(float a, float b) { return ::fminf(a, b); }
        static double Min(double a, double b) { return ::fmin(a, b); }

        static float Max(float a, float b) { return ::fmaxf(a, b); }
        static double Max(double a, double b) { return ::fmax(a, b); }

        static float Sin(float a) { return ::sinf(a); }
        static double Sin(double a) { return ::sin(a); }

        static float Cos(float a) { return ::cosf(a); }
        static double Cos(double a) { return ::cos(a); }

        static float Tan(float a) { return ::tanf(a); }
        static double Tan(double a) { return ::tan(a); }

        static float Asin(float a) { return ::asinf(a); }
        static double Asin(double a) { return ::asin(a); }

        static float Acos(float a) { return ::acosf(a); }
        static double Acos(double a) { return ::acos(a); }

        static float Atan(float a) { return ::atanf(a); }
        static double Atan(double a) { return ::atan(a); }

        static float Sqrt(float a) { return ::sqrtf(a); }
        static double Sqrt(double a) { return ::sqrt(a); }

        static float Exp(float a) { return ::expf(a); }
        static double Exp(double a) { return ::exp(a); }

        static float Pow(float a, float p) { return ::powf(a,p); }
        static double Pow(double a, double p) { return ::pow(a,p); }

        static float Log(float a) { return ::logf(a); }
        static double Log(double a) { return ::log(a); }

        static float Log2(float a) { return ::log2f(a); }
        static double Log2(double a) { return ::log2(a); }

        static float Log10(float a) { return ::log10f(a); }
        static double Log10(double a) { return ::log10(a); }

        static float Abs(float a) { return ::fabs(a); }
        static double Abs(double a) { return ::abs(a); }

        static float DegToRad(float a) { return a / 180.0f * PIf; }
        static double DegToRad(double a) { return a / 180.0 * PI; }

        static float RadToDeg(float a) { return a * 180.0f / PIf; }
        static double RadToDeg(double a) { return a * 180.0 / PI; }

        /* Math round to nearest int type  */
        static float Round(float a) { return ::roundf(a); }
        static double Round(double a) { return ::round(a); }

        /* Math up round to int type  */
        static float Ceil(float a) { return ::ceilf(a); }
        static double Ceil(double a) { return ::ceil(a); }

        /* Math down round to int type  */
        static float Floor(float a) { return ::floorf(a); }
        static double Floor(double a) { return ::floor(a); }

        static bool Same(float a, float b) {
            return Abs(a - b) < THRESH_COMPARE_FLOAT32;
        }

        static bool Same(double a, double b) {
            return Abs(a - b) < THRESH_COMPARE_FLOAT64;
        }

        static float Lerp(float t, float left, float right) {
            return left * (1.0f - t) + right * t;
        }

        static double Lerp(double t, double left, double right) {
            return left * (1.0 - t) + right * t;
        }

        static float SmoothStep(float t, float left, float right) {
            t = Clamp((t - left) / (right - left), 0.0f, 1.0f);
            return 2.0f * t * t * (1.5f - t);
        }

        static double SmoothStep(double t, double left, double right) {
            t = Clamp((t - left) / (right - left), 0.0, 1.0);
            return 2.0 * t * t * (1.5 - t);
        }

        static float SmootherStep(float t, float left, float right) {
            t = Clamp((t - left) / (right - left), 0.0f, 1.0f);
            return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        }

        static double SmootherStep(double t, double left, double right) {
            t = Clamp((t - left) / (right - left), 0.0, 1.0);
            return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
        }

        template <typename T>
        static bool Between(T t, T left, T right) { return (t >= left) && (t <= right); }

        template <typename T>
        static T Clamp(T t, T left, T right) { return (t < left ? left : (t > right ? right : t)); }

        template <typename T>
        static T Min(T a, T b) { return (a < b ? a : b); }

        template <typename T>
        static T Max(T a, T b) { return (a > b ? a : b); }

        template <typename T>
        static T Abs(T a) { return (a > 0 ? a : -a); }

        template <typename T, T a>
        static constexpr T ConstMax() noexcept { return a; };

        template <typename T, T a, T b, T ... args>
        static constexpr T ConstMax() noexcept {
            return (a > b? ConstMax<T,a,args...>() : ConstMax<T,b,args...>());
        };

        template <typename T, T a>
        static constexpr T ConstMin() noexcept { return a; };

        template <typename T, T a, T b, T ... args>
        static constexpr T ConstMin() noexcept {
            return (a < b? ConstMin<T,a,args...>() : ConstMin<T,b,args...>());
        };

    };

}

#endif //BERSERK_MATH_HPP