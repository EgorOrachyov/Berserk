//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_MATRIX2X2_H
#define BERSERKENGINE_MATRIX2X2_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

#include "Vector2.h"

namespace Berserk
{

    /**
     * Matrix with size of 2x2 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    struct CORE_EXPORT Matrix2x2
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix2x2();

        /**
         * Initialize matrix with values
         *
         * @param m11
         * @param m12
         * @param m21
         * @param m22
         */
        Matrix2x2(float32 m11, float32 m12,
                  float32 m21, float32 m22);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix2x2 GetTranspose();

        /**
         * Get matrix determinant
         *
         * @return
         */
        float32 GetDeterminant();

        /**
         * Return pointer to its internal array of values
         *
         * @return float* pointer to m
         */
        float32* GetArray();

        /**
         * Assign via matrix M
         *
         * @param M
         * @return
         */
        Matrix2x2 operator = (const Matrix2x2& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix2x2 operator + (const Matrix2x2& M);

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix2x2 operator - (const Matrix2x2& M);

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix2x2 operator * (const Matrix2x2& M);

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix2x2 operator * (const float32 a);

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix2x2 operator / (const float32 a);

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector2 operator * (const Vector2& v);

    public:

        float32 m[4];

    };

} // namespace Berserk

#endif //BERSERKENGINE_MATRIX2X2_H
