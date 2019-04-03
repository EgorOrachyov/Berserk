//
// Created by Egor Orachyov on 01.04.2019.
//

#include "Components/SceneComponent.h"
#include <Math/Mat4x4f.h>

namespace Berserk::EntitySystem
{

    SceneComponent::SceneComponent(const IObjectInitializer &objectInitializer)
            : IEntityComponent(objectInitializer),
              mScale(1.0f),
              mTranslation(),
              mRotation()
    {

    }

    void SceneComponent::addLocalRotation(float32 roll, float32 yaw, float32 pitch)
    {
        mRotation = mRotation * Quatf(roll, yaw, pitch);
    }

    void SceneComponent::addLocalRotation(const Vec3f &axis, float32 angle)
    {
        mRotation = mRotation * Quatf(axis, angle);
    }

    void SceneComponent::addLocalRotationX(float32 roll)
    {
        addLocalRotation(roll, 0, 0);
    }

    void SceneComponent::addLocalRotationY(float32 yaw)
    {
        addLocalRotation(0, yaw, 0);
    }

    void SceneComponent::addLocalRotationZ(float32 pitch)
    {
        addLocalRotation(0, 0, pitch);
    }

    void SceneComponent::addLocalTranslation(const Vec3f &translation)
    {
        mTranslation += translation;
    }

    void SceneComponent::addLocalScale(float32 factor)
    {
        mScale += factor;
    }

    void SceneComponent::update()
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

        mLocalTransform = mRotation.getMatrix();

        mLocalTransform *= mScale;

        mLocalTransform.m[3]  = mTranslation.x;
        mLocalTransform.m[7]  = mTranslation.y;
        mLocalTransform.m[11] = mTranslation.z;
        mLocalTransform.m[15] = 1.0f;
    }

    void SceneComponent::update(const SceneComponent &root)
    {
        update();

        mGlobalTransform = root.mGlobalTransform * mLocalTransform;
    }

} // namespace Berserk::EntitySystem