//
// Created by Egor Orachyov on 01.07.2018.
//

#include "../Math/UtilityQuaternions.h"
#include "../Math/UtilityVectors.h"

#include "../Essential/Assert.h"
#include <cmath>

namespace Berserk
{

    Quaternionf fromVector(Vector3f axis, float32 angle)
    {
        return Quaternionf(cos(angle / 2), normalize(axis) * sin(angle / 2));
    }

    Quaternionf fromEuler(float32 roll, float32 yaw, float32 pitch)
    {
        return fromVector(Vector3f(1, 0, 0), roll) *
               fromVector(Vector3f(0, 1, 0), yaw)  *
               fromVector(Vector3f(0, 0, 1), pitch);
    }

    Quaternionf fromMatrix(Matrix4x4f M)
    {

        float32 q[4]; // notation: x[0] y[1] z[2] w[3]

        float32 trace = M.m[0] + M.m[5] + M.m[10];
        //M = M.GetTranspose();

        // Matrix 4x4 indexes
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15

        // Check the diagonal
        if (trace > 0.0)
        {
            // positive diagonal

            float32 s = sqrt(trace + 1.0);
            q[3] = s * 0.5f;

            float32 t = 0.5f / s;
            q[0] = (M.m[9] - M.m[6]) * t;
            q[1] = (M.m[2] - M.m[8]) * t;
            q[2] = (M.m[4] - M.m[1]) * t;
        }
        else
        {
            // negative diagonal

            int32 i = 0;
            if (M.m[5] > M.m[0]) i = 1;
            if (M.m[10] > M.m[4 * i + i]) i = 2;

            static const int32 NEXT[3] = {1, 2, 0};
            int32 j = NEXT[i];
            int32 k = NEXT[j];

            float32 s = sqrt(M.m[i * 4 + i] - (M.m[j * 4 + j] + M.m[k * 4 + k]) + 1.0);

            float32 t;
            if (s == 0.0) t = s;
            else t = 0.5f / s;

            q[i] = s * 0.5f;
            q[3] = (M.m[k * 4 + j] - M.m[j * 4 + k]) * t;
            q[j] = (M.m[j * 4 + i] - M.m[i * 4 + j]) * t;
            q[k] = (M.m[k * 4 + i] - M.m[i * 4 + k]) * t;
        }

        return Quaternionf(q[3], q[0], q[1], q[2]);
    }

    Matrix4x4f  fromQuaternion(Quaternionf q)
    {
        float32 xx = q.x * q.x;
        float32 xy = q.x * q.y;
        float32 xz = q.x * q.z;
        float32 xw = q.x * q.s;

        float32 yy = q.y * q.y;
        float32 yz = q.y * q.z;
        float32 yw = q.y * q.s;

        float32 zz = q.z * q.z;
        float32 zw = q.z * q.s;

        return Matrix4x4f(1 - 2 * (yy + zz), 2 * (xy - zw),     2 * (xz + yw),     0,
                         2 * (xy + zw),     1 - 2 * (xx + zz), 2 * (yz - xw),     0,
                         2 * (xz - yw),     2 * (yz + xw),     1 - 2 * (xx + yy), 0,
                         0,                 0 ,                0,                 1);

    }

    float32 dotProduct(Quaternionf q1, Quaternionf q2)
    {
        return q1.s * q2.s + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
    }

    float32 getAngle(Quaternionf q1, Quaternionf q2)
    {
        return acos(dotProduct(q1, q2));
    }

    Vector3f rotate(Quaternionf q, Vector3f v)
    {
        Quaternionf m = Quaternionf(0, v);
        Quaternionf r = q * m * q.conjugate();
        return r.getVector3();
    }

    Vector4f rotate(Quaternionf q, Vector4f v)
    {
        Quaternionf m = Quaternionf(0, v.x, v.y, v.z);
        Quaternionf r = q * m * q.conjugate();
        return Vector4f(r.x, r.y, r.z, v.w);
    }

    Quaternionf lerp(Quaternionf q1, Quaternionf q2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        return (q1 * (1 - t) + q2 * t).normalize();
    }

    Quaternionf slerp(Quaternionf q1, Quaternionf q2, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = acos(dotProduct(q1, q2));
        float32 s = sin(angle);

        return (q1 * (sin((1 - t) * angle) / s) + q2 * (sin(t * angle) / s));
    }

    Quaternionf slerp(Quaternionf q1, Quaternionf q2, float32 angle, float32 t)
    {
        ASSERT(t >= 0, "Interpolation param t should be more than 0");
        ASSERT(t <= 1, "Interpolation param t should be less than 1");

        float32 s = sin(angle);

        return (q1 * (sin((1 - t) * angle) / s) + q2 * (sin(t * angle) / s));
    }

} // namespace Berserk