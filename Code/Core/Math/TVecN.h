/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TVECN_H
#define BERSERK_TVECN_H

#include <Math/Math.h>
#include <IO/Archive.h>
#include <Platform/Memory.h>

#include <initializer_list>


namespace Berserk {

    /**
     * Generic vector class for an N dimensional space base on type T
     * @tparam T Type of values inside vector
     * @tparam N Vector dimension value
     */
    template <typename T, uint32 N>
    class TVecN {
    public:

        static_assert(N > 0, "Vector size must be more than 0");

        TVecN() noexcept {
            zero();
        }

        TVecN(const std::initializer_list<T> &list) noexcept : TVecN<T,N>() {
            uint32 i = 0;
            for (const auto &a: list) {

                if (i >= N) return;

                values[i] = a;
                i += 1;
            }
        }

        TVecN(const TVecN& other) noexcept {
            for (uint32 i = 0; i < N; i++) {
                values[i] = other.values[i];
            }
        }

        TVecN(TVecN&& other) noexcept {
            for (uint32 i = 0; i < N; i++) {
                values[i] = other.values[i];
            }
        }

        template <uint32 M>
        TVecN(const TVecN<T, M> &other) noexcept : TVecN<T,N>() {
            uint32 i = 0;
            while (i < N && i < M) {
                values[i] = other.values[i];
                i += 1;
            }
        }

        TVecN& operator=(const TVecN& other) {
            Memory::copy(&values[0], &other.values[0], N * sizeof(T));
            return *this;
        }

        TVecN& operator=(TVecN&& other) noexcept {
            Memory::copy(&values[0], &other.values[0], N * sizeof(T));
            return *this;
        }

