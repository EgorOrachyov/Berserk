//
// Created by Egor Orachyov on 27.06.2018.
//

#include "Math/Vector3f.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Vector3f::Vector3f()
            : x(0), y(0), z(0)
    {

    }

    Vector3f::Vector3f(FLOAT32 x, FLOAT32 y, FLOAT32 z)
            : x(x), y(y), z(z)
    {

    }

    void Vector3f::normalize()
    {
        FLOAT32 length = getLength();
        ASSERT(length, "Length should be more than 1 to normalize");

        x /= length;
        y /= length;
        z /= length;
    }

    FLOAT32 Vector3f::getLength() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    FLOAT32 Vector3f::getNorm() const
    {
        return (x * x + y * y + z * z);
    }

    Vector3f Vector3f::operator = (const Vector3f& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    Vector3f Vector3f::operator + (const Vector3f& v)
    {
        return Vector3f(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    Vector3f Vector3f::operator - (const Vector3f& v)
    {
        return Vector3f(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    Vector3f Vector3f::operator * (const Vector3f& v)
    {
        return Vector3f(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    Vector3f Vector3f::operator / (const Vector3f& v)
    {
        return Vector3f(this->x / v.x, this->y / v.y, this->z / v.z);
    }

    Vector3f Vector3f::operator * (const FLOAT32 a)
    {
        return Vector3f(this->x * a, this->y * a, this->z * a);
    }

    Vector3f Vector3f::operator / (const FLOAT32 a)
    {
        return Vector3f(this->x / a, this->y / a, this->z / a);
    }

    const bool Vector3f::operator == (const Vector3f& v)
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    const bool Vector3f::operator >= (const Vector3f& v)
    {
        return (getNorm() >= v.getNorm());
    }

    const bool Vector3f::operator <= (const Vector3f& v)
    {
        return (getNorm() <= v.getNorm());
    }

    const bool Vector3f::operator > (const Vector3f& v)
    {
        return (getNorm() > v.getNorm());
    }

    const bool Vector3f::operator < (const Vector3f& v)
    {
        return (getNorm() > v.getNorm());
    }

} // namespace Berserk