//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_TRANSFORM_H
#define BERSERK_TRANSFORM_H

#include "Public/Math/Rotation.h"
#include "Public/Math/MathUtility.h"

namespace Berserk
{

    /**
     * Transform class representing from object to world transformation
     * in the form os (scale, translation, rotation).
     *
     * Applies mentioned factors in the following order:
     *
     *      p = Matrix(translation) * Matrix(rotation) * Matrix(scale) * v
     *
     * Where: v - source point 4 component vector with w = 1
     *        p - result point 4 component vector with w = 1
     *
     */
    class Transform
    {
    public:

        /** Default transformation with 1.0 scale, id rotation and (0,0,0) translation */
        Transform();

        Transform(const Transform& t) = default;

        /** Default transformation with 1.0 scale, id rotation and chosen translation  */
        explicit Transform(const Vec3f& translation);

        /** From translation, rotation and scale == 1.0f (default) */
        Transform(const Vec3f& translation, const Quatf& rotation, float32 scale = 1.0f);

        /** From translation, rotation and scale == 1.0f (default) */
        Transform(const Vec3f& translation, const Rotation& rotation, float32 scale = 1.0f);

        /** From translation, rotation and scale == 1.0f (default) */
        Transform(const Vec3f& translation, const Mat4x4f& rotation, float32 scale = 1.0f);

        ~Transform() = default;

    public:

        Transform& operator= (const Transform& t) = default;

        /** Add translation */
        void operator += (const Vec3f& t);

        /** Sub translation */
        void operator -= (const Vec3f& t);

    public:

        /** @return Reference to scale to modify it */
        float32& scale();

        /** @return Reference to rotation to modify it */
        Rotation& rotation();

        /** @return Reference to translation to modify it */
        Vec3f& translation();

        /** @return Result transform in matrix form */
        Mat4x4f toMat4x4f();

    private:

        float32  mScale;
        Vec3f    mTranslation;
        Rotation mRotation;

    };

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

} // namespace Berserk

#endif //BERSERK_TRANSFORM_H