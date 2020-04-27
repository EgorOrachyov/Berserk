/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TQUAT_H
#define BERSERK_TQUAT_H

#include <Math/TVecN.h>
#include <Math/TMatMxN.h>

namespace Berserk {

    template <typename T>
    class TQuat {
    public:
        using Vec = TVecN<T,3>;
        using Mat = TMatMxN<T,4,4>;

        TQuat() = default;

        TQuat(T s, const Vec& v) {
            scalar = s;
            vec = v;
        }

        TQuat(T s, T x, T y, T z) {
            scalar = s;
            vec = { x, y, z };
        }

        /** Clockwise rotation around an arbitrary axis */
        TQuat(const Vec& axis, T angle) {
            scalar = Math::cos(angle * (T)0.5);
            vec = axis.normalized() * Math::sin(angle * (T)0.5);
        }

        TQuat(T roll, T yaw, T pitch) {
            *this = TQuat({1,0,0}, roll) *
                    TQuat({0,1,0}, yaw)  *
                    TQuat({0,0,1}, pitch);
        }

        TQuat(const Mat& mat) {
            T q[4]; // notation: x[0] y[1] z[2] w[3]
            auto& v = mat.values;

            T trace = v[0] + v[5] + v[10];

            // Matrix 4x4 indexes
            // 0  1  2  3
            // 4  5  6  7
            // 8  9  10 11
            // 12 13 14 15

            // Check the diagonal
            if (trace > 0.0)
            {
                // positive diagonal

                T s = Math::sqrt(trace + 1.0f);
                q[3] = s * 0.5;

                T t = 0.5f / s;
                q[0] = (v[9] - v[6]) * t;
                q[1] = (v[2] - v[8]) * t;
                q[2] = (v[4] - v[1]) * t;
            }
            else
            {
                // negative diagonal

                int32 i = 0;
                if (v[5] > v[0]) i = 1;
                if (v[10] > v[4 * i + i]) i = 2;

                static const int32 NEXT[3] = {1, 2, 0};
                int32 j = NEXT[i];
                int32 k = NEXT[j];

                auto s = (T) Math::sqrt(v[i * 4 + i] - (v[j * 4 + j] + v[k * 4 + k]) + 1.0);

                T t;
                if (s == 0.0) t = s;
                else t = 0.5 / s;

                q[i] = s * 0.5f;
                q[3] = (v[k * 4 + j] - v[j * 4 + k]) * t;
                q[j] = (v[j * 4 + i] - v[i * 4 + j]) * t;
                q[k] = (v[k * 4 + i] - v[i * 4 + k]) * t;
            }

            scalar = q[3];
            vec = {q[0],q[1],q[2]};
        }

        TQuat(const TQuat& other) = default;

        TQuat(TQuat&& other) noexcept = default;

        ~TQuat() = default;

        TQuat &operator=(const TQuat& other) = default;

        TQuat &operator=(TQuat&& other) noexcept = default;

        void identity() {
            scalar = 1;
            vec.zero();
        }

        void zero() {
            scalar = 0;
            vec.zero();
        }

        TQuat operator*(const TQuat& other) const {
            auto s = other.scalar;
            auto v = other.vec;

            return TQuat(scalar * s - Vec::dot(vec, v), v * scalar + vec * s + Vec::cross(vec, v));
        }

        TQuat& operator*=(T a) {
            scalar /= a;
            vec /= a;
            return *this;
        }

        TQuat& operator/=(T a) {
            scalar /= a;
            vec /= a;
            return *this;
        }

        T length2() const {
            return scalar * scalar + vec.length2();
        }

        T length() const {
            T l2 = length2();
            return Math::sqrt(l2);
        }

        TQuat normalized() const {
            TQuat r = *this;
            return r.normalize();
        }

