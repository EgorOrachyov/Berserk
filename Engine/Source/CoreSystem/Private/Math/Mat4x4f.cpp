//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Vec3f.h"
#include "Math/Vec4f.h"
#include "Math/Mat3x3f.h"
#include "Math/Mat4x4f.h"

namespace Berserk
{

    Mat4x4f::Mat4x4f()
    {
        m[0] = 1;  m[1] = 0;  m[2] = 0;  m[3] = 0;
        m[4] = 0;  m[5] = 1;  m[6] = 0;  m[7] = 0;
        m[8] = 0;  m[9] = 0;  m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }

    Mat4x4f::Mat4x4f(float32 d)
    {
        m[0] = d;  m[1] = 0;  m[2] = 0;  m[3] = 0;
        m[4] = 0;  m[5] = d;  m[6] = 0;  m[7] = 0;
        m[8] = 0;  m[9] = 0;  m[10] = d; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = d;
    }

    Mat4x4f::Mat4x4f(float32 m11, float32 m12, float32 m13, float32 m14,
                           float32 m21, float32 m22, float32 m23, float32 m24,
                           float32 m31, float32 m32, float32 m33, float32 m34,
                           float32 m41, float32 m42, float32 m43, float32 m44)
    {
        m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
        m[4] = m21;  m[5] = m22;  m[6] = m23;  m[7] = m24;
        m[8] = m31;  m[9] = m32;  m[10] = m33; m[11] = m34;
        m[12] = m41; m[13] = m42; m[14] = m43; m[15] = m44;

    }

    Mat4x4f::Mat4x4f(const Vec4f &c1,
                           const Vec4f &c2,
                           const Vec4f &c3,
                           const Vec4f &c4)
    {
        m[0] = c1.x;  m[1] = c2.x;  m[2] = c3.x;  m[3] = c4.x;
        m[4] = c1.y;  m[5] = c2.y;  m[6] = c3.y;  m[7] = c4.y;
        m[8] = c1.z;  m[9] = c2.z;  m[10] = c3.z; m[11] = c4.z;
        m[12] = c1.w; m[13] = c2.w; m[14] = c3.w; m[15] = c4.w;
    }

    Mat4x4f Mat4x4f::transpose()
    {
        return Mat4x4f(m[0], m[4], m[8],  m[12],
                       m[1], m[5], m[9],  m[13],
                       m[2], m[6], m[10], m[14],
                       m[3], m[7], m[11], m[15]);
    }

    float32 Mat4x4f::determinant()
    {
        auto m1 = Mat3x3f(m[5],  m[6],  m[7],
                          m[9],  m[10], m[11],
                          m[13], m[14], m[15]);
        auto m2 = Mat3x3f(m[4],  m[6],  m[7],
                          m[8],  m[10], m[11],
                          m[12], m[14], m[15]);
        auto m3 = Mat3x3f(m[4],  m[5],  m[7],
                          m[8],  m[9],  m[11],
                          m[12], m[13], m[15]);
        auto m4 = Mat3x3f(m[4],  m[5],  m[6],
                          m[8],  m[9],  m[10],
                          m[12], m[13], m[14]);

        return (m[0] * m1.determinant() -
                m[1] * m2.determinant() +
                m[2] * m3.determinant() -
                m[3] * m4.determinant());
    }

    float32* Mat4x4f::get() const
    {
        return (float32*)m;
    }

    Mat4x4f& Mat4x4f::operator = (const Mat4x4f& M)
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

    Mat4x4f Mat4x4f::operator + (const Mat4x4f& M) const
    {
        return Mat4x4f(m[0] + M.m[0], m[1] + M.m[1], m[2] + M.m[2], m[3] + M.m[3],
                          m[4] + M.m[4], m[5] + M.m[5], m[6] + M.m[6], m[7] + M.m[7],
                          m[8] + M.m[8], m[9] + M.m[9], m[10] + M.m[10], m[11] + M.m[11],
                          m[12] + M.m[12], m[13] + M.m[13], m[14] + M.m[14], m[15] + M.m[15]);
    }

    Mat4x4f Mat4x4f::operator - (const Mat4x4f& M) const
    {
        return Mat4x4f(m[0] - M.m[0], m[1] - M.m[1], m[2] - M.m[2], m[3] - M.m[3],
                          m[4] - M.m[4], m[5] - M.m[5], m[6] - M.m[6], m[7] - M.m[7],
                          m[8] - M.m[8], m[9] - M.m[9], m[10] - M.m[10], m[11] - M.m[11],
                          m[12] - M.m[12], m[13] - M.m[13], m[14] - M.m[14], m[15] - M.m[15]);
    }

