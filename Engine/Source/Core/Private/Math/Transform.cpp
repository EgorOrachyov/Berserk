//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Transform.h"
#include "Math/Mat4x4f.h"

namespace Berserk
{

    Transform::Transform()
            : mScale(1.0f) , mTranslation(), mRotation()
    {

    }

    Transform::Transform(const Vec3f &translation)
            : mScale(1.0f) , mTranslation(translation), mRotation()
    {

    }

    Transform::Transform(const Vec3f &translation, const Quatf &rotation, float32 scale)
            : mScale(scale) , mTranslation(translation), mRotation(rotation)
    {

    }

    Transform::Transform(const Vec3f &translation, const Rotation &rotation, float32 scale)
            : mScale(scale) , mTranslation(translation), mRotation(rotation)
    {

    }

    Transform::Transform(const Vec3f &translation, const Mat4x4f &rotation, float32 scale)
            : mScale(scale) , mTranslation(translation), mRotation(rotation)
    {

    }

    void Transform::operator+=(const Vec3f &t)
    {
        mTranslation += t;
    }

    void Transform::operator-=(const Vec3f &t)
    {
        mTranslation -= t;
    }

    float32& Transform::scale()
    {
        return mScale;
    }

    Rotation& Transform::rotation()
    {
        return mRotation;
    }

    Vec3f& Transform::translation()
    {
        return mTranslation;
    }

    Mat4x4f Transform::toMat4x4f()
    {
        // The order of applying an arbitrary transformation
        // For some vector v:
        //
        // p = T * R * S * v,
        //
        // Where T - translation matrix,
        //       R - rotation matrix,
        //       S - scale matrix
        //
        // Matrix indices
        //
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15

        Mat4x4f t = mRotation.toMat4x4f();

        t *= mScale;
        t.m[15] = 1.0f;

        t.m[3]  = mTranslation.x;
        t.m[7]  = mTranslation.y;
        t.m[11] = mTranslation.z;

        return t;
    }

}