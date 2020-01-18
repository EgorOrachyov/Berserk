/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TVECTORN_H
#define BERSERK_TVECTORN_H

#include <Math/Math.h>
#include <Platform/Memory.h>

#include <initializer_list>


namespace Berserk {

    /**
     * Generic vector class for an N dimensional space on base type T
     * @tparam T Type of values inside vector
     * @tparam N Vector dimension value
     */
    template <typename T, uint32 N>
    class TVectorN {
    public:

        static_assert(N > 0, "Vector size must be more than 0");

        TVectorN() noexcept {
            zero();
        }

        TVectorN(const std::initializer_list<T> &list) noexcept : TVectorN<T,N>() {
            uint32 i = 0;
            for (auto &a: list) {

                if (i >= N) return;

                values[i] = a;
                i += 1;
            }
        }

        TVectorN(const TVectorN& other) noexcept {
            for (uint32 i = 0; i < N; i++) {
                values[i] = other.values[i];
            }
        }

        TVectorN(TVectorN&& other) noexcept {
            for (uint32 i = 0; i < N; i++) {
                values[i] = other.values[i];
            }
        }

        template <uint32 M>
        TVectorN(const TVectorN<T, M> &other) noexcept : TVectorN<T,N>() {
            uint32 i = 0;
            while (i < N && i < M) {
                values[i] = other.values[i];
                i += 1;
            }
        }

        TVectorN& operator=(const TVectorN& other) {
            Memory::copy(&values[0], &other.values[0], N * sizeof(T));
            return *this;
        }

        TVectorN operator+(const TVectorN& other) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] + other.values[i];
            }

            return r;
        }

        TVectorN operator-(const TVectorN& other) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] - other.values[i];
            }

            return r;
        }

        TVectorN operator*(const TVectorN& other) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] * other.values[i];
            }

            return r;
        }

        TVectorN operator/(const TVectorN& other) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] / other.values[i];
            }

            return r;
        }

        TVectorN operator+(T a) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] + a;
            }

            return r;
        }

        TVectorN operator-(T a) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] - a;
            }

            return r;
        }

        TVectorN operator*(T a) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] * a;
            }

            return r;
        }

        TVectorN operator/(T a) const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = values[i] / a;
            }

            return r;
        }

        TVectorN& operator+=(const TVectorN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] += other.values[i];
            }

            return *this;
        }

        TVectorN& operator-=(const TVectorN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] -= other.values[i];
            }

            return *this;
        }

        TVectorN& operator*=(const TVectorN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] *= other.values[i];
            }

            return *this;
        }

        TVectorN& operator/=(const TVectorN& other) {
            for (uint32 i = 0; i < N; i++) {
                values[i] /= other.values[i];
            }

            return *this;
        }

        TVectorN& operator+=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] += a;
            }

            return *this;
        }

        TVectorN& operator-=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] -= a;
            }

            return *this;
        }

        TVectorN& operator*=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] *= a;
            }

            return *this;
        }

        TVectorN& operator/=(T a) {
            for (uint32 i = 0; i < N; i++) {
                values[i] /= a;
            }

            return *this;
        }

        bool operator==(const TVectorN& other) const {
            bool r = true;

            for (uint32 i = 0; i < N; i++) {
                r = r && (values[i] == other.values[i]);
            }

            return r;
        }

        bool operator!=(const TVectorN& other) const {
            bool r = false;

            for (uint32 i = 0; i < N; i++) {
                r = r || (values[i] != other.values[i]);
            }

            return r;
        }

        static T distance2(const TVectorN& a, const TVectorN& b) {
            auto c = a - b;
            return c.length2();
        }

        static T distance(const TVectorN& a, const TVectorN& b) {
            auto c = a - b;
            return c.length();
        }

        static T dot(const TVectorN& a, const TVectorN& b) {
            T r = 0;

            for (uint32 i = 0; i < N; i++) {
                r += a.values[i] * b.values[i];
            }

            return r;
        }

        static T angle(const TVectorN& a, const TVectorN& b) {
            return Math::acos(dot(a.normalized(), b.normalized()));
        }

        /**
         * Vector cross product
         * @warning Defined only fir 3-dim vectors
         * @param a Input vector
         * @param b Input vector
         * @return Vector c, which satisfies: (a, b, c) - its right three
         */
        static TVectorN cross(const TVectorN& a, const TVectorN& b) {
            static_assert(N == 3, "Cross product only defined for 3-dim vectors");

            TVectorN<T,3> result;

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
        static T triple(const TVectorN& a, const TVectorN& b, const TVectorN& c) {
            static_assert(N == 3, "Triple product only defined for 3-dim vectors");

            // a b c - tight three
            // a x b - area of figure
            // dot(a x b, c) - volume of the figure

            return dot(cross(a, b), c);
        }

        static TVectorN lerp(float32 t, const TVectorN& a, const TVectorN& b) {
            TVectorN r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::lerp(t, a.values[i], b.values[i]);
            }

            return r;
        }

        static TVectorN slerp(T t, const TVectorN& a, const TVectorN& b) {
            T ang = angle(a, b);

            if (ang <= Math::THRESH_FLOAT32) {
                return lerp(t, a, b);
            }

            TVectorN<T,N> r;
            T angleSin = Math::sin(ang);

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = a.values[i] * (Math::sin(ang * (1 - t)) / angleSin) + b.values[i] * (Math::sin(ang * t) / angleSin);
            }

            return r;
        }

        static TVectorN slerp(T t, T ang, const TVectorN& a, const TVectorN& b) {
            if (ang <= Math::THRESH_FLOAT32) {
                return lerp(t, a, b);
            }

            TVectorN<T,N> r;
            T angleSin = Math::sin(ang);

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = a.values[i] * (Math::sin(ang * (1 - t)) / angleSin) + b.values[i] * (Math::sin(ang * t) / angleSin);
            }

            return r;
        }

        static TVectorN min(const TVectorN& a, const TVectorN& b) {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::min(a.values[i], b.values[i]);
            }

            return r;
        }

        static TVectorN max(const TVectorN& a, const TVectorN& b) {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::max(a.values[i], b.values[i]);
            }

            return r;
        }

        static TVectorN clamp(const TVectorN& t, const TVectorN& left, const TVectorN& right) {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::clamp(t.values[i], left.values[i], right.values[i]);
            }

            return r;
        }

        TVectorN abs() const {
            TVectorN<T,N> r;

            for (uint32 i = 0; i < N; i++) {
                r.values[i] = Math::abs(values[i]);
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

        TVectorN normalized() const {
            TVectorN r = *this;
            return r.normalize();
        }

        TVectorN& normalize() {
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

        TVectorN& zero() {
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

    public:
        T values[N];

    };

}

#endif //BERSERK_TVECTORN_H