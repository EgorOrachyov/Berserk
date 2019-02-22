//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATRIX2X2_H
#define BERSERK_MATRIX2X2_H

#include "Misc/Types.h"
#include "Misc/Assert.h"
#include "Math/MathUtility.h"
#include "Strings/StaticString.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Matrix with size of 2x2 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    class CORE_EXPORT Mat2x2f
    {
    public:

        /**
         * Initialize matrix with 1
         */
        Mat2x2f();

        /**
         * Initialize matrix with values
         *
         * @param m11
         * @param m12
         * @param m21
         * @param m22
         */
        Mat2x2f(float32 m11, float32 m12,
                   float32 m21, float32 m22);

        /**
         * Initialize matrix via its vector columns
         * @param c1 First column of matrix
         * @param c2 Second column of matrix
         */
        explicit Mat2x2f(const Vec2f& c1, const Vec2f& c2);

    public:

        /**
         * Get transposed matrix
         *
         * @return
         */
        Mat2x2f transpose();

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
        Mat2x2f& operator = (const Mat2x2f& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Mat2x2f operator + (const Mat2x2f& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Mat2x2f operator - (const Mat2x2f& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Mat2x2f operator * (const Mat2x2f& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Mat2x2f operator * (float32 a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Mat2x2f operator / (float32 a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vec2f operator * (const Vec2f& v) const;

    private:

        float32 m[4];

    };

} // namespace Berserk

#endif //BERSERK_MATRIX2X2_H
