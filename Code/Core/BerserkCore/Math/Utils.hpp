/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UTILS_HPP
#define BERSERK_UTILS_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <cmath>

namespace Berserk {
    namespace Math {

        class Utils {
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

            static void Split(float v, float &integer, float &fractional) {
                fractional = std::modf(v, &integer);

            }

            static void Split(double v, double &integer, double &fractional) {
                fractional = std::modf(v, &integer);
            }

            static float Min(float a, float b) { return std::fminf(a, b); }
            static double Min(double a, double b) { return std::fmin(a, b); }

            static float Max(float a, float b) { return std::fmaxf(a, b); }
            static double Max(double a, double b) { return std::fmax(a, b); }

            static float Sin(float a) { return std::sin(a); }
            static double Sin(double a) { return std::sin(a); }

            static float Cos(float a) { return std::cos(a); }
            static double Cos(double a) { return std::cos(a); }

            static float Tan(float a) { return std::tan(a); }
            static double Tan(double a) { return std::tan(a); }

            static float Asin(float a) { return std::asin(a); }
            static double Asin(double a) { return std::asin(a); }

            static float Acos(float a) { return std::acos(a); }
            static double Acos(double a) { return std::acos(a); }

            static float Atan(float a) { return std::atan(a); }
            static double Atan(double a) { return std::atan(a); }

            static float Sqrt(float a) { return std::sqrt(a); }
            static double Sqrt(double a) { return std::sqrt(a); }

            static float Exp(float a) { return std::exp(a); }
            static double Exp(double a) { return std::exp(a); }

            static float Pow(float a, float p) { return std::pow(a, p); }
            static double Pow(double a, double p) { return std::pow(a, p); }

            static float Log(float a) { return std::log(a); }
            static double Log(double a) { return std::log(a); }

            static float Log2(float a) { return std::log2(a); }
            static double Log2(double a) { return std::log2(a); }

            static float Log10(float a) { return std::log10(a); }
            static double Log10(double a) { return std::log10(a); }

            static float Abs(float a) { return std::fabs(a); }
            static double Abs(double a) { return std::abs(a); }

            static float DegToRad(float a) { return a / 180.0f * PIf; }
            static double DegToRad(double a) { return a / 180.0 * PI; }

            static float RadToDeg(float a) { return a * 180.0f / PIf; }
            static double RadToDeg(double a) { return a * 180.0 / PI; }

            /* Math round to nearest int type  */
            static float Round(float a) { return std::round(a); }
            static double Round(double a) { return std::round(a); }

            /* Math up round to int type  */
            static float Ceil(float a) { return std::ceil(a); }
            static double Ceil(double a) { return std::ceil(a); }

            /* Math down round to int type  */
            static float Floor(float a) { return std::floor(a); }
            static double Floor(double a) { return std::floor(a); }

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

            template<typename T>
            static bool Between(T t, T left, T right) { return (t >= left) && (t <= right); }

            template<typename T>
            static T Clamp(T t, T left, T right) { return (t < left ? left : (t > right ? right : t)); }

            template<typename T>
            static T Min(T a, T b) { return (a < b ? a : b); }

            template<typename T>
            static T Max(T a, T b) { return (a > b ? a : b); }

            template<typename T>
            static T Abs(T a) { return (a > 0 ? a : -a); }

            template<typename T, T a>
            static constexpr T ConstMax() noexcept { return a; };

            template<typename T, T a, T b, T ... args>
            static constexpr T ConstMax() noexcept {
                return (a > b ? ConstMax<T, a, args...>() : ConstMax<T, b, args...>());
            };

            template<typename T, T a>
            static constexpr T ConstMin() noexcept { return a; };

            template<typename T, T a, T b, T ... args>
            static constexpr T ConstMin() noexcept {
                return (a < b ? ConstMin<T, a, args...>() : ConstMin<T, b, args...>());
            };

        };
    }
}

#endif //BERSERK_UTILS_HPP