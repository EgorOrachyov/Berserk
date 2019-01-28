//
// Created by Egor Orachyov on 27.06.2018.
//

#include "Misc/Assert.h"
#include "Math/Vector2f.h"
#include "Math/Vector3f.h"
#include "Math/Vector4f.h"
#include "Misc/Buffers.h"
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

    CStaticString Vector2f::toString() const
    {
        CHAR buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f)", x, y);
        return CStaticString(buffer);
    }

    FLOAT32 Vector2f::dot(Vector2f v1, Vector2f v2)
    {
        return (v1.x * v2.x + v1.y * v2.y);
    }

    Vector2f Vector2f::normalize(Vector2f v)
    {
        Vector2f r = v;
        r.normalize();
        return r;
    }

    Vector2f Vector2f::lerp(Vector2f v1, Vector2f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vector2f Vector2f::slerp(Vector2f v1, Vector2f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        FLOAT32 angle = dot(v1.getNormalized(), v2.getNormalized());
        FLOAT32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));

    }

    Vector2f Vector2f::slerp(Vector2f v1, Vector2f v2, FLOAT32 angle, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        FLOAT32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector2f Vector2f::smoothstep(Vector2f v1, Vector2f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (FLOAT32)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    Vector2f Vector2f::smootherstep(Vector2f v1, Vector2f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

} // namespace Berserk