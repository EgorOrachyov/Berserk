//
// Created by Egor Orachyov on 27.06.2018.
//

#include "Essential/Assert.h"
#include "Math/Vector2f.h"
#include "Math/Vector3f.h"
#include "Math/Vector4f.h"
#include <cmath>

namespace Berserk
{

    Vector2f::Vector2f()
            : x(0), y(0)
    {

    }

    Vector2f::Vector2f(FLOAT32 a)
            : x(a), y(a)
    {

    }

    Vector2f::Vector2f(FLOAT32 x, FLOAT32 y)
            : x(x), y(y)
    {

    }

    Vector2f::Vector2f(const Vector3f &v)
            : x(v.x), y(v.y)
    {
    }

    Vector2f::Vector2f(const Vector4f &v)
            : x(v.x), y(v.y)
    {

    }

    void Vector2f::normalize()
    {
        FLOAT32 length = getLength();
        ASSERT(length, "Length should be more than 1 to normalize");

        x /= length;
        y /= length;
    }

    Vector2f Vector2f::getNormalized() const
    {
        return *this / getLength();
    }

    FLOAT32 Vector2f::getLength() const
    {
        return sqrt(x * x + y * y);
    }

    FLOAT32 Vector2f::getNorm() const
    {
        return (x * x + y * y);
    }

    Vector2f Vector2f::operator = (const Vector2f& v)
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    Vector2f Vector2f::operator + (const Vector2f& v) const
    {
        return Vector2f(this->x + v.x, this->y + v.y);
    }

    Vector2f Vector2f::operator - (const Vector2f& v) const
    {
        return Vector2f(this->x - v.x, this->y - v.y);
    }

    Vector2f Vector2f::operator * (const Vector2f& v) const
    {
        return Vector2f(this->x * v.x, this->y * v.y);
    }

    Vector2f Vector2f::operator / (const Vector2f& v) const
    {
        return Vector2f(this->x / v.x, this->y / v.y);
    }

    Vector2f Vector2f::operator * (const FLOAT32 a) const
    {
        return Vector2f(this->x * a, this->y * a);
    }

    Vector2f Vector2f::operator / (const FLOAT32 a) const
    {
        return Vector2f(this->x / a, this->y / a);
    }

    const bool Vector2f::operator == (const Vector2f& v) const
    {
        return (x == v.x && y == v.y);
    }

    const bool Vector2f::operator >= (const Vector2f& v) const
    {
        return (getNorm() >= v.getNorm());
    }

    const bool Vector2f::operator <= (const Vector2f& v) const
    {
        return (getNorm() <= v.getNorm());
    }

    const bool Vector2f::operator > (const Vector2f& v) const
    {
        return (getNorm() > v.getNorm());
    }

    const bool Vector2f::operator < (const Vector2f& v) const
    {
        return (getNorm() > v.getNorm());
    }

    FLOAT32 Vector2f::getX() const {
        return x;
    }

    FLOAT32 Vector2f::getY() const {
        return y;
    }

} // namespace Berserk