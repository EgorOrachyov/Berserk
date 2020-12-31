/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TVECN_H
#define BERSERK_TVECN_H

#include <BerserkCore/Math/Utils.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Misc/Contracts.hpp>
#include <BerserkCore/Misc/Crc32.hpp>
#include <initializer_list>

namespace Berserk {
    namespace Math {

        /**
         * Generic vector class for an N dimensional space base on type T
         *
         * @tparam T Type of values inside vector
         * @tparam N Vector dimension value
         */
        template<typename T, uint32 N>
        class TVecN {
        public:

            static_assert(N > 0, "Vector size must be more than 0");

            TVecN() noexcept {
                Zero();
            }

            TVecN(T x) : TVecN() {
                static_assert(N >= 1, "Out of bounds index assignment");
                values[0] = x;
            }

            TVecN(T x, T y) : TVecN() {
                static_assert(N >= 2, "Out of bounds index assignment");
                values[0] = x;
                values[1] = y;
            }

            TVecN(T x, T y, T z) : TVecN() {
                static_assert(N >= 3, "Out of bounds index assignment");
                values[0] = x;
                values[1] = y;
                values[2] = z;
            }

            TVecN(T x, T y, T z, T w) : TVecN() {
                static_assert(N >= 4, "Out of bounds index assignment");
                values[0] = x;
                values[1] = y;
                values[2] = z;
                values[3] = w;
            }

            template<uint32 M>
            TVecN(const TVecN<T, M> &v, T a) : TVecN() {
                static_assert(N >= M + 1, "Out of bounds index assignment");
                Platform::Memory::Copy(&values[0], &v.values[0], sizeof(T) * M);
                values[M] = a;
            }

            TVecN(const std::initializer_list<T> &list) noexcept: TVecN<T, N>() {
                uint32 i = 0;
                for (const auto &a: list) {

                    if (i >= N) return;

                    values[i] = a;
                    i += 1;
                }
            }

            TVecN(const TVecN &other) noexcept {
                for (uint32 i = 0; i < N; i++) {
                    values[i] = other.values[i];
                }
            }

            TVecN(TVecN &&other) noexcept {
                for (uint32 i = 0; i < N; i++) {
                    values[i] = other.values[i];
                }
            }

            template<uint32 M>
            TVecN(const TVecN<T, M> &other) noexcept : TVecN<T, N>() {
                uint32 i = 0;
                while (i < N && i < M) {
                    values[i] = other.values[i];
                    i += 1;
                }
            }

            TVecN &operator=(const TVecN &other) {
                Platform::Memory::Copy(&values[0], &other.values[0], N * sizeof(T));
                return *this;
            }

            TVecN &operator=(TVecN &&other) noexcept {
                Platform::Memory::Copy(&values[0], &other.values[0], N * sizeof(T));
                return *this;
            }

            TVecN operator+(const TVecN &other) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] + other.values[i];
                }

