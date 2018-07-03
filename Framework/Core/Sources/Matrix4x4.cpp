//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Maths/Matrix4x4.h"

namespace Berserk
{

    Matrix4x4::Matrix4x4()
    {
        m[0] = 0;  m[1] = 0;  m[2] = 0;  m[3] = 0;
        m[4] = 0;  m[5] = 0;  m[6] = 0;  m[7] = 0;
        m[8] = 0;  m[9] = 0;  m[10] = 0; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 0;
    }

    Matrix4x4::Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
                         float32 m21, float32 m22, float32 m23, float32 m24,
                         float32 m31, float32 m32, float32 m33, float32 m34,
                         float32 m41, float32 m42, float32 m43, float32 m44)
    {
        m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
        m[4] = m21;  m[5] = m22;  m[6] = m23;  m[7] = m24;
        m[8] = m31;  m[9] = m32;  m[10] = m33; m[11] = m34;
        m[12] = m41; m[13] = m42; m[14] = m43; m[15] = m44;

    }

    Matrix4x4 Matrix4x4::getTranspose()
    {
        return Matrix4x4(m[0], m[4], m[8], m[12],
                         m[1], m[5], m[9], m[13],
                         m[2], m[6], m[10], m[14],
                         m[3], m[7], m[11], m[15]);
    }

    float32 Matrix4x4::getDeterminant()
    {
        // todo: add formula for counting det of M 4x4
        return 0;
    }

    float32* Matrix4x4::getArray()
    {
        return (float32*)m;
    }

    Matrix4x4 Matrix4x4::operator = (const Matrix4x4& M)
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
        this->m[9] = M.m[9];
        this->m[10] = M.m[10];
        this->m[11] = M.m[11];
        this->m[12] = M.m[12];
        this->m[13] = M.m[13];
        this->m[14] = M.m[14];
        this->m[15] = M.m[15];

        return *this;
    }

    Matrix4x4 Matrix4x4::operator + (const Matrix4x4& M)
    {
        return Matrix4x4(m[0] + M.m[0], m[1] + M.m[1], m[2] + M.m[2], m[3] + M.m[3],
                         m[4] + M.m[4], m[5] + M.m[5], m[6] + M.m[6], m[7] + M.m[7],
                         m[8] + M.m[8], m[9] + M.m[9], m[10] + M.m[10], m[11] + M.m[11],
                         m[12] + M.m[12], m[13] + M.m[13], m[14] + M.m[14], m[15] + M.m[15]);
    }

    Matrix4x4 Matrix4x4::operator - (const Matrix4x4& M)
    {
        return Matrix4x4(m[0] - M.m[0], m[1] - M.m[1], m[2] - M.m[2], m[3] - M.m[3],
                         m[4] - M.m[4], m[5] - M.m[5], m[6] - M.m[6], m[7] - M.m[7],
                         m[8] - M.m[8], m[9] - M.m[9], m[10] - M.m[10], m[11] - M.m[11],
                         m[12] - M.m[12], m[13] - M.m[13], m[14] - M.m[14], m[15] - M.m[15]);
    }

    Matrix4x4 Matrix4x4::operator * (const Matrix4x4& M)
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

    Matrix4x4 Matrix4x4::operator * (const float32 a)
    {
        return Matrix4x4(m[0] * a, m[1] * a, m[2] * a, m[3] * a,
                         m[4] * a, m[5] * a, m[6] * a, m[7] * a,
                         m[8] * a, m[9] * a, m[10] * a, m[11] * a,
                         m[12] * a, m[13] * a, m[14] * a, m[15] * a);
    }

    Matrix4x4 Matrix4x4::operator / (const float32 a)
    {
        return Matrix4x4(m[0] / a, m[1] / a, m[2] / a, m[3] / a,
                         m[4] / a, m[5] / a, m[6] / a, m[7] / a,
                         m[8] / a, m[9] / a, m[10] / a, m[11] / a,
                         m[12] / a, m[13] / a, m[14] / a, m[15] / a);
    }

    Vector4 Matrix4x4::operator * (const Vector4& v)
    {
        return Vector4(m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
                       m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
                       m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
                       m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w);
    }

} // namespace Berserk