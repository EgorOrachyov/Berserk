//
// Created by Egor Orachyov on 27.06.2018.
//

#include "Math/Matrix3x3f.h"

namespace Berserk
{

    Matrix3x3f::Matrix3x3f()
    {
        m[0] = 0; m[1] = 0; m[2] = 0;
        m[3] = 0; m[4] = 0; m[5] = 0;
        m[6] = 0; m[7] = 0; m[8] = 0;
    }

    Matrix3x3f::Matrix3x3f(FLOAT32 m11, FLOAT32 m12, FLOAT32 m13,
                         FLOAT32 m21, FLOAT32 m22, FLOAT32 m23,
                         FLOAT32 m31, FLOAT32 m32, FLOAT32 m33)
    {
        m[0] = m11; m[1] = m12; m[2] = m13;
        m[3] = m21; m[4] = m22; m[5] = m23;
        m[6] = m31; m[7] = m32; m[8] = m33;
    }

    Matrix3x3f Matrix3x3f::getTranspose()
    {
        return Matrix3x3f(m[0], m[3], m[6],
                         m[1], m[4], m[7],
                         m[2], m[5], m[8]);
    }

    FLOAT32 Matrix3x3f::getDeterminant()
    {
        return (m[0] * m[4] * m[8] +
                m[3] * m[7] * m[2] +
                m[6] * m[1] * m[5] -
                m[2] * m[4] * m[6] -
                m[5] * m[7] * m[0] -
                m[8] * m[1] * m[3]);
    }

    FLOAT32* Matrix3x3f::getArray() const
    {
        return (FLOAT32*)m;
    }

    Matrix3x3f Matrix3x3f::operator = (const Matrix3x3f& M)
    {
        this->m[0] = M.m[0];
        this->m[1] = M.m[1];
        this->m[2] = M.m[2];
        this->m[3] = M.m[3];
        this->m[4] = M.m[4];
        this->m[5] = M.m[5];
        this->m[6] = M.m[6];
        this->m[7] = M.m[7];
        this->m[8] = M.m[8];

        return *this;
    }

    Matrix3x3f Matrix3x3f::operator + (const Matrix3x3f& M)
    {
        return Matrix3x3f(m[0] + M.m[0], m[1] + M.m[1], m[2] + M.m[2],
                         m[3] + M.m[3], m[4] + M.m[4], m[5] + M.m[5],
                         m[6] + M.m[6], m[7] + M.m[7], m[8] + M.m[8]);
    }

    Matrix3x3f Matrix3x3f::operator - (const Matrix3x3f& M)
    {
        return Matrix3x3f(m[0] - M.m[0], m[1] - M.m[1], m[2] - M.m[2],
                         m[3] - M.m[3], m[4] - M.m[4], m[5] - M.m[5],
                         m[6] - M.m[6], m[7] - M.m[7], m[8] - M.m[8]);
    }

    Matrix3x3f Matrix3x3f::operator * (const Matrix3x3f& M)
    {
        return Matrix3x3f(

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

    Matrix3x3f Matrix3x3f::operator * (const FLOAT32 a)
    {
        return Matrix3x3f(m[0] * a, m[1] * a, m[2] * a,
                         m[3] * a, m[4] * a, m[5] * a,
                         m[6] * a, m[7] * a, m[8] * a);
    }

    Matrix3x3f Matrix3x3f::operator / (const FLOAT32 a)
    {
        return Matrix3x3f(m[0] / a, m[1] / a, m[2] / a,
                         m[3] / a, m[4] / a, m[5] / a,
                         m[6] / a, m[7] / a, m[8] / a);
    }

    Vector3f Matrix3x3f::operator * (const Vector3f& v)
    {
        return Vector3f(m[0] * v.x + m[1] * v.y + m[2] * v.z,
                       m[3] * v.x + m[4] * v.y + m[5] * v.z,
                       m[6] * v.x + m[7] * v.y + m[8] * v.z);
    }

} // namespace Berserk