//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Math/Vector2.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Vector2::Vector2()
            : x(0), y(0)
    {

    }

    Vector2::Vector2(float32 x, float32 y)
    : x(x), y(y)
    {

    }

    void Vector2::normalize()
    {
        float32 length = getLength();
        ASSERT(length, "Length should be more than 1 to normalize");

        x /= length;
        y /= length;
    }

    float32 Vector2::getLength() const
    {
        return sqrt(x * x + y * y);
    }

    float32 Vector2::getNorm() const
    {
        return (x * x + y * y);
    }

    Vector2 Vector2::operator = (const Vector2& v)
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    Vector2 Vector2::operator + (const Vector2& v)
    {
        return Vector2(this->x + v.x, this->y + v.y);
    }

    Vector2 Vector2::operator - (const Vector2& v)
    {
        return Vector2(this->x - v.x, this->y - v.y);
    }

    Vector2 Vector2::operator * (const Vector2& v)
    {
        return Vector2(this->x * v.x, this->y * v.y);
    }

    Vector2 Vector2::operator / (const Vector2& v)
    {
        return Vector2(this->x / v.x, this->y / v.y);
    }

    Vector2 Vector2::operator * (const float32 a)
    {
        return Vector2(this->x * a, this->y * a);
    }

    Vector2 Vector2::operator / (const float32 a)
    {
        return Vector2(this->x / a, this->y / a);
    }

    const bool Vector2::operator == (const Vector2& v)
    {
        return (x == v.x && y == v.y);
    }

    const bool Vector2::operator >= (const Vector2& v)
    {
        return (getNorm() >= v.getNorm());
    }

    const bool Vector2::operator <= (const Vector2& v)
    {
        return (getNorm() <= v.getNorm());
    }

    const bool Vector2::operator > (const Vector2& v)
    {
        return (getNorm() > v.getNorm());
    }

    const bool Vector2::operator < (const Vector2& v)
    {
        return (getNorm() > v.getNorm());
    }

} // namespace Berserk