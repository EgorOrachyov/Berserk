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

    Vector3f::Vector3f()
            : x(0), y(0), z(0)
    {

    }

    Vector3f::Vector3f(FLOAT32 a)
            : x(a), y(a), z(a)
    {

    }

    Vector3f::Vector3f(FLOAT32 x, FLOAT32 y, FLOAT32 z)
            : x(x), y(y), z(z)
    {

    }

    Vector3f::Vector3f(const Vector2f &v, FLOAT32 z)
            : x(v.x), y(v.y), z(z)
    {

    }

    Vector3f::Vector3f(const Vector4f &v)
            : x(v.x), y(v.y), z(v.z)
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

    Vector3f Vector3f::getNormalized() const
    {
        return *this / getLength();
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

    Vector3f Vector3f::operator + (const Vector3f& v) const
    {
        return Vector3f(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    Vector3f Vector3f::operator - (const Vector3f& v) const
    {
        return Vector3f(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    Vector3f Vector3f::operator * (const Vector3f& v) const
    {
        return Vector3f(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    Vector3f Vector3f::operator / (const Vector3f& v) const
    {
        return Vector3f(this->x / v.x, this->y / v.y, this->z / v.z);
    }

    Vector3f Vector3f::operator * (const FLOAT32 a) const
    {
        return Vector3f(this->x * a, this->y * a, this->z * a);
    }

    Vector3f Vector3f::operator / (const FLOAT32 a) const
    {
        return Vector3f(this->x / a, this->y / a, this->z / a);
    }

    const bool Vector3f::operator == (const Vector3f& v) const
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    const bool Vector3f::operator >= (const Vector3f& v) const
    {
        return (getNorm() >= v.getNorm());
    }

    const bool Vector3f::operator <= (const Vector3f& v) const
    {
        return (getNorm() <= v.getNorm());
    }

    const bool Vector3f::operator > (const Vector3f& v) const
    {
        return (getNorm() > v.getNorm());
    }

    const bool Vector3f::operator < (const Vector3f& v) const
    {
        return (getNorm() > v.getNorm());
    }

    FLOAT32 Vector3f::getX() const {
        return x;
    }

    FLOAT32 Vector3f::getY() const {
        return y;
    }

    FLOAT32 Vector3f::getZ() const {
        return z;
    }

    CStaticString Vector3f::toString() const
    {
        CHAR buffer[BUFFER_SIZE_64];
        sprintf(buffer, "(X=%3.3f Y=%3.3f Z=%3.3f)", x, y, z);
        return CStaticString(buffer);
    }

    FLOAT32 Vector3f::dot(Vector3f v1, Vector3f v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

    Vector3f Vector3f::cross(Vector3f v1, Vector3f v2)
    {
        return Vector3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    FLOAT32 Vector3f::triple(Vector3f v1, Vector3f v2, Vector3f v3)
    {
        return dot(cross(v1, v2), v3);
    }

    Vector3f Vector3f::normalize(Vector3f v)
    {
        Vector3f r = v;
        r.normalize();
        return r;
    }

    Vector3f Vector3f::lerp(Vector3f v1, Vector3f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vector3f Vector3f::slerp(Vector3f v1, Vector3f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        FLOAT32 angle = dot(v1.getNormalized(), v2.getNormalized());
        FLOAT32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector3f Vector3f::slerp(Vector3f v1, Vector3f v2, FLOAT32 angle, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        FLOAT32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector3f Vector3f::smoothstep(Vector3f v1, Vector3f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = (FLOAT32)(2 * t * t * (1.5 - t));
        return lerp(v1, v2, t);
    }

    Vector3f Vector3f::smootherstep(Vector3f v1, Vector3f v2, FLOAT32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

    const Vector3f Vector3f::axisX = Vector3f(1,0,0);
    const Vector3f Vector3f::axisY = Vector3f(0,1,0);
    const Vector3f Vector3f::axisZ = Vector3f(0,0,1);

} // namespace Berserk