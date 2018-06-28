//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Maths/Vector2d.h"
#include "../Maths/Vectors.h"
#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Vector2d::Vector2d()
    {
        x = 0.0;
        y = 0.0;
    }

    Vector2d::Vector2d(float32 x, float32 y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2d::Vector2d(Vector2d& another)
    {
        x = another.x;
        y = another.y;
    }

    void Vector2d::Copy(Vector2d another)
    {
        x = another.x;
        y = another.y;
    }

    void Vector2d::Normalize()
    {
        ASSERT(GetLength(), "Cannot normalize vector of 0 length");

        float32 length = sqrt(x * x + y * y);
        x /= length;
        y /= length;
    }

    void Vector2d::Inverse()
    {
        x *= -1;
        y *= -1;
    }

    void Vector2d::Reflect(Vector2d normalOfReflection)
    {
        normalOfReflection.Normalize();
        float32 dot = dotProduct(normalOfReflection, *this);

        x = x - 2 * dot * normalOfReflection.x;
        y = y - 2 * dot * normalOfReflection.y;
    }

    void Vector2d::Project(Vector2d projectionAxis)
    {
        ASSERT(projectionAxis.GetLength(), "Cannot project via vector of 0 length");

        float32 len = length(projectionAxis);
        float32 dot = dotProduct(projectionAxis, *this);

        x = (projectionAxis.x * dot) / len;
        y = (projectionAxis.y * dot) / len;
    }

    void Vector2d::ProjectByNormalized(Vector2d projectionAxis)
    {
        ASSERT(projectionAxis.GetLength(), "Cannot project via vector of 0 length");

        float32 dot = dotProduct(projectionAxis, *this);

        x = (projectionAxis.x * dot);
        y = (projectionAxis.y * dot);
    }

    void Vector2d::Set(float32 x, float32 y)
    {
        this->x = x;
        this->y = y;
    }

    void Vector2d::SetX(float32 x)
    {
        this->x = x;
    }

    void Vector2d::SetY(float32 y)
    {
        this->y = y;
    }

    float32 Vector2d::GetLength()
    {
        return sqrt(x * x + y * y);
    }

    float32 Vector2d::GetQuadLength()
    {
        return x * x + y * y;
    }

    float32 Vector2d::GetX()
    {
        return x;
    }

    float32 Vector2d::GetY()
    {
        return y;
    }

} // namespace Berserk