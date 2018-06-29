//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_MATRIX4X4_H
#define BERSERKENGINE_MATRIX4X4_H

#include "../Essential/Types.h"
#include "Vector4.h"

namespace Berserk
{

    /**
     * Matrix with size of 4x4 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    struct Matrix4x4
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix4x4();

        /**
         * Initialize matrix with values
         *
         * @param m11
         * @param m12
         * @param m13
         * @param m14
         * @param m21
         * @param m22
         * @param m23
         * @param m24
         * @param m31
         * @param m32
         * @param m33
         * @param m34
         * @param m41
         * @param m42
         * @param m43
         * @param m44
         */
        Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
                  float32 m21, float32 m22, float32 m23, float32 m24,
                  float32 m31, float32 m32, float32 m33, float32 m34,
                  float32 m41, float32 m42, float32 m43, float32 m44);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix4x4 GetTranspose();

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
        Matrix4x4 operator = (const Matrix4x4& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix4x4 operator + (const Matrix4x4& M);

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix4x4 operator - (const Matrix4x4& M);

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix4x4 operator * (const Matrix4x4& M);

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix4x4 operator * (const float32 a);

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix4x4 operator / (const float32 a);

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector4 operator * (const Vector4& v);

    public:

        float32 m[16];

    };

} // namespace Berserk

#endif //BERSERKENGINE_MATRIX4X4_H
