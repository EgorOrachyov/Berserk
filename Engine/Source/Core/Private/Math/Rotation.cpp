//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Rotation.h"
#include "Math/Mat4x4f.h"

namespace Berserk
{

    Rotation::Rotation() : mRotation(1, 0, 0, 0)
    {

    }

    Rotation::Rotation(const Quatf &q) : mRotation(q)
    {

    }

    Rotation::Rotation(const Mat4x4f &m) : mRotation(m)
    {

    }

    Rotation::Rotation(float32 roll, float32 yaw, float32 pitch) : mRotation(roll, yaw, pitch)
    {

    }

    Rotation& Rotation::operator=(const Rotation &r)
    {
        mRotation = r.mRotation;
        return *this;
    }

    Rotation Rotation::operator*(const Rotation &r)
    {
        return Rotation(mRotation * r.mRotation);
    }

    void Rotation::operator*=(const Rotation &r)
    {
        mRotation = mRotation * r.mRotation;
    }

    void Rotation::rotateX(float32 roll)
    {
        mRotation = Quatf(Vec3f::axisX, roll) * mRotation;
    }

    void Rotation::rotateY(float32 yaw)
    {
        mRotation = Quatf(Vec3f::axisY, yaw) * mRotation;
    }

    void Rotation::rotateZ(float32 pitch)
    {
        mRotation = Quatf(Vec3f::axisZ, pitch) * mRotation;
    }

    Mat4x4f Rotation::toMat4x4f() const
    {
        return mRotation.getMatrix();
    }

}