//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Maths/Vectors.h"
#include "cmath"

namespace Berserk
{

    Vector2d identityVector2d()
    {

    }

    Vector2d identityVector3d()
    {

    }

    Vector2d identityVector4d()
    {

    }

    float32 dotProduct(Vector2d v1, Vector2d v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    float32 dotProduct(Vector3d v1, Vector3d v2)
    {
    }

    float32 dotProduct(Vector4d v1, Vector4d v2)
    {

    }

    float32 length(Vector2d v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    float32 length(Vector3d v)
    {

    }

    float32 length(Vector4d v)
    {

    }

    float32 tripleProduct(Vector3d v1, Vector3d v2, Vector3d v3)
    {

    }

    Vector3d crossProduct(Vector3d v1, Vector3d v2)
    {

    }

    Vector2d multByElement(Vector2d v1, Vector2d v2)
    {

    }

    Vector3d multByElement(Vector3d v1, Vector3d v2)
    {

    }

    Vector4d multByElement(Vector4d v1, Vector4d v2)
    {

    }

    Vector2d normalize(Vector2d v)
    {
        v.Normalize();
        return v;
    }

    Vector3d normalize(Vector3d v)
    {
        //v.Normalize();
        //return v;
    }

    Vector4d normalize(Vector4d v)
    {

    }

    Vector2d lerp(Vector2d v1, Vector2d v2, float32 t)
    {

    }

    Vector3d lerp(Vector3d v1, Vector3d v2, float32 t)
    {

    }

    Vector4d lerp(Vector4d v1, Vector4d v2, float32 t)
    {

    }

    Vector2d slerp(Vector2d v1, Vector2d v2, float32 t)
    {

    }

    Vector3d slerp(Vector3d v1, Vector3d v2, float32 t)
    {

    }

    Vector4d slerp(Vector4d v1, Vector4d v2, float32 t)
    {

    }

} // namespace Berserk