                return r;
            }

            TVecN operator-(const TVecN &other) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] - other.values[i];
                }

                return r;
            }

            TVecN operator*(const TVecN &other) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] * other.values[i];
                }

                return r;
            }

            TVecN operator/(const TVecN &other) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] / other.values[i];
                }

                return r;
            }

            TVecN operator+(T a) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] + a;
                }

                return r;
            }

            TVecN operator-(T a) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] - a;
                }

                return r;
            }

            TVecN operator*(T a) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] * a;
                }

                return r;
            }

            TVecN operator/(T a) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = values[i] / a;
                }

                return r;
            }

            TVecN &operator+=(const TVecN &other) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] += other.values[i];
                }

                return *this;
            }

            TVecN &operator-=(const TVecN &other) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] -= other.values[i];
                }

                return *this;
            }

            TVecN &operator*=(const TVecN &other) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] *= other.values[i];
                }

                return *this;
            }

            TVecN &operator/=(const TVecN &other) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] /= other.values[i];
                }

                return *this;
            }

            TVecN &operator+=(T a) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] += a;
                }

                return *this;
            }

            TVecN &operator-=(T a) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] -= a;
                }

                return *this;
            }

            TVecN &operator*=(T a) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] *= a;
                }

                return *this;
            }

            TVecN &operator/=(T a) {
                for (uint32 i = 0; i < N; i++) {
                    values[i] /= a;
                }

                return *this;
            }

            TVecN operator-() const {
                return *this * (-1);
            }

            bool operator<=(const TVecN &other) const {
                bool done = true;

                for (uint32 i = 0; i < N; i++) {
                    done = done && (values[i] <= other.values[i]);
                }
                return done;
            }

            bool operator>=(const TVecN &other) const {
                bool done = true;

                for (uint32 i = 0; i < N; i++) {
                    done = done && (values[i] >= other.values[i]);
                }
                return done;
            }

            bool operator<(const TVecN &other) const {
                bool done = true;

                for (uint32 i = 0; i < N; i++) {
                    done = done && (values[i] < other.values[i]);
                }
                return done;
            }

            bool operator>(const TVecN &other) const {
                bool done = true;

                for (uint32 i = 0; i < N; i++) {
                    done = done && (values[i] > other.values[i]);
                }
                return done;
            }

            bool operator==(const TVecN &other) const {
                bool r = true;

                for (uint32 i = 0; i < N; i++) {
                    r = r && (values[i] == other.values[i]);
                }

                return r;
            }

            bool operator!=(const TVecN &other) const {
                bool r = false;

                for (uint32 i = 0; i < N; i++) {
                    r = r || (values[i] != other.values[i]);
                }

                return r;
            }

            static T Distance2(const TVecN &a, const TVecN &b) {
                auto c = a - b;
                return c.Length2();
            }

            static T Distance(const TVecN &a, const TVecN &b) {
                auto c = a - b;
                return c.Length();
            }

            static T Dot(const TVecN &a, const TVecN &b) {
                T r = 0;

                for (uint32 i = 0; i < N; i++) {
                    r += a.values[i] * b.values[i];
                }

                return r;
            }

            static T Angle(const TVecN &a, const TVecN &b) {
                return Utils::Acos(Dot(a.Normalized(), b.Normalized()));
            }

            /**
             * Vector cross product
             * @warning Defined only fir 3-dim vectors
             * @param a Input vector
             * @param b Input vector
             * @return Vector c, which satisfies: (a, b, c) - its right three
             */
            static TVecN Cross(const TVecN &a, const TVecN &b) {
                static_assert(N == 3, "Cross product only defined for 3-dim vectors");

                TVecN<T, 3> result;

                // a x b = det | i  j  k  |
                //             | x1 y1 z1 |
                //             | x2 y2 z2 |

                result[0] = a.y() * b.z() - a.z() * b.y();
                result[1] = a.z() * b.x() - a.x() * b.z();
                result[2] = a.x() * b.y() - a.y() * b.x();

                return result;
            }

            /**
             * Triple vector product - volume of the a figure with sign:
             * Positive volume if (a,b,c) - right-system three
             * @param a Input vector
             * @param b Input vector
             * @param c Input vector
             * @return Signed volume
             */
            static T Triple(const TVecN &a, const TVecN &b, const TVecN &c) {
                static_assert(N == 3, "Triple product only defined for 3-dim vectors");

                // a b c - tight three
                // a x b - area of figure
                // dot(a x b, c) - volume of the figure

                return Dot(Cross(a, b), c);
            }

            static TVecN Lerp(float t, const TVecN &a, const TVecN &b) {
                TVecN r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = Utils::Lerp(t, a.values[i], b.values[i]);
                }

                return r;
            }

            static TVecN Slerp(T t, const TVecN &a, const TVecN &b) {
                T ang = Angle(a, b);

                if (ang <= Utils::THRESH_FLOAT32) {
                    return Lerp(t, a, b);
                }

                TVecN<T, N> r;
                T angleSin = Utils::Sin(ang);
                T angle1 = Utils::Sin(ang * (1 - t)) / angleSin;
                T angle2 = Utils::Sin(ang * t) / angleSin;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = a.values[i] * angle1 + b.values[i] * angle2;
                }

                return r;
            }

            static TVecN Slerp(T t, T ang, const TVecN &a, const TVecN &b) {
                if (ang <= Utils::THRESH_FLOAT32) {
                    return Lerp(t, a, b);
                }

                TVecN<T, N> r;
                T angleSin = Utils::Sin(ang);
                T angle1 = Utils::Sin(ang * (1 - t)) / angleSin;
                T angle2 = Utils::Sin(ang * t) / angleSin;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = a.values[i] * angle1 + b.values[i] * angle2;
                }

                return r;
            }

            static TVecN Min(const TVecN &a, const TVecN &b) {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = Utils::Min(a.values[i], b.values[i]);
                }

                return r;
            }

            static TVecN Max(const TVecN &a, const TVecN &b) {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = Utils::Max(a.values[i], b.values[i]);
                }

                return r;
            }

            static TVecN Clamp(const TVecN &t, const TVecN &left, const TVecN &right) {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = Utils::Clamp(t.values[i], left.values[i], right.values[i]);
                }

                return r;
            }

            TVecN Abs() const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = Utils::Abs(values[i]);
                }

                return r;
            }

            TVecN Pow(T factor) const {
                TVecN<T, N> r;

                for (uint32 i = 0; i < N; i++) {
                    r.values[i] = Utils::Pow(values[i], factor);
                }

                return r;
            }

            T Length2() const {
                T r = 0;

                for (uint32 i = 0; i < N; i++) {
                    r += values[i] * values[i];
                }

                return r;
            }

            T Length() const {
                T l2 = Length2();
                return Utils::Sqrt(l2);
            }

            TVecN Normalized() const {
                TVecN r = *this;
                return r.NormalizeThis();
            }

            TVecN &NormalizeThis() {
                T len2 = Length2();

                if (len2 <= Utils::THRESH_ZERO_NORM_SQUARED) {
                    Zero();
                } else {
                    T len = Utils::Sqrt(len2);
                    *this /= len;
                }

                return *this;
            }

            TVecN &Zero() {
                for (auto &a: values) {
                    a = 0;
                }

                return *this;
            }

            uint32 Hash() const {
                Crc32Builder builder;

                for (const auto& v: values) {
                    builder.Hash(&v, sizeof(T));
                }

                return builder.GetHash();
            }

            T x() const {
                static_assert(N >= 1, "No component");
                return values[0];
            }

            T y() const {
                static_assert(N >= 2, "No component");
                return values[1];
            }

            T z() const {
                static_assert(N >= 3, "No component");
                return values[2];
            }

            T w() const {
                static_assert(N >= 4, "No component");
                return values[3];
            }

            T &operator[](uint32 index) { return values[index]; }
            const T &operator[](uint32 index) const { return values[index]; }

            T *GetData() { return values; }
            const T *GetData() const { return values; }

            static TVecN AxisX() {
                return TVecN<T,N>(1);
            }
            static TVecN AxisY() {
                return TVecN<T,N>(0, 1);
            }
            static TVecN AxisZ() {
                return TVecN<T,N>(0, 0, 1);
            }

        public:
            T values[N];
        };

        using Vec2f = TVecN<float, 2>;
        using Vec3f = TVecN<float, 3>;
        using Vec4f = TVecN<float, 4>;

        using Point2i = TVecN<int32, 2>;
        using Size2i = TVecN<int32, 2>;
        using Rect2i = TVecN<int32, 4>;
    }

    template<typename T, uint32 N>
    class Equals<Math::TVecN<T, N>> {
    public:
        bool operator()(const Math::TVecN<T, N>& a, const Math::TVecN<T, N>& b) const {
            return a == b;
        }
    };

    template<typename T, uint32 N>
    class Hash<Math::TVecN<T, N>> {
    public:
        bool operator()(const Math::TVecN<T, N>& a) const {
            return a.Hash();
        }
    };

    template<typename T, uint32 N>
    class TextPrint<Math::TVecN<T, N>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Math::TVecN<T, N>& a) const {
            TextPrint<T> formatPrintT;

            stream.Add(BERSERK_TEXT("("));
            formatPrintT(stream, a.values[0]);

            for (uint32 i = 1; i < N; i++) {
                stream.Add(BERSERK_TEXT(","));
                formatPrintT(stream, a.values[i]);
            }

            stream.Add(BERSERK_TEXT(")"));
        }
    };
}

#endif //BERSERK_TVECN_H