        TVecN operator+(const TVecN& other) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] + other.values[i];
            }

            return r;
        }

        TVecN operator-(const TVecN& other) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] - other.values[i];
            }

            return r;
        }

        TVecN operator*(const TVecN& other) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] * other.values[i];
            }

            return r;
        }

        TVecN operator/(const TVecN& other) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] / other.values[i];
            }

            return r;
        }

        TVecN operator+(T a) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] + a;
            }

            return r;
        }

        TVecN operator-(T a) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] - a;
            }

            return r;
        }

        TVecN operator*(T a) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] * a;
            }

            return r;
        }

        TVecN operator/(T a) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] / a;
            }

            return r;
        }

        TVecN& operator+=(const TVecN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] += other.values[i];
            }

            return *this;
        }

        TVecN& operator-=(const TVecN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] -= other.values[i];
            }

            return *this;
        }

        TVecN& operator*=(const TVecN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] *= other.values[i];
            }

            return *this;
        }

        TVecN& operator/=(const TVecN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] /= other.values[i];
            }

            return *this;
        }

        TVecN& operator+=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] += a;
            }

            return *this;
        }

        TVecN& operator-=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] -= a;
            }

            return *this;
        }

        TVecN& operator*=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] *= a;
            }

            return *this;
        }

        TVecN& operator/=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] /= a;
            }

            return *this;
        }

        TVecN operator-() const {
            return *this * (-1);
        }

        bool operator<=(const TVecN& other) const {
            bool done = true;

            for (uint32 i = 0; i < N; i++) {
                done = done && (values[i] <= other.values[i]);
            }
            return done;
        }

        bool operator>=(const TVecN& other) const {
            bool done = true;

            for (uint32 i = 0; i < N; i++) {
                done = done && (values[i] >= other.values[i]);
            }
            return done;
        }

        bool operator<(const TVecN& other) const {
            bool done = true;

            for (uint32 i = 0; i < N; i++) {
                done = done && (values[i] < other.values[i]);
            }
            return done;
        }

        bool operator>(const TVecN& other) const {
            bool done = true;

            for (uint32 i = 0; i < N; i++) {
                done = done && (values[i] > other.values[i]);
            }
            return done;
        }

        bool operator==(const TVecN& other) const {
            bool r = true;

            for (uint32 i = 0; i < N; i++) {
                r = r && (values[i] == other.values[i]);
            }

            return r;
        }

        bool operator!=(const TVecN& other) const {
            bool r = false;

            for (uint32 i = 0; i < N; i++) {
                r = r || (values[i] != other.values[i]);
            }

            return r;
        }

        static T distance2(const TVecN& a, const TVecN& b) {
            auto c = a - b;
            return c.length2();
        }

        static T distance(const TVecN& a, const TVecN& b) {
            auto c = a - b;
            return c.length();
        }

        static T dot(const TVecN& a, const TVecN& b) {
            T r = 0;

            for (uint32 i = 0; i < N; i++) {
                r += a.values[i] * b.values[i];
            }

            return r;
        }

        static T angle(const TVecN& a, const TVecN& b) {
            return Math::acos(dot(a.normalized(), b.normalized()));
        }

        /**
         * Vector cross product
         * @warning Defined only fir 3-dim vectors
         * @param a Input vector
         * @param b Input vector
         * @return Vector c, which satisfies: (a, b, c) - its right three
         */
        static TVecN cross(const TVecN& a, const TVecN& b) {
            static_assert(N == 3, "Cross product only defined for 3-dim vectors");

            TVecN<T,3> result;

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
        static T triple(const TVecN& a, const TVecN& b, const TVecN& c) {
            static_assert(N == 3, "Triple product only defined for 3-dim vectors");

            // a b c - tight three
            // a x b - area of figure
            // dot(a x b, c) - volume of the figure

            return dot(cross(a, b), c);
        }

        static TVecN lerp(float t, const TVecN& a, const TVecN& b) {
            TVecN r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::lerp(t, a.values[i], b.values[i]);
            }

            return r;
        }

        static TVecN slerp(T t, const TVecN& a, const TVecN& b) {
            T ang = angle(a, b);

            if (ang <= Math::THRESH_FLOAT32) {
                return lerp(t, a, b);
            }

            TVecN<T,N> r;
            T angleSin = Math::sin(ang);
            T angle1 = Math::sin(ang * (1 - t)) / angleSin;
            T angle2 = Math::sin(ang * t) / angleSin;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = a.values[i] * angle1 + b.values[i] * angle2;
            }

            return r;
        }

        static TVecN slerp(T t, T ang, const TVecN& a, const TVecN& b) {
            if (ang <= Math::THRESH_FLOAT32) {
                return lerp(t, a, b);
            }

            TVecN<T,N> r;
            T angleSin = Math::sin(ang);
            T angle1 = Math::sin(ang * (1 - t)) / angleSin;
            T angle2 = Math::sin(ang * t) / angleSin;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = a.values[i] * angle1 + b.values[i] * angle2;
            }

            return r;
        }

        static TVecN min(const TVecN& a, const TVecN& b) {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::min(a.values[i], b.values[i]);
            }

            return r;
        }

        static TVecN max(const TVecN& a, const TVecN& b) {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::max(a.values[i], b.values[i]);
            }

            return r;
        }

        static TVecN clamp(const TVecN& t, const TVecN& left, const TVecN& right) {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::clamp(t.values[i], left.values[i], right.values[i]);
            }

            return r;
        }

        TVecN abs() const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::abs(values[i]);
            }

            return r;
        }

        TVecN pow(T factor) const {
            TVecN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::pow(values[i], factor);
            }

            return r;
        }

        T length2() const {
            T r = 0;

            for (uint32 i = 0; i < N; i++) {
                r += values[i] * values[i];
            }

            return r;
        }

        T length() const {
            T l2 = length2();
            return Math::sqrt(l2);
        }

        TVecN normalized() const {
            TVecN r = *this;
            return r.normalize();
        }

        TVecN& normalize() {
            T len2 = length2();

            if (len2 <= Math::THRESH_ZERO_NORM_SQUARED) {
                zero();
            }
            else {
                T len = Math::sqrt(len2);
                *this /= len;
            }

            return *this;
        }

        TVecN& zero() {
            for (auto& a: values) {
                a = 0;
            }

            return *this;
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

        T& operator[](uint32 index) { return values[index]; }
        const T& operator[](uint32 index) const { return values[index]; }

        T* getData() { return values; }
        const T* getData() const { return values; }

        friend Archive& operator<<(Archive& archive, const TVecN& ver) {
            for (auto& v: ver.values) {
                archive << v;
            }

            return archive;
        }

        friend Archive& operator>>(Archive& archive, TVecN& ver) {
            for (auto& v: ver.values) {
                archive >> v;
            }

            return archive;
        }

    public:
        T values[N];
    };

}

#endif //BERSERK_TVECN_H