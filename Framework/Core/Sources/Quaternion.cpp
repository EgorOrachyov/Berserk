//
// Created by Egor Orachyov on 01.07.2018.
//

#include "../Maths/Quaternion.h"
#include "../Maths/UtilityVectors.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Quaternion::Quaternion()
    {
        s = 0; x = 0; y = 0; z = 0;
    }

    Quaternion::Quaternion(float32 s, Vector3 v)
    {
        this->s = s; x = v.x; y = v.y; z = v.z;
    }

    Quaternion::Quaternion(float32 s, float32 x, float32 y, float32 z)
    {
        this->s = s; this->x = x; this->y = y; this->z = z;
    }

    Quaternion Quaternion::Normalize()
    {
        float32 length = sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Quaternion length should be more than 0");

        s /= length;
        x /= length;
        y /= length;
        z /= length;

        return *this;
    }

    Quaternion Quaternion::Inverse() const
    {
        float32 length = sqrt(s * s + x * x + y * y + z * z);
        ASSERT(length, "Cannot inverse 0 quaternion");

        return Quaternion(s / length, -x / length, -y / length, -z / length);
    }

    Quaternion Quaternion::Conjugate() const
    {
        return Quaternion(s, -x, -y, -z);
    }

    float32 Quaternion::GetNorm() const
    {
        return (s * s + x * x + y * y + z * z);
    }

    float32 Quaternion::GetLength() const
    {
        return sqrt(s * s + x * x + y * y + z * z);
    }

    Vector3 Quaternion::GetVector3() const
    {
        return Vector3(x, y, z);
    }

    float32 Quaternion::GetScalar() const
    {
        return s;
    }

    Quaternion Quaternion::operator = (const Quaternion& q)
    {
        this->s = q.s;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;

        return *this;
    }

    Quaternion Quaternion::operator + (const Quaternion& q) const
    {
        return Quaternion(s + q.s, x + q.x, y + q.y, z + q.z);
    }

    Quaternion Quaternion::operator - (const Quaternion& q) const
    {
        return Quaternion(s - q.s, x - q.x, y - q.y, z - q.z);
    }

    Quaternion Quaternion::operator * (const Quaternion& q) const
    {
        // todo: rewrite in more simple mode

        Vector3 v = Vector3(x, y, z);
        Vector3 w = Vector3(q.x, q.y, q.z);

        return Quaternion(s * q.s - dotProduct(v, w), w * s + v * q.s + crossProduct(v, w));
    }

    Quaternion Quaternion::operator * (const float32 a) const
    {
        return Quaternion(s * a, x * a, y * a, z * a);
    }

    Quaternion Quaternion::operator / (const float32 a) const
    {
        return Quaternion(s / a, x / a, y / a, z / a);
    }

    const bool Quaternion::operator == (const Quaternion& q) const
    {
        return (s == q.s && x == q.x && y == q.y && z == q.z);
    }

    const bool Quaternion::operator >= (const Quaternion& q) const
    {
        return (GetNorm() >= q.GetNorm());
    }

    const bool Quaternion::operator <= (const Quaternion& q) const
    {
        return (GetNorm() <= q.GetNorm());
    }

    const bool Quaternion::operator > (const Quaternion& q) const
    {
        return (GetNorm() > q.GetNorm());
    }

    const bool Quaternion::operator < (const Quaternion& q) const
    {
        return (GetNorm() < q.GetNorm());
    }

} // namespace Berserk