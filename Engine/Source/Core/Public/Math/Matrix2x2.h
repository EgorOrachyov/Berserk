//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATRIX2X2_H
#define BERSERK_MATRIX2X2_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    /**
     * Matrix with size of 2x2 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     * @tparam T type (considered as float32 or float64)
     */
    template <typename T>
    class CORE_EXPORT Matrix2x2
    {
    public:

        /**
         * Initialize matrix with 1
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
        Matrix2x2(T m11, T m12,
                  T m21, T m22);

        /**
         * Initialize matrix via its vector columns
         * @param c1 First column of matrix
         * @param c2 Second column of matrix
         */
        explicit Matrix2x2(const Vector2<T>& c1, const Vector2<T>& c2);

    public:

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix2x2 transpose();

        /**
         * Get matrix determinant
         *
         * @return
         */
        T determinant();

        /**
         * Return pointer to its internal array of values
         *
         * @return float* pointer to m
         */
        T* get() const;

        /**
         * Assign via matrix M
         *
         * @param M
         * @return
         */
        Matrix2x2& operator = (const Matrix2x2& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix2x2 operator + (const Matrix2x2& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix2x2 operator - (const Matrix2x2& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix2x2 operator * (const Matrix2x2& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix2x2 operator * (T a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix2x2 operator / (T a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector2<T> operator * (const Vector2<T>& v) const;

    private:

        T m[4];

    };

    template <typename T>
    Matrix2x2<T>::Matrix2x2()
    {
        m[0] = 1; m[1] = 0;
        m[2] = 0; m[3] = 1;
    }

    template <typename T>
    Matrix2x2<T>::Matrix2x2(T m11, T m12,
                            T m21, T m22)
    {
        m[0] = m11; m[1] = m12;
        m[2] = m21; m[3] = m22;
    }

    template <typename T>
    Matrix2x2<T>::Matrix2x2(const Vector2<T> &c1, const Vector2<T> &c2)
    {
        m[0] = c1.x; m[1] = c2.x;
        m[2] = c1.y; m[3] = c2.y;
    }

    template <typename T>
    Matrix2x2<T> Matrix2x2<T>::transpose()
    {
        return Matrix2x2(m[0], m[2], m[1], m[3]);
    }

    template <typename T>
    T Matrix2x2<T>::determinant()
    {
        return (m[0] * m[3] - m[1] * m[2]);
    }

    template <typename T>
    T* Matrix2x2<T>::get() const
    {
        return (T*)m;
    }

    template <typename T>
    Matrix2x2<T>& Matrix2x2<T>::operator = (const Matrix2x2<T>& M)
    {
        m[0] = M.m[0];
        m[1] = M.m[1];
        m[2] = M.m[2];
        m[3] = M.m[3];

        return *this;
    }

    template <typename T>
    Matrix2x2<T> Matrix2x2<T>::operator + (const Matrix2x2<T>& M) const
    {
        return Matrix2x2(m[0] + M.m[0], m[1] + M.m[1],
                         m[2] + M.m[2], m[3] + M.m[3]);
    }

    template <typename T>
    Matrix2x2<T> Matrix2x2<T>::operator - (const Matrix2x2<T>& M) const
    {
        return Matrix2x2(m[0] - M.m[0], m[1] - M.m[1],
                         m[2] - M.m[2], m[3] - M.m[3]);
    }

    template <typename T>
    Matrix2x2<T> Matrix2x2<T>::operator * (const Matrix2x2<T>& M) const
    {
        return Matrix2x2(

                // 1 string

                m[0] * M.m[0] + m[1] * M.m[2],
                m[0] * M.m[1] + m[1] * M.m[3],

                // 2 string

                m[2] * M.m[0] + m[3] * M.m[2],
                m[2] * M.m[1] + m[3] * M.m[3]

        );
    }

    template <typename T>
    Matrix2x2<T> Matrix2x2<T>::operator * (const T a) const
    {
        return Matrix2x2(m[0] * a, m[1] * a,
                         m[2] * a, m[3] * a);
    }

    template <typename T>
    Matrix2x2<T> Matrix2x2<T>::operator / (const T a) const
    {
        return Matrix2x2(m[0] / a, m[1] / a,
                         m[2] / a, m[3] / a);
    }

    template <typename T>
    Vector2<T> Matrix2x2<T>::operator * (const Vector2<T>& v) const
    {
        return Vector2<T>(m[0] * v.x + m[1] * v.y, m[2] * v.x + m[3] * v.y);
    }

} // namespace Berserk

#endif //BERSERK_MATRIX2X2_H