        TQuat& normalize() {
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

        TQuat inverse() const {
            T len2 = length2();

            if (len2 <= Math::THRESH_ZERO_NORM_SQUARED) {
                TQuat q;
                q.zero();
                return q;
            }
            else {
                T len = Math::sqrt(len2);
                return TQuat(scalar/len, -vec/len);
            }
        }

        TQuat conjugate() const {
            return TQuat(scalar, -vec);
        }

        Vec rotate(const Vec& v) const {
            TQuat q(0, v);
            TQuat r = *this * q * conjugate();
            return r.vec;
        }

        Vec rotateReverse(const Vec& v) const {
            TQuat q(0, v);
            TQuat r = conjugate() * q * *this;
            return r.vec;
        }

        Vec getAxisX() const {
            return rotate({1,0,0});
        }

        Vec getAxisY() const {
            return rotate({0,1,0});
        }

        Vec getAxisZ() const {
            return rotate({0,0,1});
        }

        T getAngle() const {
            return (T)2 * Math::acos(scalar);
        }

        Mat asMatrix() const {
            T x = vec[0];
            T y = vec[1];
            T z = vec[2];

            T xx = x * x;
            T xy = x * y;
            T xz = x * z;
            T xw = x * scalar;

            T yy = y * y;
            T yz = y * z;
            T yw = y * scalar;

            T zz = z * z;
            T zw = z * scalar;

            Mat r;
            auto& v = r.values;

            v[0] = 1 - 2 * (yy + zz); v[1] = 2 * (xy - zw);     v[2] = 2 * (xz + yw);      v[3] = 0;
            v[4] = 2 * (xy + zw);     v[5] = 1 - 2 * (xx + zz); v[6] = 2 * (yz - xw);      v[7] = 0;
            v[8] = 2 * (xz - yw);     v[9] = 2 * (yz + xw);     v[10] = 1 - 2 * (xx + yy); v[11] = 0;
            v[12] = 0;                v[13] = 0;                v[14] = 0;                 v[15] = 1;

            return r;
        }

        void getAxisAngle(Vec& axis, T& angle) const {
            angle = (T)2 * Math::acos(scalar);
            T s = Math::max((T)(1.0 - scalar * scalar), (T)0.0);

            if (s > Math::THRESH_ZERO_NORM_SQUARED)
                axis = vec / Math::sqrt(s);
            else
                axis = vec({0,1,0});
        }

        /** Clockwise rotation around an arbitrary axis */
        static TQuat rotation(const Vec& axis, T angle) {
            return TQuat(axis,angle);
        }

        static T dot(const TQuat& a, const TQuat& b) {
            return a.scalar * b.scalar + Vec::dot(a.vec,b.vec);
        }

        static T angle(const TQuat& a, const TQuat& b) {
            return  Math::acos(dot(a.normalized(), b.normalized()));
        }

        static TQuat lerp(T t, const TQuat& a, const TQuat& b) {
            TQuat q;
            q.scalar = Math::lerp(t, a.scalar, b.scalar);
            q.vec = Vec::lerp(t, a.vec, b.vec);
            return q;
        }

        static TQuat slerp(T t, const TQuat& a, const TQuat& b) {
            T ang = angle(a, b);

            if (ang <= Math::THRESH_FLOAT32) {
                return lerp(t, a, b);
            }

            TQuat r;
            T angleSin = Math::sin(ang);
            T angle1 = Math::sin(ang * (1 - t)) / angleSin;
            T angle2 = Math::sin(ang * t) / angleSin;

            r.scalar = a.scalar * angle1 + b.scalar * angle2;
            r.vec = a.vec * angle1 + b.vec * angle2;

            return r;
        }

        static TQuat slerp(T t, T ang, const TQuat& a, const TQuat& b) {
            if (ang <= Math::THRESH_FLOAT32) {
                return lerp(t, a, b);
            }

            TQuat r;
            T angleSin = Math::sin(ang);
            T angle1 = Math::sin(ang * (1 - t)) / angleSin;
            T angle2 = Math::sin(ang * t) / angleSin;

            r.scalar = a.scalar * angle1 + b.scalar * angle2;
            r.vec = a.vec * angle1 + b.vec * angle2;

            return r;
        }

        /**
        *      | y
        *      |
        *      |_____ x
        *     /
        *    /
        *   /z
        *
        * Look at view quaternion for camera (in OpenGL style)
        * @note Final are is located in the negative z space
        * @param direction Direction vector of viewing
        * @param up Up vector to define orientation
        * @return Look quaternion to rotate world to the View space
        */
        static TQuat lookAt(const TVecN<T,3>& direction, const TVecN<T,3>& up) {
            auto Z = (-direction).normalized();
            auto X = TVecN<T,3>::cross(up, Z).normalized();
            auto Y = TVecN<T,3>::cross(Z, X);

            TMatMxN<T,4,4> m;

            for (int i = 0; i < 3; i++) {
                m.values[0 + i] = X[i];
                m.values[4 + i] = Y[i];
                m.values[8 + i] = Z[i];
            }

            return TQuat(m);
        }

        /**
         *      | y
         *      |
         *      |_____ x
         *     /
         *    /
         *   /z
         *
         * Rotation to orient an object with direction and up vectors
         * @param direction Where to look up (rotates z to this dir)
         * @param up Where is up aftre rotation (rotates y)
         * @return Rotation quaternion
         */
        static TQuat faceAt(const TVecN<T,3>& direction, const TVecN<T,3>& up) {
            auto Z = direction.normalized();
            auto X = TVecN<T,3>::cross(up, Z).normalized();
            auto Y = TVecN<T,3>::cross(Z, X);

            TMatMxN<T,4,4> m;

            for (uint32 i = 0; i < 3; i++) {
                m.values[0 + i] = X[i];
                m.values[4 + i] = Y[i];
                m.values[8 + i] = Z[i];
            }

            return TQuat(m).inverse();
        }
        
    public:
        T scalar = 1;
        Vec vec;
    };

}

#endif //BERSERK_TQUAT_H