//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Vec2f.h"
#include "Math/Vec3f.h"
#include "Math/Vec4f.h"

namespace Berserk
{

    Vec3f::Vec3f()
            : x(0.0), y(0.0), z(0.0)
    {

    }


    Vec3f::Vec3f(float32 a)
            : x(a), y(a), z(a)
    {

    }


    Vec3f::Vec3f(float32 x, float32 y, float32 z)
            : x(x), y(y), z(z)
    {

    }


    Vec3f::Vec3f(const Vec2f &v, float32 z)
            : x(v.x), y(v.y), z(z)
    {

    }


    Vec3f::Vec3f(const Vec4f &v)
            : x(v.x), y(v.y), z(v.z)
    {

    }


    void Vec3f::normalize()
    {
        float32 l = length();
        ASSERT(l, "Length should be more than 0 to normalize");

        x /= l;
        y /= l;
        z /= l;
    }


    Vec3f Vec3f::getNormalized() const
    {
        return *this / length();
    }


    float32 Vec3f::length() const
    {
        return Math::sqrt(x * x + y * y + z * z);
    }


    float32 Vec3f::norm() const
    {
        return (x * x + y * y + z * z);
    }


    Vec3f Vec3f::operator = (const Vec3f& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }


    Vec3f Vec3f::operator-() const
    {
        return  Vec3f(x * (-1), y * (-1), z * (-1));
    }


    Vec3f Vec3f::operator + (const Vec3f& v) const
    {
        return  Vec3f(this->x + v.x, this->y + v.y, this->z + v.z);
    }


    Vec3f Vec3f::operator - (const Vec3f& v) const
    {
        return  Vec3f(this->x - v.x, this->y - v.y, this->z - v.z);
    }


    Vec3f Vec3f::operator * (const Vec3f& v) const
    {
        return  Vec3f(this->x * v.x, this->y * v.y, this->z * v.z);
    }


    Vec3f Vec3f::operator / (const Vec3f& v) const
    {
        return  Vec3f(this->x / v.x, this->y / v.y, this->z / v.z);
    }


    Vec3f Vec3f::operator * (const float32 a) const
    {
        return  Vec3f(this->x * a, this->y * a, this->z * a);
    }


    Vec3f Vec3f::operator / (const float32 a) const
    {
        return  Vec3f(this->x / a, this->y / a, this->z / a);
    }


    float32 Vec3f::operator[](uint32 i) const
    {
        return ((float32*)(this))[i];
    }


    void Vec3f::operator+=(const Vec3f &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }


    void Vec3f::operator-=(const Vec3f &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }


    const bool Vec3f::operator == (const Vec3f& v) const
    {
        return (x == v.x && y == v.y && z == v.z);
    }


    const bool Vec3f::operator >= (const Vec3f& v) const
    {
        return (norm() >= v.norm());
    }


    const bool Vec3f::operator <= (const Vec3f& v) const
    {
        return (norm() <= v.norm());
    }


    const bool Vec3f::operator > (const Vec3f& v) const
    {
        return (norm() > v.norm());
    }


    const bool Vec3f::operator < (const Vec3f& v) const
    {
        return (norm() > v.norm());
    }


    float32 Vec3f::getX() const
    {
        return x;
    }


    float32 Vec3f::getY() const
    {
        return y;
    }


    float32 Vec3f::getZ() const
    {
        return z;
    }


    CName Vec3f::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f)", x, y, z);
        return CName(buffer);
    }


    float32 Vec3f::dot(Vec3f v1, Vec3f v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }


    Vec3f Vec3f::cross(Vec3f v1, Vec3f v2)
    {
        return  Vec3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }


    float32 Vec3f::triple(Vec3f v1, Vec3f v2, Vec3f v3)
    {
        return dot(cross(v1, v2), v3);
    }


    Vec3f Vec3f::normalize(Vec3f v)
    {
        Vec3f r = v;
        r.normalize();
        return r;
    }


    Vec3f Vec3f::lerp(Vec3f v1, Vec3f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }


    Vec3f Vec3f::slerp(Vec3f v1, Vec3f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = dot(v1.getNormalized(), v2.getNormalized());
        float32 sin_angle = Math::sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (Math::sin(angle * (1 - t)) / sin_angle) + v2 * (Math::sin(angle * t) / sin_angle));
    }


    Vec3f Vec3f::slerp(Vec3f v1, Vec3f v2, float32 angle, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 sin_angle = Math::sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (Math::sin(angle * (1 - t)) / sin_angle) + v2 * (Math::sin(angle * t) / sin_angle));
    }


    Vec3f Vec3f::smoothstep(Vec3f v1, Vec3f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (float32)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }


    Vec3f Vec3f::smootherstep(Vec3f v1, Vec3f v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

    const Vec3f Vec3f::axisX = Vec3f(1.0f,0.0f,0.0f);

    const Vec3f Vec3f::axisY = Vec3f(0.0f,1.0f,0.0f);

    const Vec3f Vec3f::axisZ = Vec3f(0.0f,0.0f,1.0f);

}