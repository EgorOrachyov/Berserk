//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATRIX4X4_H
#define BERSERK_MATRIX4X4_H

#include "Info/VideoDriver.h"
#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Misc/UsageDescriptors.h"
#include "Math/MathUtility.h"

namespace Berserk
{

    /**
     * Matrix with size of 4x4 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    class CORE_EXPORT Mat4x4f
    {
    public:

        /**
         * Initialize matrix with 1 in the diagonal
         */
        Mat4x4f();

        /**
         * Diagonal matrix with value d
         * @param d Consider as d * unit_mat
         */
        explicit Mat4x4f(float32 d);

        /**
         * Initialize matrix with values
         */
        explicit Mat4x4f(float32 m11, float32 m12, float32 m13, float32 m14,
                            float32 m21, float32 m22, float32 m23, float32 m24,
                            float32 m31, float32 m32, float32 m33, float32 m34,
                            float32 m41, float32 m42, float32 m43, float32 m44);

        /**
         * Initialize matrix via its vector columns
         * @param c1 First column of matrix
         * @param c2 Second column of matrix
         * @param c3 Third column of matrix
         * @param c4 Fourth column of matrix
         */
        explicit Mat4x4f(const Vec4f& c1,
                            const Vec4f& c2,
                            const Vec4f& c3,
                            const Vec4f& c4);

        ~Mat4x4f() = default;

    public:

        /**
         * Get transposed matrix
         *
         * @return
         */
        Mat4x4f transpose();

        /**
         * Get matrix determinant
         *
         * @return
         */
        float32 determinant();

        /**
         * Return pointer to its internal array of values
         *
         * @return float* pointer to m
         */
        float32* get() const;

        /**
         * Assign via matrix M
         *
         * @param M
         * @return
         */
        Mat4x4f& operator = (const Mat4x4f& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Mat4x4f operator + (const Mat4x4f& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Mat4x4f operator - (const Mat4x4f& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Mat4x4f operator * (const Mat4x4f& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Mat4x4f operator * (float32 a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Mat4x4f operator / (float32 a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vec4f operator * (const Vec4f& v) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        void operator *= (float32 a);

    public:

        /**
         * Scale matrix for x, y, z axises
         * @param sX X axis scale
         * @param sY Y axis scale
         * @param sZ Z axis scale
         * @return Scale matrix
         */
        static Mat4x4f scale(float32 sX, float32 sY, float32 sZ);

        /**
         * Translation matrix for t vector
         * @param t Translation
         * @return Matrix which will translate points on vector t
         */
        static Mat4x4f translate(const Vec3f& t);

        /**
         * Clockwise rotation around the x-axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static Mat4x4f rotateX(float32 angle);

        /**
         * Clockwise rotation around the y-axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static Mat4x4f rotateY(float32 angle);

        /**
         * Clockwise rotation around the z-axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static Mat4x4f rotateZ(float32 angle);

        /**
         * Clockwise rotation around the chosen axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static Mat4x4f rotate(const Vec3f& axis, float32 angle);

        /**
         * Look at view matrix
         *
         * @param eye       Current viewer position
         * @param direction Direction vector of viewing
         * @param up        Up vector to define orientation
         * @return
         */
        static Mat4x4f lookAt(const Vec3f& eye, const Vec3f& direction, const Vec3f& up);

        /**
         * Perspective projection for OpenGL
         *
         * @warning Fovy should be me more than 0
         * @warning Aspect should be more than 0
         *
         * @param fovy   Angle between top and bottom sides in radians
         * @param aspect Width-to-height ratio
         * @param near   Near clip plane
         * @param far    Far clip plane
         * @return
         */
        static Mat4x4f perspective(float32 fovy, float32 aspect, float32 near, float32 far);

        /**
         * Orthographic projection for OpenGL
         *
         * @warning Left != rigth
         * @warning Bottom != top
         * @warning Near != far
         *
         * @param left    Left side  (min view)
         * @param right   Right side (max view)
         * @param bottom  Bottom side (min view)
         * @param top
         * @param near
         * @param far
         * @return
         */
        static Mat4x4f orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far);

    public:

        float32 m[16];

    };

} // namespace Berserk

#endif //BERSERK_MATRIX4X4_H
