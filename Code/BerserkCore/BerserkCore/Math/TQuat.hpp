/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TQUAT_H
#define BERSERK_TQUAT_H

#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/TMatMxN.hpp>

namespace Berserk {
    namespace Math {

        template<typename T>
        class TQuat {
        public:
            using Vec = TVecN<T, 3>;
            using Mat = TMatMxN<T, 4, 4>;

            TQuat() = default;

            TQuat(T s, const Vec &v) {
                scalar = s;
                vec = v;
            }

            TQuat(T s, T x, T y, T z) {
                scalar = s;
                vec = {x, y, z};
            }

            /** Clockwise rotation around an arbitrary axis */
            TQuat(const Vec &axis, T angle) {
                scalar = Utils::Cos(angle * (T) 0.5);
                vec = axis.Normalized() * Utils::Sin(angle * (T) 0.5);
            }

            TQuat(T roll, T yaw, T pitch) {
                *this = TQuat({1, 0, 0}, roll) *
                        TQuat({0, 1, 0}, yaw) *
                        TQuat({0, 0, 1}, pitch);
            }

            explicit TQuat(const Mat &mat) {
                T q[4]; // notation: x[0] y[1] z[2] w[3]
                auto &v = mat.values;

                T trace = v[0] + v[5] + v[10];

                // Matrix 4x4 indexes
                // 0  1  2  3
                // 4  5  6  7
                // 8  9  10 11
                // 12 13 14 15

                // Check the diagonal
                if (trace > 0.0) {
                    // positive diagonal

                    T s = Utils::Sqrt(trace + 1.0f);
                    q[3] = s * 0.5;

                    T t = 0.5f / s;
                    q[0] = (v[9] - v[6]) * t;
                    q[1] = (v[2] - v[8]) * t;
                    q[2] = (v[4] - v[1]) * t;
                } else {
                    // negative diagonal

                    int32 i = 0;
                    if (v[5] > v[0]) i = 1;
                    if (v[10] > v[4 * i + i]) i = 2;

                    static const int32 NEXT[3] = {1, 2, 0};
                    int32 j = NEXT[i];
                    int32 k = NEXT[j];

                    auto s = (T) Utils::Sqrt(v[i * 4 + i] - (v[j * 4 + j] + v[k * 4 + k]) + 1.0);

                    T t;
                    if (s == 0.0) t = s;
                    else t = 0.5 / s;

                    q[i] = s * 0.5f;
                    q[3] = (v[k * 4 + j] - v[j * 4 + k]) * t;
                    q[j] = (v[j * 4 + i] - v[i * 4 + j]) * t;
                    q[k] = (v[k * 4 + i] - v[i * 4 + k]) * t;
                }

                scalar = q[3];
                vec = {q[0], q[1], q[2]};
            }

            TQuat(const TQuat &other) = default;
            TQuat(TQuat &&other) noexcept = default;
            ~TQuat() = default;

            TQuat &operator=(const TQuat &other) = default;
            TQuat &operator=(TQuat &&other) noexcept = default;

            void Identity() {
                scalar = 1;
                vec.Zero();
            }

            void Zero() {
                scalar = 0;
                vec.Zero();
            }

            TQuat operator*(const TQuat &other) const {
                auto s = other.scalar;
                auto v = other.vec;

                return TQuat(scalar * s - Vec::Dot(vec, v), v * scalar + vec * s + Vec::Cross(vec, v));
            }

            TQuat &operator*=(T a) {
                scalar /= a;
                vec /= a;
                return *this;
            }

            TQuat &operator/=(T a) {
                scalar /= a;
                vec /= a;
                return *this;
            }

            T Length2() const {
                return scalar * scalar + vec.Length2();
            }

            T Length() const {
                T l2 = Length2();
                return Utils::Sqrt(l2);
            }

            TQuat Normalized() const {
                TQuat r = *this;
                return r.Normalize();
            }

            TQuat &Normalize() {
                T len2 = Length2();

                if (len2 <= Utils::THRESH_ZERO_NORM_SQUARED) {
                    Zero();
                } else {
                    T len = Utils::Sqrt(len2);
                    *this /= len;
                }

                return *this;
            }

            TQuat Inverse() const {
                T len2 = Length2();

                if (len2 <= Utils::THRESH_ZERO_NORM_SQUARED) {
                    TQuat q;
                    q.Zero();
                    return q;
                } else {
                    T len = Utils::Sqrt(len2);
                    return TQuat(scalar / len, -vec / len);
                }
            }

            TQuat Conjugate() const {
                return TQuat(scalar, -vec);
            }

            Vec Rotate(const Vec &v) const {
                TQuat q(0, v);
                TQuat r = *this * q * Conjugate();
                return r.vec;
            }

            Vec RotateReverse(const Vec &v) const {
                TQuat q(0, v);
                TQuat r = Conjugate() * q * *this;
                return r.vec;
            }

            Vec GetAxisX() const {
                return Rotate({1, 0, 0});
            }

            Vec GetAxisY() const {
                return Rotate({0, 1, 0});
            }

            Vec GetAxisZ() const {
                return Rotate({0, 0, 1});
            }

            T GetAngle() const {
                return (T) 2 * Utils::Acos(scalar);
            }

