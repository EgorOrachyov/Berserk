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
        Matrix3x3f(float32 m11, float32 m12, float32 m13,
                  float32 m21, float32 m22, float32 m23,
                  float32 m31, float32 m32, float32 m33);

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
        float32 getDeterminant();

        /**
         * Return pointer to its internal array of values
         *
         * @return float* pointer to m
         */
        float32* getArray() const;

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
        Matrix3x3f operator + (const Matrix3x3f& M);

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix3x3f operator - (const Matrix3x3f& M);

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix3x3f operator * (const Matrix3x3f& M);

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix3x3f operator * (const float32 a);

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix3x3f operator / (const float32 a);

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector3f operator * (const Vector3f& v);

    private:

        float32 m[9];

    };

} // namespace Berserk


#endif //BERSERKENGINE_MATRIX3X3_H