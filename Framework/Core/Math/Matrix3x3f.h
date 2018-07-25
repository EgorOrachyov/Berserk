//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_MATRIX3X3_H
#define BERSERKENGINE_MATRIX3X3_H

#include "../Essential/Types.h"
#include "../Essential/UsageDescriptors.h"

#include "Vector3f.h"

namespace Berserk
{

    /**
     * Matrix with size of 3x3 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    struct CORE_EXPORT Matrix3x3f
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix3x3f();

        /**
         * Initialize matrix with values
         *
         * @param m11
         * @param m12
         * @param m13
         * @param m21
         * @param m22
         * @param m23
         * @param m31
         * @param m32
         * @param m33
         */
        Matrix3x3f(FLOAT32 m11, FLOAT32 m12, FLOAT32 m13,
                  FLOAT32 m21, FLOAT32 m22, FLOAT32 m23,
                  FLOAT32 m31, FLOAT32 m32, FLOAT32 m33);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix3x3f getTranspose();

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
        Matrix3x3f operator = (const Matrix3x3f& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix3x3f operator + (const Matrix3x3f& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix3x3f operator - (const Matrix3x3f& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix3x3f operator * (const Matrix3x3f& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix3x3f operator * (const FLOAT32 a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix3x3f operator / (const FLOAT32 a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector3f operator * (const Vector3f& v) const;

    private:

        FLOAT32 m[9];

    };

} // namespace Berserk


#endif //BERSERKENGINE_MATRIX3X3_H
