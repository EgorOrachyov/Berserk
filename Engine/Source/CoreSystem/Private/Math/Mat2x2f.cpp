//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Vec2f.h"
#include "Math/Mat2x2f.h"

namespace Berserk
{

    Mat2x2f::Mat2x2f()
    {
        m[0] = 1; m[1] = 0;
        m[2] = 0; m[3] = 1;
    }

    Mat2x2f::Mat2x2f(float32 m11, float32 m12,
                           float32 m21, float32 m22)
    {
        m[0] = m11; m[1] = m12;
        m[2] = m21; m[3] = m22;
    }

    Mat2x2f::Mat2x2f(const Vec2f &c1, const Vec2f &c2)
    {
        m[0] = c1.x; m[1] = c2.x;
        m[2] = c1.y; m[3] = c2.y;
    }

    Mat2x2f Mat2x2f::transpose()
    {
        return Mat2x2f(m[0], m[2], m[1], m[3]);
    }

    float32 Mat2x2f::determinant()
    {
        return (m[0] * m[3] - m[1] * m[2]);
    }

    float32* Mat2x2f::get() const
    {
        return (float32*)m;
    }

    Mat2x2f& Mat2x2f::operator = (const Mat2x2f& M)
    {
        m[0] = M.m[0];
        m[1] = M.m[1];
        m[2] = M.m[2];
        m[3] = M.m[3];

        return *this;
    }

    Mat2x2f Mat2x2f::operator + (const Mat2x2f& M) const
    {
        return Mat2x2f(m[0] + M.m[0], m[1] + M.m[1],
                          m[2] + M.m[2], m[3] + M.m[3]);
    }

    Mat2x2f Mat2x2f::operator - (const Mat2x2f& M) const
    {
        return Mat2x2f(m[0] - M.m[0], m[1] - M.m[1],
                          m[2] - M.m[2], m[3] - M.m[3]);
    }

    Mat2x2f Mat2x2f::operator * (const Mat2x2f& M) const
    {
        return Mat2x2f(

                // 1 string

                m[0] * M.m[0] + m[1] * M.m[2],
                m[0] * M.m[1] + m[1] * M.m[3],

                // 2 string

                m[2] * M.m[0] + m[3] * M.m[2],
                m[2] * M.m[1] + m[3] * M.m[3]

        );
    }

    Mat2x2f Mat2x2f::operator * (const float32 a) const
    {
        return Mat2x2f(m[0] * a, m[1] * a,
                          m[2] * a, m[3] * a);
    }

    Mat2x2f Mat2x2f::operator / (const float32 a) const
    {
        return Mat2x2f(m[0] / a, m[1] / a,
                          m[2] / a, m[3] / a);
    }

    Vec2f Mat2x2f::operator * (const Vec2f& v) const
    {
        return Vec2f(m[0] * v.x + m[1] * v.y, m[2] * v.x + m[3] * v.y);
    }
}