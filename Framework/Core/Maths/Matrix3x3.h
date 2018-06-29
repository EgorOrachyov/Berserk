//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_MATRIX3X3_H
#define BERSERKENGINE_MATRIX3X3_H

#include "../Essential/Types.h"
#include "Vector3.h"

namespace Berserk
{

    /**
     * Matrix with size of 3x3 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    struct Matrix3x3
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix3x3();

        /**
         * Initialize matrix with values
         *
         * @param m11
         * @param m12
         * @param m13
         * @param m21
         * @param m22
         * @param m23
         * @param m3
         * @param m32
         * @param m33
         */
        Matrix3x3(float32 m11, float32 m12, float32 m13,
                  float32 m21, float32 m22, float32 m23,
                  float32 m31, float32 m32, float32 m33);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix3x3 GetTranspose();

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
        Matrix3x3 operator = (const Matrix3x3& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix3x3 operator + (const Matrix3x3& M);

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix3x3 operator - (const Matrix3x3& M);

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix3x3 operator * (const Matrix3x3& M);

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix3x3 operator * (const float32 a);

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix3x3 operator / (const float32 a);

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector3 operator * (const Vector3& v);

    public:

        float32 m[9];

    };

} // namespace Berserk


#endif //BERSERKENGINE_MATRIX3X3_H
