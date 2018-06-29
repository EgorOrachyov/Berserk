//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Maths/Vector3.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Vector3::Vector3()
            : x(0), y(0), z(0)
    {

    }

    Vector3::Vector3(float32 x, float32 y, float32 z)
            : x(x), y(y), z(z)
    {

    }

    void Vector3::Normalize()
    {
        float32 length = GetLength();
        ASSERT(length, "Length should be more than 1 to normalize");

        x /= length;
        y /= length;
        z /= length;
    }

    float32 Vector3::GetLength() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    float32 Vector3::GetQuadLength() const
    {
        return (x * x + y * y + z * z);
    }

    Vector3 Vector3::operator = (const Vector3& v)
    {
        return Vector3(v.x, v.y, v.z);
    }

    Vector3 Vector3::operator + (const Vector3& v)
    {
        return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    Vector3 Vector3::operator - (const Vector3& v)
    {
        return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    Vector3 Vector3::operator * (const Vector3& v)
    {
        return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    Vector3 Vector3::operator / (const Vector3& v)
    {
        return Vector3(this->x / v.x, this->y / v.y, this->z / v.z);
    }

    Vector3 Vector3::operator * (const float32 a)
    {
        return Vector3(this->x * a, this->y * a, this->z * a);
    }

    Vector3 Vector3::operator / (const float32 a)
    {
        return Vector3(this->x / a, this->y / a, this->z / a);
    }

    const bool Vector3::operator == (const Vector3& v)
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    const bool Vector3::operator >= (const Vector3& v)
    {
        return (GetQuadLength() >= v.GetQuadLength());
    }

    const bool Vector3::operator <= (const Vector3& v)
    {
        return (GetQuadLength() <= v.GetQuadLength());
    }

    const bool Vector3::operator > (const Vector3& v)
    {
        return (GetQuadLength() > v.GetQuadLength());
    }

    const bool Vector3::operator < (const Vector3& v)
    {
        return (GetQuadLength() > v.GetQuadLength());
    }

} // namespace Berserk