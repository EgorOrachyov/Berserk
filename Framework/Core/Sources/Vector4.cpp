//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Maths/Vector4.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Vector4::Vector4()
            : x(0), y(0), z(0), w(0)
    {

    }

    Vector4::Vector4(float32 x, float32 y, float32 z, float32 w)
            : x(x), y(y), z(z), w(w)
    {

    }

    void Vector4::Normalize()
    {
        float32 length = GetLength();
        ASSERT(length, "Length should be more than 1 to normalize");

        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    float32 Vector4::GetLength() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    float32 Vector4::GetNorm() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    Vector4 Vector4::operator = (const Vector4& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    Vector4 Vector4::operator + (const Vector4& v)
    {
        return Vector4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    Vector4 Vector4::operator - (const Vector4& v)
    {
        return Vector4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    Vector4 Vector4::operator * (const Vector4& v)
    {
        return Vector4(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w);
    }

    Vector4 Vector4::operator / (const Vector4& v)
    {
        return Vector4(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w);
    }

    Vector4 Vector4::operator * (const float32 a)
    {
        return Vector4(this->x * a, this->y * a, this->z * a, this->w * a);
    }

    Vector4 Vector4::operator / (const float32 a)
    {
        return Vector4(this->x / a, this->y / a, this->z / a, this->w / a);
    }

    const bool Vector4::operator == (const Vector4& v)
    {
        return (x == v.x && y == v.y && z == v.z && w == v.w);
    }

    const bool Vector4::operator >= (const Vector4& v)
    {
        return (GetNorm() >= v.GetNorm());
    }

    const bool Vector4::operator <= (const Vector4& v)
    {
        return (GetNorm() <= v.GetNorm());
    }

    const bool Vector4::operator > (const Vector4& v)
    {
        return (GetNorm() > v.GetNorm());
    }

    const bool Vector4::operator < (const Vector4& v)
    {
        return (GetNorm() > v.GetNorm());
    }

} // namespace Berserk