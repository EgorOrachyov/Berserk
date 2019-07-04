//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATRIX3X3_H
#define BERSERK_MATRIX3X3_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Misc/UsageDescriptors.h"
#include "Math/MathUtility.h"

namespace Berserk
{

    /**
     * Matrix with size of 3x3 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    class CORE_EXPORT Mat3x3f
    {
    public:

        /**
         * Initialize matrix with 1
         */
        Mat3x3f();

        /**
         * Initialize matrix with values
         */
        Mat3x3f(float32 m11, float32 m12, float32 m13,
                   float32 m21, float32 m22, float32 m23,
                   float32 m31, float32 m32, float32 m33);

        /**
         * Initialize matrix via its vector columns
         * @param c1 First column of matrix
         * @param c2 Second column of matrix
         * @param c3 Third column of matrix
         */
        explicit Mat3x3f(const Vec3f& c1, const Vec3f& c2, const Vec3f& c3);

    public:

        /**
         * Get transposed matrix
         *
         * @return
         */
        Mat3x3f transpose() const;

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
        Mat3x3f& operator = (const Mat3x3f& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Mat3x3f operator + (const Mat3x3f& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Mat3x3f operator - (const Mat3x3f& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Mat3x3f operator * (const Mat3x3f& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Mat3x3f operator * (float32 a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Mat3x3f operator / (float32 a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vec3f operator * (const Vec3f& v) const;

    private:

        friend class Mat4x4f;

        float32 m[9];

    };

} // namespace Berserk

#endif //BERSERK_MATRIX3X3_H