    Mat4x4f Mat4x4f::operator * (const Mat4x4f& M) const
    {
        return Mat4x4f(

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

    Mat4x4f Mat4x4f::operator * (const float32 a) const
    {
        return Mat4x4f(m[0] * a, m[1] * a, m[2] * a, m[3] * a,
                          m[4] * a, m[5] * a, m[6] * a, m[7] * a,
                          m[8] * a, m[9] * a, m[10] * a, m[11] * a,
                          m[12] * a, m[13] * a, m[14] * a, m[15] * a);
    }

    Mat4x4f Mat4x4f::operator / (const float32 a) const
    {
        return Mat4x4f(m[0] / a, m[1] / a, m[2] / a, m[3] / a,
                          m[4] / a, m[5] / a, m[6] / a, m[7] / a,
                          m[8] / a, m[9] / a, m[10] / a, m[11] / a,
                          m[12] / a, m[13] / a, m[14] / a, m[15] / a);
    }

    Vec4f Mat4x4f::operator * (const Vec4f& v) const
    {
        return Vec4f(m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
                        m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
                        m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
                        m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w);
    }

    void Mat4x4f::operator*=(float32 a)
    {
        m[0] *= a;  m[1] *= a;  m[2] *= a;  m[3] *= a;
        m[4] *= a;  m[5] *= a;  m[6] *= a;  m[7] *= a;
        m[8] *= a;  m[9] *= a;  m[10] *= a; m[11] *= a;
        m[12] *= a; m[13] *= a; m[14] *= a; m[15] *= a;
    }

    Mat4x4f Mat4x4f::scale(float32 sX, float32 sY, float32 sZ)
    {
        return Mat4x4f(sX,  0,  0, 0,
                          0, sY,  0, 0,
                          0,  0, sZ, 0,
                          0,  0,  0, 1);
    }

    Mat4x4f Mat4x4f::translate(const Vec3f& t)
    {
        return Mat4x4f(1, 0, 0, t.x,
                          0, 1, 0, t.y,
                          0, 0, 1, t.z,
                          0, 0, 0, 1);
    }

    Mat4x4f Mat4x4f::rotateX(float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);

        return Mat4x4f(1, 0,      0,     0,
                          0, cos_a, -sin_a, 0,
                          0, sin_a,  cos_a, 0,
                          0, 0,      0,     1);
    }

    Mat4x4f Mat4x4f::rotateY(float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);

        return Mat4x4f(cos_a, 0, sin_a, 0,
                          0,     1, 0,     0,
                          -sin_a, 0, cos_a, 0,
                          0,     0, 0,     1);
    }

    Mat4x4f Mat4x4f::rotateZ(float32 angle)
    {
        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);

        return Mat4x4f(cos_a, -sin_a, 0, 0,
                          sin_a,  cos_a, 0, 0,
                          0,      0,     1, 0,
                          0,      0,     0, 1);
    }

    Mat4x4f Mat4x4f::rotate(const Vec3f &axis, float32 angle)
    {
        auto u = axis.getNormalized();

        auto sin_a = Math::sin(angle);
        auto cos_a = Math::cos(angle);
        auto one_min_cos = (1 - cos_a);

        return Mat4x4f (

                // 1 string

                cos_a + u.x * u.x * one_min_cos,
                u.x * u.y * one_min_cos - u.z * sin_a,
                u.x * u.z * one_min_cos + u.y * sin_a,
                0,

                // 2 string

                u.y * u.x * one_min_cos + u.z * sin_a,
                cos_a + u.y * u.y * one_min_cos,
                u.y * u.z * one_min_cos - u.x * sin_a,
                0,

                // 3 string

                u.z * u.x * one_min_cos - u.y * sin_a,
                u.z * u.y * one_min_cos + u.x * sin_a,
                cos_a + u.z * u.z * one_min_cos,
                0,

                // 4 string

                0, 0, 0, 1

        );
    }

    Mat4x4f Mat4x4f::lookAt(const Vec3f &eye, const Vec3f &direction,
                                  const Vec3f &up)
    {
        Vec3f zaxis = (direction * -1).getNormalized();                // The "forward" vector.
        Vec3f xaxis = Vec3f::cross(up, zaxis).getNormalized();         // The "right" vector.
        Vec3f yaxis = Vec3f::cross(zaxis, xaxis);                      // The "up" vector.

        return Mat4x4f(xaxis.x, xaxis.y, xaxis.z, -Vec3f::dot(xaxis, eye),
                       yaxis.x, yaxis.y, yaxis.z, -Vec3f::dot(yaxis, eye),
                       zaxis.x, zaxis.y, zaxis.z, -Vec3f::dot(zaxis, eye),
                       0,       0,       0,        1);
    }

    Mat4x4f Mat4x4f::perspective(float32 fovy, float32 aspect, float32 near, float32 far)
    {
#ifdef USE_OPEN_GL
        ASSERT(fovy > 0, "Angle should be more than 0 in perspective projection");
        ASSERT(aspect > 0, "Aspect should be more than 0 in perspective projection");

        float32 ctg_angle = 1.0f / Math::tg(fovy / 2.0f);

        return Mat4x4f(ctg_angle / aspect, 0,          0,                            0,
                       0,                  ctg_angle,  0,                            0,
                       0,                  0,          (far + near) / (near - far),  (2 * far * near) / (near - far),
                       0,                  0,          -1,                           0);
#endif
    }

    Mat4x4f Mat4x4f::orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far)
    {
        return Mat4x4f(2 / (right - left), 0,                  0,                (right + left) / (left - right),
                       0,                  2 / (top - bottom), 0,                (top + bottom) / (bottom - top),
                       0,                  0,                  2 / (near - far), (far + near) / (near - far),
                       0,                  0,                  0,                1);
    }

}