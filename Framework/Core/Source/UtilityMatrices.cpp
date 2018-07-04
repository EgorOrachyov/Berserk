//
// Created by Egor Orachyov on 30.06.2018.
//

#include "../Math/UtilityMatrices.h"
#include "../Math/UtilityVectors.h"

#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Matrix4x4f newMatrix(float32 d)
    {
        return Matrix4x4f(d, 0, 0, 0,
                         0, d, 0, 0,
                         0, 0, d, 0,
                         0, 0, 0, d);
    }


    Matrix4x4f newMatrix(Vector4f c1, Vector4f c2, Vector4f c3, Vector4f c4)
    {
        return Matrix4x4f(c1.x, c2.w, c3.x, c4.x,
                         c1.y, c2.y, c3.y, c4.y,
                         c1.z, c2.z, c3.z, c4.z,
                         c1.w, c2.w, c3.w, c4.w);
    }

    Matrix4x4f translate(Vector3f t)
    {
        return Matrix4x4f(1, 0, 0, t.x,
                         0, 1, 0, t.y,
                         0, 0, 1, t.z,
                         0, 0, 0, 1);
    }

    Matrix4x4f scale(float32 sX, float32 sY, float32 sZ)
    {
        return Matrix4x4f(sX, 0,  0,  0,
                         0,  sY, 0,  0,
                         0,  0,  sZ, 0,
                         0,  0,  0,  1);
    }

    Matrix4x4f rotateX(float32 angle)
    {
        float32 sin_a = sin(angle);
        float32 cos_a = cos(angle);

        return Matrix4x4f(1, 0,      0,     0,
                         0, cos_a, -sin_a, 0,
                         0, sin_a,  cos_a, 0,
                         0, 0,      0,     1);

    }

    Matrix4x4f rotateY(float32 angle)
    {
        float32 sin_a = sin(angle);
        float32 cos_a = cos(angle);

        return Matrix4x4f( cos_a, 0, sin_a, 0,
                          0,     1, 0,     0,
                         -sin_a, 0, cos_a, 0,
                          0,     0, 0,     1);
    }

    Matrix4x4f rotateZ(float32 angle)
    {
        float32 sin_a = sin(angle);
        float32 cos_a = cos(angle);

        return Matrix4x4f(cos_a, -sin_a, 0, 0,
                         sin_a,  cos_a, 0, 0,
                         0,      0,     1, 0,
                         0,      0,     0, 1);
    }

    Matrix4x4f rotate(Vector3f axis, float32 angle)
    {
        float32 sin_a = sin(angle);
        float32 cos_a = cos(angle);
        float32 one_min_cos = (1 - cos_a);

        return Matrix4x4f(

                // 1 string

                cos_a + axis.x * axis.x * one_min_cos,
                axis.x * axis.y * one_min_cos - axis.z * sin_a,
                axis.x * axis.z * one_min_cos + axis.y * sin_a,
                0,

                // 2 string

                axis.y * axis.x * one_min_cos + axis.z * sin_a,
                cos_a + axis.y * axis.y * one_min_cos,
                axis.y * axis.z * one_min_cos - axis.x * sin_a,
                0,

                // 2 string

                axis.z * axis.x * one_min_cos - axis.y * sin_a,
                axis.z * axis.y * one_min_cos + axis.x * sin_a,
                cos_a + axis.z * axis.z * one_min_cos,
                0,

                // 4 string

                0, 0, 0, 1

        );
    }

    Matrix4x4f lookAt(Vector3f eye, Vector3f target, Vector3f up)
    {
        Vector3f zaxis = normalize(eye - target);                // The "forward" vector.
        Vector3f xaxis = normalize(crossProduct(up, zaxis));     // The "right" vector.
        Vector3f yaxis = crossProduct(zaxis, xaxis);             // The "up" vector.

        return Matrix4x4f(xaxis.x, xaxis.y, xaxis.z, -dotProduct(xaxis, eye),
                         yaxis.x, yaxis.y, yaxis.z, -dotProduct(yaxis, eye),
                         zaxis.x, zaxis.y, zaxis.z, -dotProduct(zaxis, eye),
                         0,       0,       0,        1);
    }

    Matrix4x4f perspective(float32 fovy, float32 aspect, float32 near, float32 far)
    {
        ASSERT(fovy > 0, "Angle should be more than 0 in perspective projection");
        ASSERT(aspect > 0, "Aspect should be more than 0 in perspective projection");

        float32 ctg_angle = 1 / tan(fovy / 2);

        return Matrix4x4f(ctg_angle / aspect, 0,                           0,                                0,
                         0,                  ctg_angle,                   0,                                0,
                         0,                  0,                           (far + near) / (near - far),     (2 * far * near) / (near - far),
                         0,                  0,                          -1,                                0);
    }

    Matrix4x4f orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far)
    {
        return Matrix4x4f(2 / (right - left), 0,                  0,                (right + left) / (left - right),
                         0,                  2 / (top - bottom), 0,                (top + bottom) / (bottom - top),
                         0,                  0,                  2 / (near - far), (far + near) / (near - far),
                         0,                  0,                  0,                1);
    }

} // namespace Berserk