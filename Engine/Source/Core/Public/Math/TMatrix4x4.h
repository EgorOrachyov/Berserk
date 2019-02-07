//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATRIX4X4_H
#define BERSERK_MATRIX4X4_H

#include "Public/Info/VideoDriver.h"
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
    class CORE_EXPORT TMatrix4x4
    {
    public:

        /**
         * Initialize matrix with 0
         */
        TMatrix4x4();

        /**
         * Diagonal matrix with value d
         * @param d Consider as d * unit_mat
         */
        explicit TMatrix4x4(T d);

        /**
         * Initialize matrix with values
         */
        explicit TMatrix4x4(T m11, T m12, T m13, T m14,
                            T m21, T m22, T m23, T m24,
                            T m31, T m32, T m33, T m34,
                            T m41, T m42, T m43, T m44);

        /**
         * Initialize matrix via its vector columns
         * @param c1 First column of matrix
         * @param c2 Second column of matrix
         * @param c3 Third column of matrix
         * @param c4 Fourth column of matrix
         */
        explicit TMatrix4x4(const TVector4<T>& c1,
                            const TVector4<T>& c2,
                            const TVector4<T>& c3,
                            const TVector4<T>& c4);

        ~TMatrix4x4() = default;

    public:

        /**
         * Get transposed matrix
         *
         * @return
         */
        TMatrix4x4 transpose();

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
        TMatrix4x4& operator = (const TMatrix4x4& M);

        /**
         * Per value addition (m[i] + M.m[i])
         *
         * @param M
         * @return
         */
        TMatrix4x4 operator + (const TMatrix4x4& M) const;

        /**
         * Per value subtraction (m[i] - M.m[i])
         *
         * @param M
         * @return
         */
        TMatrix4x4 operator - (const TMatrix4x4& M) const;

        /**
         * Multiplication of matrix (this * M)
         *
         * @param M
         * @return
         */
        TMatrix4x4 operator * (const TMatrix4x4& M) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        TMatrix4x4 operator * (T a) const;

        /**
         * Division by number a (m[i] / a)
         *
         * @param a
         * @return
         */
        TMatrix4x4 operator / (T a) const;

        /**
         * get vector = this * v
         *
         * @param v
         * @return
         */
        TVector4<T> operator * (const TVector4<T>& v) const;

        /**
         * Multiplication via number a (m[i] * a)
         *
         * @param a
         * @return
         */
        void operator *= (T a);

    public:

        /**
         * Scale matrix for x, y, z axises
         * @param sX X axis scale
         * @param sY Y axis scale
         * @param sZ Z axis scale
         * @return Scale matrix
         */
        static TMatrix4x4 scale(T sX, T sY, T sZ);

        /**
         * Translation matrix for t vector
         * @param t Translation
         * @return Matrix which will translate points on vector t
         */
        static TMatrix4x4 translate(TVector3<T>& t);

        /**
         * Clockwise rotation around the x-axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static TMatrix4x4 rotateX(T angle);

        /**
         * Clockwise rotation around the y-axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static TMatrix4x4 rotateY(T angle);

        /**
         * Clockwise rotation around the z-axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static TMatrix4x4 rotateZ(T angle);

        /**
         * Clockwise rotation around the chosen axis
         * @param angle In radians
         * @return Rotation matrix
         */
        static TMatrix4x4 rotate(const TVector3<T>& axis, T angle);

        /**
         * Look at view matrix
         *
         * @param eye       Current viewer position
         * @param direction Direction vector of viewing
         * @param up        Up vector to define orientation
         * @return
         */
        static TMatrix4x4 lookAt(const TVector3<T>& eye, const TVector3<T>& direction, const TVector3<T>& up);

        /**
         * Perspective projection for OpenGL
         *
         * @warning Fovy should be me more than 0
         * @warning Aspect should be more than 0
         *
         * @param fovy   Angle between top and bottom sides in radians
         * @param aspect Width-to-height ratio
         * @param near   Near clip plane
         * @param far    Far clip plane
         * @return
         */
        static TMatrix4x4 perspective(T fovy, T aspect, T near, T far);

        /**
         * Orthographic projection for OpenGL
         *
         * @warning Left != rigth
         * @warning Bottom != top
         * @warning Near != far
         *
         * @param left    Left side  (min view)
         * @param right   Right side (max view)
         * @param bottom  Bottom side (min view)
         * @param top
         * @param near
         * @param far
         * @return
         */
        static TMatrix4x4 orthographic(T left, T right, T bottom, T top, T near, T far);
        
    public:

        T m[16];

    };

    template <typename T>
    TMatrix4x4<T>::TMatrix4x4()
    {
        m[0] = 1;  m[1] = 0;  m[2] = 0;  m[3] = 0;
        m[4] = 0;  m[5] = 1;  m[6] = 0;  m[7] = 0;
        m[8] = 0;  m[9] = 0;  m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }

    template <typename T>
    TMatrix4x4<T>::TMatrix4x4(T d)
    {
        m[0] = d;  m[1] = 0;  m[2] = 0;  m[3] = 0;
        m[4] = 0;  m[5] = d;  m[6] = 0;  m[7] = 0;
        m[8] = 0;  m[9] = 0;  m[10] = d; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = d;
    }

    template <typename T>
    TMatrix4x4<T>::TMatrix4x4(T m11, T m12, T m13, T m14,
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
    TMatrix4x4<T>::TMatrix4x4(const TVector4<T> &c1,
                            const TVector4<T> &c2,
                            const TVector4<T> &c3,
                            const TVector4<T> &c4)
    {
        m[0] = c1.x;  m[1] = c2.x;  m[2] = c3.x;  m[3] = c4.x;
        m[4] = c1.y;  m[5] = c2.y;  m[6] = c3.y;  m[7] = c4.y;
        m[8] = c1.z;  m[9] = c2.z;  m[10] = c3.z; m[11] = c4.z;
        m[12] = c1.w; m[13] = c2.w; m[14] = c3.w; m[15] = c4.w;
    }

    template <typename T>
    TMatrix4x4<T> TMatrix4x4<T>::transpose()
    {
        return TMatrix4x4(m[0], m[4], m[8], m[12],
                         m[1], m[5], m[9], m[13],
                         m[2], m[6], m[10], m[14],
                         m[3], m[7], m[11], m[15]);
    }

    template <typename T>
    T TMatrix4x4<T>::determinant()
    {
        auto m1 = TMatrix3x3<T>(m[5],  m[6],  m[7],
                               m[9],  m[10], m[11],
                               m[13], m[14], m[15]);
        auto m2 = TMatrix3x3<T>(m[4],  m[6],  m[7],
                               m[8],  m[10], m[11],
                               m[12], m[14], m[15]);
        auto m3 = TMatrix3x3<T>(m[4],  m[5],  m[7],
                               m[8],  m[9],  m[11],
                               m[12], m[13], m[15]);
        auto m4 = TMatrix3x3<T>(m[4],  m[5],  m[6],
                               m[8],  m[9],  m[10],
                               m[12], m[13], m[14]);

        return (m[0] * m1.determinant() -
                m[1] * m2.determinant() +
                m[2] * m3.determinant() -
                m[3] * m4.determinant());
    }

    template <typename T>
    T* TMatrix4x4<T>::get() const
    {
        return (T*)m;
    }

    template <typename T>
    TMatrix4x4<T>& TMatrix4x4<T>::operator = (const TMatrix4x4<T>& M)
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
    TMatrix4x4<T> TMatrix4x4<T>::operator + (const TMatrix4x4<T>& M) const
    {
        return TMatrix4x4(m[0] + M.m[0], m[1] + M.m[1], m[2] + M.m[2], m[3] + M.m[3],
                         m[4] + M.m[4], m[5] + M.m[5], m[6] + M.m[6], m[7] + M.m[7],
                         m[8] + M.m[8], m[9] + M.m[9], m[10] + M.m[10], m[11] + M.m[11],
                         m[12] + M.m[12], m[13] + M.m[13], m[14] + M.m[14], m[15] + M.m[15]);
    }

    template <typename T>
    TMatrix4x4<T> TMatrix4x4<T>::operator - (const TMatrix4x4<T>& M) const
    {
        return TMatrix4x4(m[0] - M.m[0], m[1] - M.m[1], m[2] - M.m[2], m[3] - M.m[3],
                         m[4] - M.m[4], m[5] - M.m[5], m[6] - M.m[6], m[7] - M.m[7],
                         m[8] - M.m[8], m[9] - M.m[9], m[10] - M.m[10], m[11] - M.m[11],
                         m[12] - M.m[12], m[13] - M.m[13], m[14] - M.m[14], m[15] - M.m[15]);
    }

    template <typename T>
    TMatrix4x4<T> TMatrix4x4<T>::operator * (const TMatrix4x4<T>& M) const
    {
        return TMatrix4x4(

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
    TMatrix4x4<T> TMatrix4x4<T>::operator * (const T a) const
    {
        return TMatrix4x4(m[0] * a, m[1] * a, m[2] * a, m[3] * a,
                         m[4] * a, m[5] * a, m[6] * a, m[7] * a,
                         m[8] * a, m[9] * a, m[10] * a, m[11] * a,
                         m[12] * a, m[13] * a, m[14] * a, m[15] * a);
    }

    template <typename T>
    TMatrix4x4<T> TMatrix4x4<T>::operator / (const T a) const
    {
        return TMatrix4x4(m[0] / a, m[1] / a, m[2] / a, m[3] / a,
                          m[4] / a, m[5] / a, m[6] / a, m[7] / a,
                          m[8] / a, m[9] / a, m[10] / a, m[11] / a,
                          m[12] / a, m[13] / a, m[14] / a, m[15] / a);
    }

    template <typename T>
    TVector4<T> TMatrix4x4<T>::operator * (const TVector4<T>& v) const
    {
        return TVector4<T>(m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
                           m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
                           m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
                           m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w);
    }

    template <typename T>
    void TMatrix4x4<T>::operator*=(T a)
    {
        m[0] *= a;  m[1] *= a;  m[2] *= a;  m[3] *= a;
        m[4] *= a;  m[5] *= a;  m[6] *= a;  m[7] *= a;
        m[8] *= a;  m[9] *= a;  m[10] *= a; m[11] *= a;
        m[12] *= a; m[13] *= a; m[14] *= a; m[15] *= a;
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::scale(float32 sX, float32 sY, float32 sZ)
    {
        return TMatrix4x4<float32>(sX,  0,  0, 0,
                                   0, sY,  0, 0,
                                   0,  0, sZ, 0,
                                   0,  0,  0, 1);
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::translate(TVector3<float32>& t)
    {
        return TMatrix4x4<float32>(1, 0, 0, t.x,
                                   0, 1, 0, t.y,
                                   0, 0, 1, t.z,
                                   0, 0, 0, 1);
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::rotateX(float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);

        return TMatrix4x4<float32>(1, 0,      0,     0,
                                   0, cos_a, -sin_a, 0,
                                   0, sin_a,  cos_a, 0,
                                   0, 0,      0,     1);
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::rotateY(float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);

        return TMatrix4x4<float32>(cos_a, 0, sin_a, 0,
                                   0,     1, 0,     0,
                                  -sin_a, 0, cos_a, 0,
                                   0,     0, 0,     1);
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::rotateZ(float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);

        return TMatrix4x4<float32>(cos_a, -sin_a, 0, 0,
                                   sin_a,  cos_a, 0, 0,
                                   0,      0,     1, 0,
                                   0,      0,     0, 1);
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::rotate(const TVector3<float32> &axis, float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);
        auto one_min_cos = (1 - cos_a);

        return TMatrix4x4<float32>(

                // 1 string

                cos_a + axis.x * axis.x * one_min_cos,
                axis.x * axis.y * one_min_cos - axis.z * sin_a,
                axis.x * axis.z * one_min_cos + axis.y * sin_a,
                0,

                // 2 string

                axis.y * axis.x * one_min_cos + axis.z * sin_a,
                cos_a + axis.y * axis.y * one_min_cos,
                axis.y * axis.z * one_min_cos - axis.x * sin_a,
                0,

                // 2 string

                axis.z * axis.x * one_min_cos - axis.y * sin_a,
                axis.z * axis.y * one_min_cos + axis.x * sin_a,
                cos_a + axis.z * axis.z * one_min_cos,
                0,

                // 4 string

                0, 0, 0, 1

        );
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::lookAt(const TVector3<float32> &eye, const TVector3<float32> &direction,
                                                    const TVector3<float32> &up)
    {
        TVector3<float32> zaxis = (direction * -1).getNormalized();                           // The "forward" vector.
        TVector3<float32> xaxis = TVector3<float32>::cross(up, zaxis).getNormalized();         // The "right" vector.
        TVector3<float32> yaxis = TVector3<float32>::cross(zaxis, xaxis);                      // The "up" vector.

        return TMatrix4x4<float32>(xaxis.x, xaxis.y, xaxis.z, -TVector3<float32>::dot(xaxis, eye),
                                   yaxis.x, yaxis.y, yaxis.z, -TVector3<float32>::dot(yaxis, eye),
                                   zaxis.x, zaxis.y, zaxis.z, -TVector3<float32>::dot(zaxis, eye),
                                   0,       0,       0,        1);
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::perspective(float32 fovy, float32 aspect, float32 near, float32 far)
    {
#ifdef USE_OPEN_GL
        ASSERT(fovy > 0, "Angle should be more than 0 in perspective projection");
        ASSERT(aspect > 0, "Aspect should be more than 0 in perspective projection");

        float32 ctg_angle = 1.0f / Math::tg(fovy / 2.0f);

        return TMatrix4x4<float32>(ctg_angle / aspect, 0,          0,                            0,
                                   0,                  ctg_angle,  0,                            0,
                                   0,                  0,          (far + near) / (near - far),  (2 * far * near) / (near - far),
                                   0,                  0,          -1,                           0);
#endif
    }

    template <>
    TMatrix4x4<float32> TMatrix4x4<float32>::orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far)
    {
        return TMatrix4x4<float32>(2 / (right - left), 0,                  0,                (right + left) / (left - right),
                                   0,                  2 / (top - bottom), 0,                (top + bottom) / (bottom - top),
                                   0,                  0,                  2 / (near - far), (far + near) / (near - far),
                                   0,                  0,                  0,                1);
    }



} // namespace Berserk

#endif //BERSERK_MATRIX4X4_H
