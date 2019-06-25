//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Vec2f.h"
#include "Math/Vec3f.h"
#include "Math/Vec4f.h"

namespace Berserk
{

    Vec2f::Vec2f()
            : x(0), y(0)
    {

    }

    Vec2f::Vec2f(float32 a)
            : x(a), y(a)
    {

    }

    Vec2f::Vec2f(float32 x, float32 y)
            : x(x), y(y)
    {

    }

    Vec2f::Vec2f(const Vec3f &v)
            : x(v.x), y(v.y)
    {
    }

    Vec2f::Vec2f(const Vec4f &v)
            : x(v.x), y(v.y)
    {

    }

    void Vec2f::normalize()
    {
        float32 l = length();
        assertion(l >= Math::THRESH_FLOAT32);

        x /= l;
        y /= l;
    }

    Vec2f Vec2f::getNormalized() const
    {
        return *this / length();
    }

    float32 Vec2f::length() const
    {
        return Math::sqrt(x * x + y * y);
    }

    float32 Vec2f::norm() const
    {
        return (x * x + y * y);
    }

    Vec2f Vec2f::operator = (const Vec2f& v)
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    Vec2f Vec2f::operator + (const Vec2f& v) const
    {
        return Vec2f(this->x + v.x, this->y + v.y);
    }

    Vec2f Vec2f::operator - (const Vec2f& v) const
    {
        return Vec2f(this->x - v.x, this->y - v.y);
    }

    Vec2f Vec2f::operator * (const Vec2f& v) const
    {
        return Vec2f(this->x * v.x, this->y * v.y);
    }

    Vec2f Vec2f::operator / (const Vec2f& v) const
    {
        return Vec2f(this->x / v.x, this->y / v.y);
    }

    Vec2f Vec2f::operator * (const float32 a) const
    {
        return Vec2f(this->x * a, this->y * a);
    }

    Vec2f Vec2f::operator / (const float32 a) const
    {
        return Vec2f(this->x / a, this->y / a);
    }

    const bool Vec2f::operator == (const Vec2f& v) const
    {
        return (x == v.x && y == v.y);
    }

    const bool Vec2f::operator >= (const Vec2f& v) const
    {
        return (norm() >= v.norm());
    }

    const bool Vec2f::operator <= (const Vec2f& v) const
    {
        return (norm() <= v.norm());
    }

    const bool Vec2f::operator > (const Vec2f& v) const
    {
        return (norm() > v.norm());
    }

    const bool Vec2f::operator < (const Vec2f& v) const
    {
        return (norm() > v.norm());
    }

    float32 Vec2f::getX() const
    {
        return x;
    }

    float32 Vec2f::getY() const
    {
        return y;
    }

    Name Vec2f::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f)", x, y);
        return Name(buffer);
    }

    float32 Vec2f::dot(Vec2f v1, Vec2f v2)
    {
        return (v1.x * v2.x + v1.y * v2.y);
    }

    Vec2f Vec2f::normalize(Vec2f v)
    {
        Vec2f r = v;
        r.normalize();
        return r;
    }

    Vec2f Vec2f::lerp(Vec2f v1, Vec2f v2, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vec2f Vec2f::slerp(Vec2f v1, Vec2f v2, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = dot(v1.getNormalized(), v2.getNormalized());
        float32 sin_angle = sin(angle);

        assertion_msg(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));

    }

    Vec2f Vec2f::slerp(Vec2f v1, Vec2f v2, float32 angle, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        float32 sin_angle = sin(angle);

        assertion_msg(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vec2f Vec2f::smoothstep(Vec2f v1, Vec2f v2, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        t = (float32)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    Vec2f Vec2f::smootherstep(Vec2f v1, Vec2f v2, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

}