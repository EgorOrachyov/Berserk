//
// Created by Egor Orachyov on 27.06.2018.
//

#include "../Maths/UtilityVectors.h"
#include "../Essential/Assert.h"
#include "cmath"

namespace Berserk
{

    Vector2 newVector(float32 x, float32 y)
    {
        return Vector2(x, y);
    }

    Vector3 newVector(float32 x, float32 y, float32 z)
    {
        return Vector3(x, y, z);
    }

    Vector4 newVector(float32 x, float32 y, float32 z, float32 w)
    {
        return Vector4(x, y, z, w);
    }

    float32 dotProduct(Vector2 v1, Vector2 v2)
    {
        return (v1.x * v2.x + v1.y * v2.y);
    }

    float32 dotProduct(Vector3 v1, Vector3 v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

    float32 dotProduct(Vector4 v1, Vector4 v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w);
    }

    float32 length(Vector2 v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    float32 length(Vector3 v)
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float32 length(Vector4 v)
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }

    float32 norm(Vector2 v)
    {
        return (v.x * v.x + v.y * v.y);
    }

    float32 norm(Vector3 v)
    {
        return (v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float32 norm(Vector4 v)
    {
        return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }

    Vector3 crossProduct(Vector3 v1, Vector3 v2)
    {
        return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    float32 tripleProduct(Vector3 v1, Vector3 v2, Vector3 v3)
    {
        return dotProduct(crossProduct(v1, v2), v3);
    }

    Vector2 multByElement(Vector2 v1, Vector2 v2)
    {
        return v1 * v2;
    }

    Vector3 multByElement(Vector3 v1, Vector3 v2)
    {
        return v1 * v2;
    }

    Vector4 multByElement(Vector4 v1, Vector4 v2)
    {
        return v1 * v2;
    }

    Vector2 normalize(Vector2 v)
    {
        Vector2 r = v;
        r.normalize();
        return r;
    }

    Vector3 normalize(Vector3 v)
    {
        Vector3 r = v;
        r.normalize();
        return r;
    }

    Vector4 normalize(Vector4 v)
    {
        Vector4 r = v;
        r.normalize();
        return r;
    }

    Vector2 lerp(Vector2 v1, Vector2 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vector3 lerp(Vector3 v1, Vector3 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vector4 lerp(Vector4 v1, Vector4 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (v1 * (1 - t) + v2 * (t));
    }

    Vector2 slerp(Vector2 v1, Vector2 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = dotProduct(normalize(v1), normalize(v2));
        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));

    }

    Vector3 slerp(Vector3 v1, Vector3 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = dotProduct(normalize(v1), normalize(v2));
        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector4 slerp(Vector4 v1, Vector4 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = dotProduct(normalize(v1), normalize(v2));
        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector2 slerp(Vector2 v1, Vector2 v2, float32 angle, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector3 slerp(Vector3 v1, Vector3 v2, float32 angle, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector4 slerp(Vector4 v1, Vector4 v2, float32 angle, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 sin_angle = sin(angle);

        ASSERT(angle > 0, "Angle between vectors should be more than 0");

        return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
    }

    Vector2 smoothstep(Vector2 v1, Vector2 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = 2 * t * t * (1.5 - t);
        return lerp(v1, v2, t);
    }

    Vector3 smoothstep(Vector3 v1, Vector3 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = 2 * t * t * (1.5 - t);
        return lerp(v1, v2, t);
    }

    Vector4 smoothstep(Vector4 v1, Vector4 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = 2 * t * t * (1.5 - t);
        return lerp(v1, v2, t);
    }

    Vector2 smootherstep(Vector2 v1, Vector2 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

    Vector3 smootherstep(Vector3 v1, Vector3 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

    Vector4 smootherstep(Vector4 v1, Vector4 v2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        t = t * t * t * (t * (t * 6 - 15) + 10);
        return lerp(v1, v2, t);
    }

} // namespace Berserk