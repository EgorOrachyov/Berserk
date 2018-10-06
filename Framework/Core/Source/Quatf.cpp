//
// Created by Egor Orachyov on 01.07.2018.
//

#include "Math/Quatf.h"
#include "Math/Vector3f.h"
#include "Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Quatf::Quatf()
    {
        s = 0; x = 0; y = 0; z = 0;
    }

    Quatf::Quatf(FLOAT32 s, Vector3f v)
    {
        this->s = s; x = v.x; y = v.y; z = v.z;
    }

    Quatf::Quatf(FLOAT32 s, FLOAT32 x, FLOAT32 y, FLOAT32 z)
    {
        this->s = s; this->x = x; this->y = y; this->z = z;
    }

    Quatf Quatf::normalize()
    {
        FLOAT32 length = sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Quaternionf length should be more than 0");

        s /= length;
        x /= length;
        y /= length;
        z /= length;

        return *this;
    }

    Quatf Quatf::inverse() const
    {
        FLOAT32 length = sqrt(s * s + x * x + y * y + z * z);
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
        return sqrt(s * s + x * x + y * y + z * z);
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

    Quatf Quatf::operator * (const FLOAT32 a) const
    {
        return Quatf(s * a, x * a, y * a, z * a);
    }

    Quatf Quatf::operator / (const FLOAT32 a) const
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

} // namespace Berserk