//
// Created by Egor Orachyov on 01.07.2018.
//

#include "../Maths/UtilityQuaternions.h"
#include "../Maths/UtilityVectors.h"

#include <cmath>

namespace Berserk
{

    Quaternion fromVector(Vector3 axis, float32 angle)
    {
        return Quaternion(cos(angle / 2), normalize(axis) * sin(angle / 2));
    }

    Quaternion fromEuler(float32 yaw, float32 pitch, float32 roll)
    {

    }

    Quaternion fromMatrix(Matrix4x4 m)
    {

    }

    Matrix4x4  fromQuaternion(Quaternion q)
    {

    }

    Vector3 rotate(Quaternion q, Vector3 v)
    {
        Quaternion m = Quaternion(0, v);
        Quaternion r = q * m * q.Conjugate();
        return r.GetVector3();
    }

    Vector4 rotate(Quaternion q, Vector4 v)
    {
        Quaternion m = Quaternion(0, v.x, v.y, v.z);
        Quaternion r = q * m * q.Conjugate();
        Vector3 t = r.GetVector3();
        return Vector4(t.x, t.y, t.z, v.w);
    }

    Quaternion lerp(Quaternion q1, Quaternion q2, float32 t)
    {

    }

    Quaternion slerp(Quaternion q1, Quaternion q2, float32 t)
    {

    }

} // namespace Berserk