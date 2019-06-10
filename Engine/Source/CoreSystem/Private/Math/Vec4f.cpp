//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Vec2f.h"
#include "Math/Vec3f.h"
#include "Math/Vec4f.h"

namespace Berserk
{

    Vec4f::Vec4f()
            : x(0), y(0), z(0), w(0)
    {

    }

    Vec4f::Vec4f(float32 a)
            : x(a), y(a), z(a), w(a)
    {

    }

    Vec4f::Vec4f(float32 x, float32 y, float32 z, float32 w)
            : x(x), y(y), z(z), w(w)
    {

    }

    Vec4f::Vec4f(const Vec2f &v, float32 z, float32 w)
            : x(v.x), y(v.y), z(z), w(w)
    {

    }

    Vec4f::Vec4f(const Vec2f &xy, const Vec2f &zw)
            : x(xy.x), y(xy.y), z(zw.x), w(zw.y)
    {

    }

    Vec4f::Vec4f(const Vec3f &v, float32 w)
            : x(v.x), y(v.y), z(v.z), w(w)
    {

    }

    void Vec4f::normalize()
    {
        float32 l = length();
        assert(l >= Math::THRESH_FLOAT32);

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    Vec4f Vec4f::getNormalized() const
    {
        return *this / length();
    }

    float32 Vec4f::length() const
    {
        return Math::sqrt(x * x + y * y + z * z + w * w);
    }

    float32 Vec4f::norm() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    Vec4f Vec4f::operator = (const Vec4f& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    Vec4f Vec4f::operator + (const Vec4f& v) const
    {
        return Vec4f(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    Vec4f Vec4f::operator - (const Vec4f& v) const
    {
        return Vec4f(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    Vec4f Vec4f::operator * (const Vec4f& v) const
    {
        return Vec4f(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w);
    }

    Vec4f Vec4f::operator / (const Vec4f& v) const
    {
        return Vec4f(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w);
    }

    Vec4f Vec4f::operator * (const float32 a) const
    {
        return Vec4f(this->x * a, this->y * a, this->z * a, this->w * a);
    }

    Vec4f Vec4f::operator / (const float32 a) const
    {
        return Vec4f(this->x / a, this->y / a, this->z / a, this->w / a);
    }

    const bool Vec4f::operator == (const Vec4f& v) const
    {
        return (x == v.x && y == v.y && z == v.z && w == v.w);
    }

    const bool Vec4f::operator >= (const Vec4f& v) const
    {
        return (norm() >= v.norm());
    }

    const bool Vec4f::operator <= (const Vec4f& v) const
    {
        return (norm() <= v.norm());
    }

    const bool Vec4f::operator > (const Vec4f& v) const
    {
        return (norm() > v.norm());
    }

    const bool Vec4f::operator < (const Vec4f& v) const
    {
        return (norm() > v.norm());
    }

    float32 Vec4f::getX() const
    {
        return x;
    }

    float32 Vec4f::getY() const
    {
        return y;
    }

    float32 Vec4f::getZ() const
    {
        return z;
    }

    float32 Vec4f::getW() const
    {
        return w;
    }

    CName Vec4f::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f W=%3.3f)", x, y, z, w);
        return CName(buffer);
    }

    float32 Vec4f::dot(Vec4f v1, Vec4f v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w);
    }

    Vec4f Vec4f::normalize(Vec4f v)
    {
        Vec4f r = v;
        r.normalize();
        return r;
    }

    Vec4f Vec4f::lerp(Vec4f v1, Vec4f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vec4f Vec4f::slerp(Vec4f v1, Vec4f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = dot(v1.getNormalized(), v2.getNormalized());
        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vec4f Vec4f::slerp(Vec4f v1, Vec4f v2, float32 angle, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vec4f Vec4f::smoothstep(Vec4f v1, Vec4f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (float32)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    Vec4f Vec4f::smootherstep(Vec4f v1, Vec4f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

}