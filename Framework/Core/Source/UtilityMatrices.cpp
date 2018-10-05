//
// Created by Egor Orachyov on 30.06.2018.
//

#include "Math/UtilityMatrices.h"
#include "Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Matrix4x4f newMatrix(FLOAT32 d)
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

    Matrix4x4f scale(FLOAT32 sX, FLOAT32 sY, FLOAT32 sZ)
    {
        return Matrix4x4f(sX, 0,  0,  0,
                          0,  sY, 0,  0,
                          0,  0,  sZ, 0,
                          0,  0,  0,  1);
    }

    Matrix4x4f rotateX(FLOAT32 angle)
    {
        FLOAT32 sin_a = sinf(angle);
        FLOAT32 cos_a = cosf(angle);

        return Matrix4x4f(1, 0,      0,     0,
                          0, cos_a, -sin_a, 0,
                          0, sin_a,  cos_a, 0,
                          0, 0,      0,     1);

    }

    Matrix4x4f rotateY(FLOAT32 angle)
    {
        FLOAT32 sin_a = sinf(angle);
        FLOAT32 cos_a = cosf(angle);

        return Matrix4x4f( cos_a, 0, sin_a, 0,
                           0,     1, 0,     0,
                          -sin_a, 0, cos_a, 0,
                           0,     0, 0,     1);
    }

    Matrix4x4f rotateZ(FLOAT32 angle)
    {
        FLOAT32 sin_a = sinf(angle);
        FLOAT32 cos_a = cosf(angle);

        return Matrix4x4f(cos_a, -sin_a, 0, 0,
                          sin_a,  cos_a, 0, 0,
                          0,      0,     1, 0,
                          0,      0,     0, 1);
    }

    Matrix4x4f rotate(Vector3f axis, FLOAT32 angle)
    {
        FLOAT32 sin_a = sinf(angle);
        FLOAT32 cos_a = cosf(angle);
        FLOAT32 one_min_cos = (1 - cos_a);

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
        Vector3f zaxis = (eye - target).getNormalized();                   // The "forward" vector.
        Vector3f xaxis = Vector3f::cross(up, zaxis).getNormalized();       // The "right" vector.
        Vector3f yaxis = Vector3f::cross(zaxis, xaxis);                    // The "up" vector.

        return Matrix4x4f(xaxis.x, xaxis.y, xaxis.z, -Vector3f::dot(xaxis, eye),
                          yaxis.x, yaxis.y, yaxis.z, -Vector3f::dot(yaxis, eye),
                          zaxis.x, zaxis.y, zaxis.z, -Vector3f::dot(zaxis, eye),
                          0,       0,       0,        1);
    }

    Matrix4x4f perspective(FLOAT32 fovy, FLOAT32 aspect, FLOAT32 near, FLOAT32 far)
    {
        ASSERT(fovy > 0, "Angle should be more than 0 in perspective projection");
        ASSERT(aspect > 0, "Aspect should be more than 0 in perspective projection");

        FLOAT32 ctg_angle = 1 / tan(fovy / 2);

        return Matrix4x4f(ctg_angle / aspect, 0,          0,                            0,
                          0,                  ctg_angle,  0,                            0,
                          0,                  0,          (far + near) / (near - far),  (2 * far * near) / (near - far),
                          0,                  0,          -1,                           0);
    }

    Matrix4x4f orthographic(FLOAT32 left, FLOAT32 right, FLOAT32 bottom, FLOAT32 top, FLOAT32 near, FLOAT32 far)
    {
        return Matrix4x4f(2 / (right - left), 0,                  0,                (right + left) / (left - right),
                          0,                  2 / (top - bottom), 0,                (top + bottom) / (bottom - top),
                          0,                  0,                  2 / (near - far), (far + near) / (near - far),
                          0,                  0,                  0,                1);
    }

    const Vector3f& project(const Matrix4x4f& transformation, const Vector3f& position)
    {
        Vector4f pos = transformation * Vector4f(position, 1.0);

        pos.x /= pos.w;
        pos.y /= pos.w;
        pos.z /= pos.w;

        return Vector3f(pos);
    }

    const Vector3f& projectOnScreen(const Matrix4x4f& transformation, const Vector3f& position)
    {
        Vector4f pos = transformation * Vector4f(position, 1.0);

        pos.x /= pos.w;
        pos.y /= pos.w;
        pos.z /= pos.w;

        pos.x = pos.x * 0.5f + 0.5f;
        pos.y = pos.y * 0.5f + 0.5f;
        pos.z = pos.z * 0.5f + 0.5f;

        return Vector3f(pos);
    }

    bool isInNormalView(const Vector3f& position)
    {
        return (position.x >= 0 && position.x <= 1.0 &&
                position.y >= 0 && position.y <= 1.0 &&
                position.z >= 0 && position.z <= 1.0);
    }



} // namespace Berserk