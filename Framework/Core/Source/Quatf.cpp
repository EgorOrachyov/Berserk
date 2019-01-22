//
// Created by Egor Orachyov on 01.07.2018.
//

#include "Math/Quatf.h"
#include "Math/Vector3f.h"
#include "Math/Matrix4x4f.h"
#include "Misc/Assert.h"
#include "Misc/Buffers.h"

namespace Berserk
{

    Quatf::Quatf()
            : s(0), x(0), y(0), z(0)
    {

    }

    Quatf::Quatf(FLOAT32 s, Vector3f v)
            : s(s), x(v.x), y(v.y), z(v.z)

    {

    }

    Quatf::Quatf(FLOAT32 s, FLOAT32 x, FLOAT32 y, FLOAT32 z)
            : s(s), x(x), y(y), z(z)
    {

    }

    Quatf::Quatf(Vector3f axis, FLOAT32 angle)
    {
        Vector3f v = Vector3f::normalize(axis) * Math::sin(angle / 2);
        s = Math::cos(angle / 2);
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Quatf::Quatf(FLOAT32 roll, FLOAT32 yaw, FLOAT32 pitch)
    {
        *this = Quatf(Vector3f::axisX, roll) *
                Quatf(Vector3f::axisY, yaw) *
                Quatf(Vector3f::axisZ, pitch);
    }

    Quatf::Quatf(const Vector3f &v)
    {
        *this = Quatf(Vector3f::axisX, v.x) *
                Quatf(Vector3f::axisY, v.y) *
                Quatf(Vector3f::axisZ, v.z);
    }

    Quatf::Quatf(const Matrix4x4f &M)
    {
        FLOAT32 q[4]; // notation: x[0] y[1] z[2] w[3]

        FLOAT32 trace = M.m[0] + M.m[5] + M.m[10];
        //M = M.GetTranspose();

        // Matrix 4x4 indexes
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15

        // Check the diagonal
        if (trace > 0.0)
        {
            // positive diagonal

            FLOAT32 s = Math::sqrt(trace + 1.0);
            q[3] = s * 0.5f;

            FLOAT32 t = 0.5f / s;
            q[0] = (M.m[9] - M.m[6]) * t;
            q[1] = (M.m[2] - M.m[8]) * t;
            q[2] = (M.m[4] - M.m[1]) * t;
        }
        else
        {
            // negative diagonal

            INT32 i = 0;
            if (M.m[5] > M.m[0]) i = 1;
            if (M.m[10] > M.m[4 * i + i]) i = 2;

            static const INT32 NEXT[3] = {1, 2, 0};
            INT32 j = NEXT[i];
            INT32 k = NEXT[j];

            FLOAT32 s = Math::sqrt(M.m[i * 4 + i] - (M.m[j * 4 + j] + M.m[k * 4 + k]) + 1.0);

            FLOAT32 t;
            if (s == 0.0) t = s;
            else t = 0.5f / s;

            q[i] = s * 0.5f;
            q[3] = (M.m[k * 4 + j] - M.m[j * 4 + k]) * t;
            q[j] = (M.m[j * 4 + i] - M.m[i * 4 + j]) * t;
            q[k] = (M.m[k * 4 + i] - M.m[i * 4 + k]) * t;
        }

        s = q[3];
        x = q[0];
        y = q[1];
        z = q[2];
    }

    Quatf::Quatf(const Rotator &R)
    {

    }

    Quatf Quatf::getNormalized() const
    {
        Quatf res = *this;
        return res.normalize();
    }

    Quatf Quatf::normalize()
    {
        FLOAT32 length = Math::sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Quaternionf length should be more than 0");

        s /= length;
        x /= length;
        y /= length;
        z /= length;

        return (*this);
    }

    Quatf Quatf::inverse() const
    {
        FLOAT32 length = Math::sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Cannot inverse 0 quaternion");

        return Quatf(s / length, -x / length, -y / length, -z / length);
    }

    Quatf Quatf::conjugate() const
    {
        return Quatf(s, -x, -y, -z);
    }

    FLOAT32 Quatf::getNorm() const
    {
        return (s * s + x * x + y * y + z * z);
    }

    FLOAT32 Quatf::getLength() const
    {
        return Math::Math::sqrt(s * s + x * x + y * y + z * z);
    }

    Vector3f Quatf::getVector3() const
    {
        return Vector3f(x, y, z);
    }

    FLOAT32 Quatf::getScalar() const
    {
        return s;
    }

    Quatf Quatf::operator = (const Quatf& q)
    {
        this->s = q.s;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;

        return *this;
    }

    Quatf Quatf::operator + (const Quatf& q) const
    {
        return Quatf(s + q.s, x + q.x, y + q.y, z + q.z);
    }

    Quatf Quatf::operator - (const Quatf& q) const
    {
        return Quatf(s - q.s, x - q.x, y - q.y, z - q.z);
    }

    Quatf Quatf::operator * (const Quatf& q) const
    {
        // todo: rewrite in more simple mode

        Vector3f v = Vector3f(x, y, z);
        Vector3f w = Vector3f(q.x, q.y, q.z);

        return Quatf(s * q.s - Vector3f::dot(v, w), w * s + v * q.s + Vector3f::cross(v, w));
    }

    Quatf Quatf::operator * (FLOAT32 a) const
    {
        return Quatf(s * a, x * a, y * a, z * a);
    }

    Quatf Quatf::operator / (FLOAT32 a) const
    {
        return Quatf(s / a, x / a, y / a, z / a);
    }

    const bool Quatf::operator == (const Quatf& q) const
    {
        return (s == q.s && x == q.x && y == q.y && z == q.z);
    }

    const bool Quatf::operator >= (const Quatf& q) const
    {
        return (getNorm() >= q.getNorm());
    }

    const bool Quatf::operator <= (const Quatf& q) const
    {
        return (getNorm() <= q.getNorm());
    }

    const bool Quatf::operator > (const Quatf& q) const
    {
        return (getNorm() > q.getNorm());
    }

    const bool Quatf::operator < (const Quatf& q) const
    {
        return (getNorm() < q.getNorm());
    }

    CStaticString Quatf::toString() const
    {
        CHAR buffer[Buffers::SIZE_64];
        sprintf(buffer, "(S=%3.3f X=%3.3f Y=%3.3f Z=%3.3f)", s, x, y, z);
        return CStaticString(buffer);
    }

} // namespace Berserk