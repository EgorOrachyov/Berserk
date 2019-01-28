//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_MATRIX2X2_H
#define BERSERKENGINE_MATRIX2X2_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

#include "Vector2f.h"

namespace Berserk
{

    /**
     * Matrix with size of 2x2 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     */
    struct CORE_EXPORT Matrix2x2f
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix2x2f();

        /**
         * Initialize matrix with values
         *
         * @param m11
         * @param m12
         * @param m21
         * @param m22
         */
        Matrix2x2f(FLOAT32 m11, FLOAT32 m12,
                  FLOAT32 m21, FLOAT32 m22);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix2x2f getTranspose();

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
        Matrix2x2f operator = (const Matrix2x2f& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix2x2f operator + (const Matrix2x2f& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix2x2f operator - (const Matrix2x2f& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix2x2f operator * (const Matrix2x2f& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix2x2f operator * (const FLOAT32 a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix2x2f operator / (const FLOAT32 a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector2f operator * (const Vector2f& v) const;

    private:

        FLOAT32 m[4];

    };

} // namespace Berserk

#endif //BERSERKENGINE_MATRIX2X2_H
