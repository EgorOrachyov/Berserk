//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_MATRIX4X4_H
#define BERSERKENGINE_MATRIX4X4_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

#include "Vector4f.h"
#include "Quaternionf.h"

namespace Berserk
{

    /**
     * Matrix with size of 4x4 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    struct CORE_EXPORT Matrix4x4f
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix4x4f();

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
        Matrix4x4f(FLOAT32 m11, FLOAT32 m12, FLOAT32 m13, FLOAT32 m14,
                  FLOAT32 m21, FLOAT32 m22, FLOAT32 m23, FLOAT32 m24,
                  FLOAT32 m31, FLOAT32 m32, FLOAT32 m33, FLOAT32 m34,
                  FLOAT32 m41, FLOAT32 m42, FLOAT32 m43, FLOAT32 m44);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix4x4f getTranspose();

        /**
         * Get matrix determinant
         *
         * @return
         */
        FLOAT32 getDeterminant();

        /**
         * Return pointer to its internal array of values
         *
         * @return float* pointer to m
         */
        FLOAT32* getArray() const;

        /**
         * Assign via matrix M
         *
         * @param M
         * @return
         */
        Matrix4x4f operator = (const Matrix4x4f& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix4x4f operator + (const Matrix4x4f& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix4x4f operator - (const Matrix4x4f& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix4x4f operator * (const Matrix4x4f& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix4x4f operator * (const FLOAT32 a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix4x4f operator / (const FLOAT32 a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector4f operator * (const Vector4f& v) const;

    public:

        friend struct Quaternionf;

        FLOAT32 m[16];

    };

} // namespace Berserk

#endif //BERSERKENGINE_MATRIX4X4_H
