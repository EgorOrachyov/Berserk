/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MAT4X4F_H
#define BERSERK_MAT4X4F_H

#include <Math/TMatMxN.h>

namespace Berserk {

    class Mat4x4f : public TMatMxN<float32,4,4> {
    public:
        using TMatMxN<float32,4,4>::TMatMxN;
        using TMatMxN<float32,4,4>::operator=;
        using Vec3f = TVecN<float32,3>;

        Mat4x4f(float32 m_00,float32 m_01,float32 m_02,float32 m_03,
                float32 m_10,float32 m_11,float32 m_12,float32 m_13,
                float32 m_20,float32 m_21,float32 m_22,float32 m_23,
                float32 m_30,float32 m_31,float32 m_32,float32 m_33) noexcept {
            values[0] = m_00;  values[1] = m_01;  values[2] = m_02;  values[3] = m_03;
            values[4] = m_10;  values[5] = m_11;  values[6] = m_12;  values[7] = m_13;
            values[8] = m_30;  values[9] = m_21;  values[10] = m_22; values[11] = m_23;
            values[12] = m_30; values[13] = m_31; values[14] = m_32; values[15] = m_33;
        }

        /** Identity matrix e */
        static Mat4x4f identuty() {
            return Mat4x4f{
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1
            };
        }

        /** Set translation column of matrix m to vec t */
        static void translate(Mat4x4f& m, const Vec3f& t) {
            m.values[3]  = t.values[0];
            m.values[7]  = t.values[1];
            m.values[11] = t.values[2];
        }

        /** Translation in the direction of vec t */
        static Mat4x4f translate(const Vec3f& t) {
            return Mat4x4f{
                1, 0, 0, t.values[0],
                0, 1, 0, t.values[1],
                0, 0, 1, t.values[2],
                0, 0, 0,           1
            };
        }

        /** Clockwise around axis rotation */
        static Mat4x4f rotateX(float32 angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat4x4f{
                1, 0,  0, 0,
                0, c, -s, 0,
                0, s,  c, 0,
                0, 0,  0, 1
            };
        }

        /** Clockwise around axis rotation */
        static Mat4x4f rotateY(float32 angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat4x4f{
                 c, 0, s, 0,
                 0, 1, 0, 0,
                -s, 0, c, 0,
                 0, 0, 0, 1
            };
        }

        /** Clockwise around axis rotation */
        static Mat4x4f rotateZ(float32 angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat4x4f{
                c, -s, 0, 0,
                s,  c, 0, 0,
                0,  0, 1, 0,
                0,  0, 0, 1
            };
        }

        /** Clockwise rotation around an arbitrary axis */
        static Mat4x4f rotate(const Vec3f& axis, float32 angleRad) {
            auto Ax = axis.normalized();
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);
            auto oneMinC = 1 - c;

            return Mat4x4f{
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
            };
        }

        /**
         * Look at view matrix
         * @param eye Current viewer position
         * @param direction Direction vector of viewing
         * @param up Up vector to define orientation
         * @return Look matrix to transform world position to the View space
         */
        static Mat4x4f lookAt(const Vec3f &eye, const Vec3f &direction, const Vec3f &up) {
            auto Z = (-direction).normalized();
            auto X = Vec3f::cross(up, Z).normalized();
            auto Y = Vec3f::cross(Z, X);

            return Mat4x4f{
                X[0], X[1], X[2], -Vec3f::dot(X, eye),
                Y[0], Y[1], Y[2], -Vec3f::dot(Y, eye),
                Z[0], Z[1], Z[2], -Vec3f::dot(X, eye),
                   0,    0,    0,                   1
            };
        }
    };

}

#endif //BERSERK_MAT4X4F_H