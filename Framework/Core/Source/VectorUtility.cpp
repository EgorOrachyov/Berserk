//
// Created by Egor Orachyov on 27.06.2018.
//

#include "Math/VectorUtility.h"
#include "../Essential/Assert.h"
#include "cmath"

namespace Berserk
{

    namespace VectorUtility
    {

        FLOAT32 dot(Vector2f v1, Vector2f v2)
        {
            return (v1.x * v2.x + v1.y * v2.y);
        }

        FLOAT32 dot(Vector3f v1, Vector3f v2)
        {
            return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
        }

        FLOAT32 dot(Vector4f v1, Vector4f v2)
        {
            return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w);
        }

        Vector3f cross(Vector3f v1, Vector3f v2)
        {
            return Vector3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
        }

        FLOAT32 triple(Vector3f v1, Vector3f v2, Vector3f v3)
        {
            return dot(cross(v1, v2), v3);
        }

        Vector2f normalize(Vector2f v)
        {
            Vector2f r = v;
            r.normalize();
            return r;
        }

        Vector3f normalize(Vector3f v)
        {
            Vector3f r = v;
            r.normalize();
            return r;
        }

        Vector4f normalize(Vector4f v)
        {
            Vector4f r = v;
            r.normalize();
            return r;
        }

        Vector2f lerp(Vector2f v1, Vector2f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            return (v1 * (1 - t) + v2 * (t));
        }

        Vector3f lerp(Vector3f v1, Vector3f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            return (v1 * (1 - t) + v2 * (t));
        }

        Vector4f lerp(Vector4f v1, Vector4f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            return (v1 * (1 - t) + v2 * (t));
        }

        Vector2f slerp(Vector2f v1, Vector2f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            FLOAT32 angle = dot(v1.getNormalized(), v2.getNormalized());
            FLOAT32 sin_angle = sin(angle);

            ASSERT(angle > 0, "Angle between vectors should be more than 0");

            return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));

        }

        Vector3f slerp(Vector3f v1, Vector3f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            FLOAT32 angle = dot(v1.getNormalized(), v2.getNormalized());
            FLOAT32 sin_angle = sin(angle);

            ASSERT(angle > 0, "Angle between vectors should be more than 0");

            return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
        }

        Vector4f slerp(Vector4f v1, Vector4f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            FLOAT32 angle = dot(v1.getNormalized(), v2.getNormalized());
            FLOAT32 sin_angle = sin(angle);

            ASSERT(angle > 0, "Angle between vectors should be more than 0");

            return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
        }

        Vector2f slerp(Vector2f v1, Vector2f v2, FLOAT32 angle, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            FLOAT32 sin_angle = sin(angle);

            ASSERT(angle > 0, "Angle between vectors should be more than 0");

            return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
        }

        Vector3f slerp(Vector3f v1, Vector3f v2, FLOAT32 angle, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            FLOAT32 sin_angle = sin(angle);

            ASSERT(angle > 0, "Angle between vectors should be more than 0");

            return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
        }

        Vector4f slerp(Vector4f v1, Vector4f v2, FLOAT32 angle, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            FLOAT32 sin_angle = sin(angle);

            ASSERT(angle > 0, "Angle between vectors should be more than 0");

            return (v1 * (sin(angle * (1 - t)) / sin_angle) + v2 * (sin(angle * t) / sin_angle));
        }

        Vector2f smoothstep(Vector2f v1, Vector2f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            t = 2 * t * t * (1.5 - t);
            return lerp(v1, v2, t);
        }

        Vector3f smoothstep(Vector3f v1, Vector3f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            t = 2 * t * t * (1.5 - t);
            return lerp(v1, v2, t);
        }

        Vector4f smoothstep(Vector4f v1, Vector4f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            t = 2 * t * t * (1.5 - t);
            return lerp(v1, v2, t);
        }

        Vector2f smootherstep(Vector2f v1, Vector2f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            t = t * t * t * (t * (t * 6 - 15) + 10);
            return lerp(v1, v2, t);
        }

        Vector3f smootherstep(Vector3f v1, Vector3f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            t = t * t * t * (t * (t * 6 - 15) + 10);
            return lerp(v1, v2, t);
        }

        Vector4f smootherstep(Vector4f v1, Vector4f v2, FLOAT32 t)
        {
            ASSERT(t >= 0, "Interpolation param t should be more than 0");
            ASSERT(t <= 1, "Interpolation param t should be less than 1");

            t = t * t * t * (t * (t * 6 - 15) + 10);
            return lerp(v1, v2, t);
        }

    } // namespace VectorUtility

} // namespace Berserk