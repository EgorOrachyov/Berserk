//
// Created by Egor Orachyov on 27.06.2018.
//

#include "Math/Vector4f.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Vector4f::Vector4f()
            : x(0), y(0), z(0), w(0)
    {

    }

    Vector4f::Vector4f(FLOAT32 x, FLOAT32 y, FLOAT32 z, FLOAT32 w)
            : x(x), y(y), z(z), w(w)
    {

    }

    void Vector4f::normalize()
    {
        FLOAT32 length = getLength();
        ASSERT(length, "Length should be more than 1 to normalize");

        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    FLOAT32 Vector4f::getLength() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    FLOAT32 Vector4f::getNorm() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    Vector4f Vector4f::operator = (const Vector4f& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    Vector4f Vector4f::operator + (const Vector4f& v)
    {
        return Vector4f(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    Vector4f Vector4f::operator - (const Vector4f& v)
    {
        return Vector4f(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    Vector4f Vector4f::operator * (const Vector4f& v)
    {
        return Vector4f(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w);
    }

    Vector4f Vector4f::operator / (const Vector4f& v)
    {
        return Vector4f(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w);
    }

    Vector4f Vector4f::operator * (const FLOAT32 a)
    {
        return Vector4f(this->x * a, this->y * a, this->z * a, this->w * a);
    }

    Vector4f Vector4f::operator / (const FLOAT32 a)
    {
        return Vector4f(this->x / a, this->y / a, this->z / a, this->w / a);
    }

    const bool Vector4f::operator == (const Vector4f& v)
    {
        return (x == v.x && y == v.y && z == v.z && w == v.w);
    }

    const bool Vector4f::operator >= (const Vector4f& v)
    {
        return (getNorm() >= v.getNorm());
    }

    const bool Vector4f::operator <= (const Vector4f& v)
    {
        return (getNorm() <= v.getNorm());
    }

    const bool Vector4f::operator > (const Vector4f& v)
    {
        return (getNorm() > v.getNorm());
    }

    const bool Vector4f::operator < (const Vector4f& v)
    {
        return (getNorm() > v.getNorm());
    }

} // namespace Berserk