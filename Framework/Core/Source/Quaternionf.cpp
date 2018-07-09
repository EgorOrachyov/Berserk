//
// Created by Egor Orachyov on 01.07.2018.
//

#include "Math/Quaternionf.h"
#include "../Math/UtilityVectors.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Quaternionf::Quaternionf()
    {
        s = 0; x = 0; y = 0; z = 0;
    }

    Quaternionf::Quaternionf(FLOAT32 s, Vector3f v)
    {
        this->s = s; x = v.x; y = v.y; z = v.z;
    }

    Quaternionf::Quaternionf(FLOAT32 s, FLOAT32 x, FLOAT32 y, FLOAT32 z)
    {
        this->s = s; this->x = x; this->y = y; this->z = z;
    }

    Quaternionf Quaternionf::normalize()
    {
        FLOAT32 length = sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Quaternionf length should be more than 0");

        s /= length;
        x /= length;
        y /= length;
        z /= length;

        return *this;
    }

    Quaternionf Quaternionf::inverse() const
    {
        FLOAT32 length = sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Cannot inverse 0 quaternion");

        return Quaternionf(s / length, -x / length, -y / length, -z / length);
    }

    Quaternionf Quaternionf::conjugate() const
    {
        return Quaternionf(s, -x, -y, -z);
    }

    FLOAT32 Quaternionf::getNorm() const
    {
        return (s * s + x * x + y * y + z * z);
    }

    FLOAT32 Quaternionf::getLength() const
    {
        return sqrt(s * s + x * x + y * y + z * z);
    }

    Vector3f Quaternionf::getVector3() const
    {
        return Vector3f(x, y, z);
    }

    FLOAT32 Quaternionf::getScalar() const
    {
        return s;
    }

    Quaternionf Quaternionf::operator = (const Quaternionf& q)
    {
        this->s = q.s;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;

        return *this;
    }

    Quaternionf Quaternionf::operator + (const Quaternionf& q) const
    {
        return Quaternionf(s + q.s, x + q.x, y + q.y, z + q.z);
    }

    Quaternionf Quaternionf::operator - (const Quaternionf& q) const
    {
        return Quaternionf(s - q.s, x - q.x, y - q.y, z - q.z);
    }

    Quaternionf Quaternionf::operator * (const Quaternionf& q) const
    {
        // todo: rewrite in more simple mode

        Vector3f v = Vector3f(x, y, z);
        Vector3f w = Vector3f(q.x, q.y, q.z);

        return Quaternionf(s * q.s - dotProduct(v, w), w * s + v * q.s + crossProduct(v, w));
    }

    Quaternionf Quaternionf::operator * (const FLOAT32 a) const
    {
        return Quaternionf(s * a, x * a, y * a, z * a);
    }

    Quaternionf Quaternionf::operator / (const FLOAT32 a) const
    {
        return Quaternionf(s / a, x / a, y / a, z / a);
    }

    const bool Quaternionf::operator == (const Quaternionf& q) const
    {
        return (s == q.s && x == q.x && y == q.y && z == q.z);
    }

    const bool Quaternionf::operator >= (const Quaternionf& q) const
    {
        return (getNorm() >= q.getNorm());
    }

    const bool Quaternionf::operator <= (const Quaternionf& q) const
    {
        return (getNorm() <= q.getNorm());
    }

    const bool Quaternionf::operator > (const Quaternionf& q) const
    {
        return (getNorm() > q.getNorm());
    }

    const bool Quaternionf::operator < (const Quaternionf& q) const
    {
        return (getNorm() < q.getNorm());
    }

} // namespace Berserk