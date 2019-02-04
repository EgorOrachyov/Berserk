//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATRIX4X4_H
#define BERSERK_MATRIX4X4_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Math/MathUtility.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    /**
     * Matrix with size of 4x4 which stores its data in per string format and
     * multiplies vectors via right side (M * v)
     * @tparam T type (considered as float32 or float64)
     */
    template <typename T>
    class CORE_EXPORT Matrix4x4
    {
    public:

        /**
         * Initialize matrix with 0
         */
        Matrix4x4();

        /**
         * Initialize matrix with values
         */
        Matrix4x4(T m11, T m12, T m13, T m14,
                  T m21, T m22, T m23, T m24,
                  T m31, T m32, T m33, T m34,
                  T m41, T m42, T m43, T m44);

        /**
         * Get transposed matrix
         *
         * @return
         */
        Matrix4x4 getTranspose();

        /**
         * Get matrix determinant
         *
         * @return
         */
        T getDeterminant();

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
        Matrix4x4& operator = (const Matrix4x4& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        Matrix4x4 operator + (const Matrix4x4& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        Matrix4x4 operator - (const Matrix4x4& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        Matrix4x4 operator * (const Matrix4x4& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        Matrix4x4 operator * (T a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        Matrix4x4 operator / (T a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        Vector4<T> operator * (const Vector4<T>& v) const;

    public:

        friend struct Quatf;

        T m[16];

    };

    template <typename T>
    Matrix4x4<T>::Matrix4x4()
    {
        m[0] = 0;  m[1] = 0;  m[2] = 0;  m[3] = 0;
        m[4] = 0;  m[5] = 0;  m[6] = 0;  m[7] = 0;
        m[8] = 0;  m[9] = 0;  m[10] = 0; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 0;
    }

    template <typename T>
    Matrix4x4<T>::Matrix4x4(T m11, T m12, T m13, T m14,
                           T m21, T m22, T m23, T m24,
                           T m31, T m32, T m33, T m34,
                           T m41, T m42, T m43, T m44)
    {
        m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
        m[4] = m21;  m[5] = m22;  m[6] = m23;  m[7] = m24;
        m[8] = m31;  m[9] = m32;  m[10] = m33; m[11] = m34;
        m[12] = m41; m[13] = m42; m[14] = m43; m[15] = m44;

    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::getTranspose()
    {
        return Matrix4x4(m[0], m[4], m[8], m[12],
                         m[1], m[5], m[9], m[13],
                         m[2], m[6], m[10], m[14],
                         m[3], m[7], m[11], m[15]);
    }

    template <typename T>
    T Matrix4x4<T>::getDeterminant()
    {
        // todo: add formula for counting det of M 4x4
        return 0;
    }

    template <typename T>
    T* Matrix4x4<T>::get() const
    {
        return (T*)m;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::operator = (const Matrix4x4<T>& M)
    {
        m[0] = M.m[0];
        m[1] = M.m[1];
        m[2] = M.m[2];
        m[3] = M.m[3];
        m[4] = M.m[4];
        m[5] = M.m[5];
        m[6] = M.m[6];
        m[7] = M.m[7];
        m[8] = M.m[8];
        m[9] = M.m[9];
        m[10] = M.m[10];
        m[11] = M.m[11];
        m[12] = M.m[12];
        m[13] = M.m[13];
        m[14] = M.m[14];
        m[15] = M.m[15];

        return *this;
    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::operator + (const Matrix4x4<T>& M) const
    {
        return Matrix4x4(m[0] + M.m[0], m[1] + M.m[1], m[2] + M.m[2], m[3] + M.m[3],
                         m[4] + M.m[4], m[5] + M.m[5], m[6] + M.m[6], m[7] + M.m[7],
                         m[8] + M.m[8], m[9] + M.m[9], m[10] + M.m[10], m[11] + M.m[11],
                         m[12] + M.m[12], m[13] + M.m[13], m[14] + M.m[14], m[15] + M.m[15]);
    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::operator - (const Matrix4x4<T>& M) const
    {
        return Matrix4x4(m[0] - M.m[0], m[1] - M.m[1], m[2] - M.m[2], m[3] - M.m[3],
                         m[4] - M.m[4], m[5] - M.m[5], m[6] - M.m[6], m[7] - M.m[7],
                         m[8] - M.m[8], m[9] - M.m[9], m[10] - M.m[10], m[11] - M.m[11],
                         m[12] - M.m[12], m[13] - M.m[13], m[14] - M.m[14], m[15] - M.m[15]);
    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::operator * (const Matrix4x4<T>& M) const
    {
        return Matrix4x4(

                // 1 string

                m[0] * M.m[0] + m[1] * M.m[4] + m[2] * M.m[8] + m[3] * M.m[12],
                m[0] * M.m[1] + m[1] * M.m[5] + m[2] * M.m[9] + m[3] * M.m[13],
                m[0] * M.m[2] + m[1] * M.m[6] + m[2] * M.m[10] + m[3] * M.m[14],
                m[0] * M.m[3] + m[1] * M.m[7] + m[2] * M.m[11] + m[3] * M.m[15],

                // 2 string

                m[4] * M.m[0] + m[5] * M.m[4] + m[6] * M.m[8] + m[7] * M.m[12],
                m[4] * M.m[1] + m[5] * M.m[5] + m[6] * M.m[9] + m[7] * M.m[13],
                m[4] * M.m[2] + m[5] * M.m[6] + m[6] * M.m[10] + m[7] * M.m[14],
                m[4] * M.m[3] + m[5] * M.m[7] + m[6] * M.m[11] + m[7] * M.m[15],

                // 3 string

                m[8] * M.m[0] + m[9] * M.m[4] + m[10] * M.m[8] + m[11] * M.m[12],
                m[8] * M.m[1] + m[9] * M.m[5] + m[10] * M.m[9] + m[11] * M.m[13],
                m[8] * M.m[2] + m[9] * M.m[6] + m[10] * M.m[10] + m[11] * M.m[14],
                m[8] * M.m[3] + m[9] * M.m[7] + m[10] * M.m[11] + m[11] * M.m[15],

                // 4 string

                m[12] * M.m[0] + m[13] * M.m[4] + m[14] * M.m[8] + m[15] * M.m[12],
                m[12] * M.m[1] + m[13] * M.m[5] + m[14] * M.m[9] + m[15] * M.m[13],
                m[12] * M.m[2] + m[13] * M.m[6] + m[14] * M.m[10] + m[15] * M.m[14],
                m[12] * M.m[3] + m[13] * M.m[7] + m[14] * M.m[11] + m[15] * M.m[15]

        );
    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::operator * (const T a) const
    {
        return Matrix4x4(m[0] * a, m[1] * a, m[2] * a, m[3] * a,
                         m[4] * a, m[5] * a, m[6] * a, m[7] * a,
                         m[8] * a, m[9] * a, m[10] * a, m[11] * a,
                         m[12] * a, m[13] * a, m[14] * a, m[15] * a);
    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::operator / (const T a) const
    {
        return Matrix4x4(m[0] / a, m[1] / a, m[2] / a, m[3] / a,
                         m[4] / a, m[5] / a, m[6] / a, m[7] / a,
                         m[8] / a, m[9] / a, m[10] / a, m[11] / a,
                         m[12] / a, m[13] / a, m[14] / a, m[15] / a);
    }

    template <typename T>
    Vector4<T> Matrix4x4<T>::operator * (const Vector4<T>& v) const
    {
        return Vector4<T>(m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
                          m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
                          m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
                          m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w);
    }

} // namespace Berserk

#endif //BERSERK_MATRIX4X4_H
