/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_UTILS3d_H
#define BERSERK_UTILS3d_H

#include <BerserkCore/Math/TMatMxN.hpp>

namespace Berserk {
    namespace Math {
        class Utils3d {
        public:

            static TVecN<float, 3> Multiply(const Mat4x4f& m, const TVecN<float, 3> &v) {
                TVecN<float, 4> t;
                t[0] = v[0];
                t[1] = v[1];
                t[2] = v[2];
                t[3] = 1.0f;

                return m * t;
            }

            /** Identity matrix e */
            static Mat4x4f IdentityMatrix() {
                return {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
            }

            /** Set scale elements */
            static void SetScale(Mat4x4f &m, const Vec3f &scale) {
                m.values[0] = scale[0];
                m.values[5] = scale[1];
                m.values[10] = scale[2];
            }

            static Mat4x4f scale(const Vec3f &scale) {
                return {
                    scale[0], 0, 0, 0,
                    0, scale[1], 0, 0,
                    0, 0, scale[2], 0,
                    0, 0, 0, 1
                };
            }

            /** Set translation column of matrix m to vec t */
            static void SetTranslation(Mat4x4f &m, const Vec3f &t) {
                m.values[3] = t.values[0];
                m.values[7] = t.values[1];
                m.values[11] = t.values[2];
            }

            /** Translation in the direction of vec t */
            static Mat4x4f Translate(const Vec3f &t) {
                return {
                    1, 0, 0, t.values[0],
                    0, 1, 0, t.values[1],
                    0, 0, 1, t.values[2],
                    0, 0, 0, 1
                };
            }

            /** Clockwise around axis rotation */
            static Mat4x4f RotateX(float angleRad) {
                auto s = Utils::Sin(angleRad);
                auto c = Utils::Cos(angleRad);

                return {
                    1, 0, 0, 0,
                    0, c, -s, 0,
                    0, s, c, 0,
                    0, 0, 0, 1
                };
            }

            /** Clockwise around axis rotation */
            static Mat4x4f RotateY(float angleRad) {
                auto s = Utils::Sin(angleRad);
                auto c = Utils::Cos(angleRad);

                return {
                    c, 0, s, 0,
                    0, 1, 0, 0,
                    -s, 0, c, 0,
                    0, 0, 0, 1
                };
            }

            /** Clockwise around axis rotation */
            static Mat4x4f RotateZ(float angleRad) {
                auto s = Utils::Sin(angleRad);
                auto c = Utils::Cos(angleRad);

                return {
                    c, -s, 0, 0,
                    s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                };
            }

            /** Clockwise rotation around an arbitrary axis */
            static Mat4x4f Rotate(const Vec3f &axis, float angleRad) {
                auto Ax = axis.Normalized();
                auto s = Utils::Sin(angleRad);
                auto c = Utils::Cos(angleRad);
                auto oneMinC = 1 - c;

                return {
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
            static Mat4x4f LookAt(const Vec3f &eye, const Vec3f &direction, const Vec3f &up) {
                auto Z = (-direction).Normalized();
                auto X = Vec3f::Cross(up, Z).Normalized();
                auto Y = Vec3f::Cross(Z, X);

                return {
                    X[0], X[1], X[2], -Vec3f::Dot(X, eye),
                    Y[0], Y[1], Y[2], -Vec3f::Dot(Y, eye),
                    Z[0], Z[1], Z[2], -Vec3f::Dot(Z, eye),
                    0.0f, 0.0f, 0.0f, 1.0f
                };
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
             *
             * @return Perspective matrix
             */
            static Mat4x4f Perspective(float fov, float aspect, float near, float far) {
                float ctg_angle = 1.0f / Utils::Tan(fov / 2.0f);

                return {
                    ctg_angle / aspect, 0.0f, 0.0f, 0.0f,
                    0.0f, ctg_angle, 0.0f, 0.0f,
                    0.0f, 0.0f, (far + near) / (near - far), (2 * far * near) / (near - far),
                    0.0f, 0.0f, -1.0f, 0.0f
                };

            }

            /**
             * @brief Orthographic projection
             * @warning Left != right
             * @warning Bottom != top
             * @warning Near != far
             * @return Orthographic matrix
             */
            static Mat4x4f Orthographic(float left, float right, float bottom, float top, float near, float far) {
                return {
                    2.0f / (right - left), 0.0f, 0.0f, (right + left) / (left - right),
                    0.0f, 2.0f / (top - bottom), 0.0f, (top + bottom) / (bottom - top),
                    0.0f, 0.0f, 2.0f / (far - near), (far + near) / (near - far),
                    0.0f, 0.0f, 0.0f, 1.0f
                };
            }

        };
    }
}

#endif //BERSERK_UTILS3d_H