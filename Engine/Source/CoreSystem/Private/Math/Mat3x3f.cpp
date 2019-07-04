//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Vec3f.h"
#include "Math/Mat3x3f.h"

namespace Berserk
{

    Mat3x3f::Mat3x3f()
    {
        m[0] = 1; m[1] = 0; m[2] = 0;
        m[3] = 0; m[4] = 1; m[5] = 0;
        m[6] = 0; m[7] = 0; m[8] = 1;
    }

    Mat3x3f::Mat3x3f(float32 m11, float32 m12, float32 m13,
                           float32 m21, float32 m22, float32 m23,
                           float32 m31, float32 m32, float32 m33)
    {
        m[0] = m11; m[1] = m12; m[2] = m13;
        m[3] = m21; m[4] = m22; m[5] = m23;
        m[6] = m31; m[7] = m32; m[8] = m33;
    }

    Mat3x3f::Mat3x3f(const Vec3f &c1, const Vec3f &c2, const Vec3f &c3)
    {
        m[0] = c1.x; m[1] = c2.x; m[2] = c3.x;
        m[3] = c1.y; m[4] = c2.y; m[5] = c3.y;
        m[6] = c1.z; m[7] = c2.z; m[8] = c3.z;
    }

    Mat3x3f Mat3x3f::transpose() const
    {
        return Mat3x3f(m[0], m[3], m[6],
                          m[1], m[4], m[7],
                          m[2], m[5], m[8]);
    }

    float32 Mat3x3f::determinant()
    {
        return (m[0] * m[4] * m[8] +
                m[3] * m[7] * m[2] +
                m[6] * m[1] * m[5] -
                m[2] * m[4] * m[6] -
                m[5] * m[7] * m[0] -
                m[8] * m[1] * m[3]);
    }

    float32* Mat3x3f::get() const
    {
        return (float32*)m;
    }

    Mat3x3f& Mat3x3f::operator = (const Mat3x3f& M)
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

        return *this;
    }

    Mat3x3f Mat3x3f::operator + (const Mat3x3f& M) const
    {
        return Mat3x3f(m[0] + M.m[0], m[1] + M.m[1], m[2] + M.m[2],
                          m[3] + M.m[3], m[4] + M.m[4], m[5] + M.m[5],
                          m[6] + M.m[6], m[7] + M.m[7], m[8] + M.m[8]);
    }

    Mat3x3f Mat3x3f::operator - (const Mat3x3f& M) const
    {
        return Mat3x3f(m[0] - M.m[0], m[1] - M.m[1], m[2] - M.m[2],
                          m[3] - M.m[3], m[4] - M.m[4], m[5] - M.m[5],
                          m[6] - M.m[6], m[7] - M.m[7], m[8] - M.m[8]);
    }

    Mat3x3f Mat3x3f::operator * (const Mat3x3f& M) const
    {
        return Mat3x3f(

                // 1 string

                m[0] * M.m[0] + m[1] * M.m[3] + m[2] * M.m[6],
                m[0] * M.m[1] + m[1] * M.m[4] + m[2] * M.m[7],
                m[0] * M.m[2] + m[1] * M.m[5] + m[2] * M.m[8],

                // 2 string

                m[3] * M.m[0] + m[4] * M.m[3] + m[5] * M.m[6],
                m[3] * M.m[1] + m[4] * M.m[4] + m[5] * M.m[7],
                m[3] * M.m[2] + m[4] * M.m[5] + m[5] * M.m[8],

                // 3 string

                m[6] * M.m[0] + m[7] * M.m[3] + m[8] * M.m[6],
                m[6] * M.m[1] + m[7] * M.m[4] + m[8] * M.m[7],
                m[6] * M.m[2] + m[7] * M.m[5] + m[8] * M.m[8]

        );
    }

    Mat3x3f Mat3x3f::operator * (const float32 a) const
    {
        return Mat3x3f(m[0] * a, m[1] * a, m[2] * a,
                          m[3] * a, m[4] * a, m[5] * a,
                          m[6] * a, m[7] * a, m[8] * a);
    }

    Mat3x3f Mat3x3f::operator / (const float32 a) const
    {
        return Mat3x3f(m[0] / a, m[1] / a, m[2] / a,
                          m[3] / a, m[4] / a, m[5] / a,
                          m[6] / a, m[7] / a, m[8] / a);
    }

    Vec3f Mat3x3f::operator * (const Vec3f& v) const
    {
        return Vec3f(m[0] * v.x + m[1] * v.y + m[2] * v.z,
                        m[3] * v.x + m[4] * v.y + m[5] * v.z,
                        m[6] * v.x + m[7] * v.y + m[8] * v.z);
    }

}