            Mat AsMatrix() const {
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
                auto &v = r.values;

                v[0] = 1 - 2 * (yy + zz);
                v[1] = 2 * (xy - zw);
                v[2] = 2 * (xz + yw);
                v[3] = 0;
                v[4] = 2 * (xy + zw);
                v[5] = 1 - 2 * (xx + zz);
                v[6] = 2 * (yz - xw);
                v[7] = 0;
                v[8] = 2 * (xz - yw);
                v[9] = 2 * (yz + xw);
                v[10] = 1 - 2 * (xx + yy);
                v[11] = 0;
                v[12] = 0;
                v[13] = 0;
                v[14] = 0;
                v[15] = 1;

                return r;
            }

            void GetAxisAngle(Vec &axis, T &angle) const {
                angle = (T) 2 * Utils::Acos(scalar);
                T s = Utils::Max((T) (1.0 - scalar * scalar), (T) 0.0);

                if (s > Utils::THRESH_ZERO_NORM_SQUARED)
                    axis = vec / Utils::Sqrt(s);
                else
                    axis = vec({0, 1, 0});
            }

            uint32 Hash() const {
                Crc32Builder builder;
                builder.Hash(&vec, sizeof(vec));
                builder.Hash(&scalar, sizeof(scalar));
                return builder.GetHash();
            }

            bool operator==(const TQuat &other) const {
                return vec == other.vec && scalar == other.scalar;
            }

            bool operator!=(const TQuat &other) const {
                return vec != other.vec || scalar != other.scalar;
            }

            /** Clockwise rotation around an arbitrary axis */
            static TQuat Rotation(const Vec &axis, T angle) {
                return TQuat(axis, angle);
            }

            static T Dot(const TQuat &a, const TQuat &b) {
                return a.scalar * b.scalar + Vec::Dot(a.vec, b.vec);
            }

            static T Angle(const TQuat &a, const TQuat &b) {
                return Utils::Acos(Dot(a.Normalized(), b.Normalized()));
            }

            static TQuat Lerp(T t, const TQuat &a, const TQuat &b) {
                TQuat q;
                q.scalar = Utils::Lerp(t, a.scalar, b.scalar);
                q.vec = Vec::Lerp(t, a.vec, b.vec);
                return q;
            }

            static TQuat Slerp(T t, const TQuat &a, const TQuat &b) {
                T ang = angle(a, b);

                if (ang <= Utils::THRESH_FLOAT32) {
                    return lerp(t, a, b);
                }

                TQuat r;
                T angleSin = Utils::Sin(ang);
                T angle1 = Utils::Sin(ang * (1 - t)) / angleSin;
                T angle2 = Utils::Sin(ang * t) / angleSin;

                r.scalar = a.scalar * angle1 + b.scalar * angle2;
                r.vec = a.vec * angle1 + b.vec * angle2;

                return r;
            }

            static TQuat Slerp(T t, T ang, const TQuat &a, const TQuat &b) {
                if (ang <= Utils::THRESH_FLOAT32) {
                    return Lerp(t, a, b);
                }

                TQuat r;
                T angleSin = Utils::Sin(ang);
                T angle1 = Utils::Sin(ang * (1 - t)) / angleSin;
                T angle2 = Utils::Sin(ang * t) / angleSin;

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
            static TQuat LookAt(const TVecN<T, 3> &direction, const TVecN<T, 3> &up) {
                auto Z = (-direction).Normalized();
                auto X = TVecN<T, 3>::Cross(up, Z).Normalized();
                auto Y = TVecN<T, 3>::Cross(Z, X);

                TMatMxN<T, 4, 4> m;

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
             * @param up Where is up after rotation (rotates y)
             * @return Rotation quaternion
             */
            static TQuat FaceAt(const TVecN<T, 3> &direction, const TVecN<T, 3> &up) {
                auto Z = direction.Normalized();
                auto X = TVecN<T, 3>::Cross(up, Z).Normalized();
                auto Y = TVecN<T, 3>::Cross(Z, X);

                TMatMxN<T, 4, 4> m;

                for (uint32 i = 0; i < 3; i++) {
                    m.values[0 + i] = X[i];
                    m.values[4 + i] = Y[i];
                    m.values[8 + i] = Z[i];
                }

                return TQuat(m).Inverse();
            }

        public:
            T scalar = 1;
            Vec vec;
        };

        using Quatf = TQuat<float>;
    }

    template<typename T>
    class Equals<Math::TQuat<T>> {
    public:
        bool operator()(const Math::TQuat<T>& a, const Math::TQuat<T>& b) const {
            return a == b;
        }
    };

    template<typename T>
    class Hash<Math::TQuat<T>> {
    public:
        bool operator()(const Math::TQuat<T>& a) const {
            return a.Hash();
        }
    };

    template<typename T>
    class FormatPrint<Math::TQuat<T>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Math::TQuat<T>& a) const {
            FormatPrint<T> formatPrintT;

            stream.Add(BERSERK_TEXT("("));
            formatPrintT(stream, a.scalar);
            stream.Add(BERSERK_TEXT(","));
            formatPrintT(stream, a.vec[0]);
            stream.Add(BERSERK_TEXT(","));
            formatPrintT(stream, a.vec[1]);
            stream.Add(BERSERK_TEXT(","));
            formatPrintT(stream, a.vec[2]);
            stream.Add(BERSERK_TEXT(")"));
        }
    };
}

#endif //BERSERK_TQUAT_H