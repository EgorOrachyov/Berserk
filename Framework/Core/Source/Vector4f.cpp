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

    Vector4f::Vector4f()
            : x(0), y(0), z(0), w(0)
    {

    }

    Vector4f::Vector4f(FLOAT32 a)
            : x(a), y(a), z(a), w(a)
    {

    }

    Vector4f::Vector4f(FLOAT32 x, FLOAT32 y, FLOAT32 z, FLOAT32 w)
            : x(x), y(y), z(z), w(w)
    {

    }

    Vector4f::Vector4f(const Vector2f &v, FLOAT32 z, FLOAT32 w)
            : x(v.x), y(v.y), z(z), w(w)
    {

    }

    Vector4f::Vector4f(const Vector2f &xy, const Vector2f &zw)
            : x(xy.x), y(xy.y), z(zw.x), w(zw.y)
    {

    }

    Vector4f::Vector4f(const Vector3f &v, FLOAT32 w)
            : x(v.x), y(v.y), z(v.z), w(w)
    {

    }

    void Vector4f::normalize()
    {
        FLOAT32 length = getLength();
        ASSERT(length, "Length should be more than 0 to normalize");

        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    Vector4f Vector4f::getNormalized() const
    {
        return *this / getLength();
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

    Vector4f Vector4f::operator + (const Vector4f& v) const
    {
        return Vector4f(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    Vector4f Vector4f::operator - (const Vector4f& v) const
    {
        return Vector4f(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    Vector4f Vector4f::operator * (const Vector4f& v) const
    {
        return Vector4f(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w);
    }

    Vector4f Vector4f::operator / (const Vector4f& v) const
    {
        return Vector4f(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w);
    }

    Vector4f Vector4f::operator * (const FLOAT32 a) const
    {
        return Vector4f(this->x * a, this->y * a, this->z * a, this->w * a);
    }

    Vector4f Vector4f::operator / (const FLOAT32 a) const
    {
        return Vector4f(this->x / a, this->y / a, this->z / a, this->w / a);
    }

    const bool Vector4f::operator == (const Vector4f& v) const
    {
        return (x == v.x && y == v.y && z == v.z && w == v.w);
    }

    const bool Vector4f::operator >= (const Vector4f& v) const
    {
        return (getNorm() >= v.getNorm());
    }

    const bool Vector4f::operator <= (const Vector4f& v) const
    {
        return (getNorm() <= v.getNorm());
    }

    const bool Vector4f::operator > (const Vector4f& v) const
    {
        return (getNorm() > v.getNorm());
    }

    const bool Vector4f::operator < (const Vector4f& v) const
    {
        return (getNorm() > v.getNorm());
    }

    FLOAT32 Vector4f::getX() const {
        return x;
    }

    FLOAT32 Vector4f::getY() const {
        return y;
    }

    FLOAT32 Vector4f::getZ() const {
        return z;
    }

    FLOAT32 Vector4f::getW() const {
        return w;
    }

    CStaticString Vector4f::toString() const
    {
        CHAR buffer[Buffers::SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f W=%3.3f)", x, y, z, w);
        return CStaticString(buffer);
    }

    FLOAT32 Vector4f::dot(Vector4f v1, Vector4f v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w);
    }

    Vector4f Vector4f::normalize(Vector4f v)
    {
        Vector4f r = v;
        r.normalize();
        return r;
    }

    Vector4f Vector4f::lerp(Vector4f v1, Vector4f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vector4f Vector4f::slerp(Vector4f v1, Vector4f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        FLOAT32 angle = dot(v1.getNormalized(), v2.getNormalized());
        FLOAT32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector4f Vector4f::slerp(Vector4f v1, Vector4f v2, FLOAT32 angle, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        FLOAT32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector4f Vector4f::smoothstep(Vector4f v1, Vector4f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (FLOAT32)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    Vector4f Vector4f::smootherstep(Vector4f v1, Vector4f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

} // namespace Berserk