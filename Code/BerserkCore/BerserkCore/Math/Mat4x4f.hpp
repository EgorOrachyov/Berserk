/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MAT4X4F_H
#define BERSERK_MAT4X4F_H

#include <BerserkCore/Math/TMatMxN.hpp>

namespace Berserk {

    class Mat4x4f : public TMatMxN<float,4,4> {
    public:
        using TMatMxN<float,4,4>::TMatMxN;
        using TMatMxN<float,4,4>::operator=;
        using Vec3f = TVecN<float,3>;

        Mat4x4f(const TMatMxN<float,4,4>& m) : TMatMxN<float,4,4>(m) { }

        Mat4x4f(float m_00,float m_01,float m_02,float m_03,
                float m_10,float m_11,float m_12,float m_13,
                float m_20,float m_21,float m_22,float m_23,
                float m_30,float m_31,float m_32,float m_33) noexcept {
            values[0] = m_00;  values[1] = m_01;  values[2] = m_02;  values[3] = m_03;
            values[4] = m_10;  values[5] = m_11;  values[6] = m_12;  values[7] = m_13;
            values[8] = m_20;  values[9] = m_21;  values[10] = m_22; values[11] = m_23;
            values[12] = m_30; values[13] = m_31; values[14] = m_32; values[15] = m_33;
        }

        TVecN<float,3> multiply(const TVecN<float,3>& v) const {
            TVecN<float,4> t;
            t[0] = v[0];
            t[1] = v[1];
            t[2] = v[2];
            t[3] = 1.0f;

            return *this * t;
        }

        /** Identity matrix e */
        static Mat4x4f identity() {
            return Mat4x4f(
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            );
        }

        /** Set scale elements */
        static void setScale(Mat4x4f& m, const Vec3f& scale) {
            m.values[0] = scale[0];
            m.values[5] = scale[1];
            m.values[10] = scale[2];
        }

        static Mat4x4f scale(const Vec3f& scale) {
            return Mat4x4f(
                scale[0], 0, 0, 0,
                0, scale[1], 0, 0,
                0, 0, scale[2], 0,
                0 ,0 ,0, 1
            );
        }

        /** Set translation column of matrix m to vec t */
        static void setTranslation(Mat4x4f &m, const Vec3f &t) {
            m.values[3]  = t.values[0];
            m.values[7]  = t.values[1];
            m.values[11] = t.values[2];
        }

        /** Translation in the direction of vec t */
        static Mat4x4f translate(const Vec3f& t) {
            return Mat4x4f(
                1, 0, 0, t.values[0],
                0, 1, 0, t.values[1],
                0, 0, 1, t.values[2],
                0, 0, 0,           1
            );
        }

        /** Clockwise around axis rotation */
        static Mat4x4f rotateX(float angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat4x4f(
                1, 0,  0, 0,
                0, c, -s, 0,
                0, s,  c, 0,
                0, 0,  0, 1
            );
        }

        /** Clockwise around axis rotation */
        static Mat4x4f rotateY(float angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat4x4f(
                 c, 0, s, 0,
                 0, 1, 0, 0,
                -s, 0, c, 0,
                 0, 0, 0, 1
            );
        }

        /** Clockwise around axis rotation */
        static Mat4x4f rotateZ(float angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat4x4f(
                c, -s, 0, 0,
                s,  c, 0, 0,
                0,  0, 1, 0,
                0,  0, 0, 1
            );
        }

        /** Clockwise rotation around an arbitrary axis */
        static Mat4x4f rotate(const Vec3f& axis, float angleRad) {
            auto Ax = axis.normalized();
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);
            auto oneMinC = 1 - c;

            return Mat4x4f(
                // 0 string
                c + Ax[0] * Ax[0] * oneMinC,
                Ax[0] * Ax[1] * oneMinC - Ax[2] * s,
                Ax[0] * Ax[2] * oneMinC + Ax[1] * s,
                0,

                // 1 string
                Ax[1] * Ax[0] * oneMinC + Ax[2] * s,
                c + Ax[1] * Ax[1] * oneMinC,
                Ax[1] * Ax[2] * oneMinC - Ax[0] * s,
                0,

                // 2 string
                Ax[2] * Ax[0] * oneMinC - Ax[1] * s,
                Ax[2] * Ax[1] * oneMinC + Ax[0] * s,
                c + Ax[2] * Ax[2] * oneMinC,
                0,

                // 3 string
                0, 0, 0, 1
            );
        }

        /**
         *      | y
         *      |
         *      |_____ x
         *     /
         *    /
         *   /z
         *
         * Look at view matrix for camera (in OpenGL style)
         * @note Final are is located in the negative z space
         * @param eye Current viewer position
         * @param direction Direction vector of viewing
         * @param up Up vector to define orientation
         * @return Look matrix to transform world position to the View space
         */
        static Mat4x4f lookAt(const Vec3f &eye, const Vec3f &direction, const Vec3f &up) {
            auto Z = (-direction).normalized();
            auto X = Vec3f::cross(up, Z).normalized();
            auto Y = Vec3f::cross(Z, X);

            return Mat4x4f(
                X[0], X[1], X[2], -Vec3f::dot(X, eye),
                Y[0], Y[1], Y[2], -Vec3f::dot(Y, eye),
                Z[0], Z[1], Z[2], -Vec3f::dot(Z, eye),
                0.0f, 0.0f, 0.0f,                1.0f
            );
        }

        /**
         * @britef Perspective projection
         * Perspective projection to range [-1..1]x[-1..1]x[-1..1].
         * Note that z axis value are inn range [-1..1] (as in OpenGL).
         *
         * @warning fov should be me more than 0
         * @warning aspect should be more than 0
         *
         * @param fov   Angle between top and bottom sides in radians
         * @param aspect Width / height ratio
         * @param near   Near clip plane
         * @param far    Far clip plane
         * @return Perspective matrix
         */
        static Mat4x4f perspective(float fov, float aspect, float near, float far) {
            float ctg_angle = 1.0f / Math::tan(fov / 2.0f);

            return Mat4x4f(
                ctg_angle / aspect, 0.0f,       0.0f,                        0.0f,
                0.0f,               ctg_angle,  0.0f,                        0.0f,
                0.0f,               0.0f,       (far + near) / (near - far), (2 * far * near) / (near - far),
                0.0f,               0.0f,      -1.0f,                        0.0f
            );

        }

        /**
         * @brief Orthographic projection
         * @warning Left != right
         * @warning Bottom != top
         * @warning Near != far
         * @return Orthographic matrix
         */
        static Mat4x4f orthographic(float left, float right, float bottom, float top, float near, float far) {
            return Mat4x4f(
                2.0f / (right - left), 0.0f,                  0.0f,                (right + left) / (left - right),
                0.0f,                  2.0f / (top - bottom), 0.0f,                (top + bottom) / (bottom - top),
                0.0f,                  0.0f,                  2.0f / (far - near), (far + near) / (near - far),
                0.0f,                  0.0f,                  0.0f,                1.0f
            );
        }

    };

}

#endif //BERSERK_MAT4X4F_H