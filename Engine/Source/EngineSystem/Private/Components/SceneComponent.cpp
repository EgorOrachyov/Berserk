//
// Created by Egor Orachyov on 01.04.2019.
//

#include "Components/SceneComponent.h"
#include <Math/Mat4x4f.h>

namespace Berserk::Engine
{

    SceneComponent::SceneComponent(const IObjectInitializer &objectInitializer)
            : IEntityComponent(objectInitializer),
              mAttachedComponents(INITIAL_COMPONENTS_COUNT, objectInitializer.getAllocator())
    {

    }

    void SceneComponent::addLocalRotation(float32 roll, float32 yaw, float32 pitch)
    {
        mRotation = Quatf(roll, yaw, pitch) * mRotation;
    }

    void SceneComponent::addLocalRotation(const Vec3f &axis, float32 angle)
    {
        mRotation = Quatf(axis, angle) * mRotation;
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

    void SceneComponent::addGlobalTranslation(const Vec3f &translation)
    {
        mTranslation += Vec3f(mOwnerComponent->mGlobalTransform * Vec4f(translation, 0.0f));
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

        if (mOwnerComponent)
        {
            mGlobalTransform = mOwnerComponent->mGlobalTransform * mLocalTransform;
        }
        else
        {
            mGlobalTransform = mLocalTransform;
        }

        PUSH("It is scene component '%s'", mObjectName.get());

        onTransformUpdate();
    }

    void SceneComponent::traverse()
    {
        update();

        for (uint32 i = 0; i < mAttachedComponents.getSize(); i++)
        {
            mAttachedComponents[i]->traverse();
        }
    }

    void SceneComponent::attachSceneComponent(SceneComponent *component)
    {
        FAIL(component, "Null pointer child component");

        if (!component->mIsAttachable)
        {
            return;
        }

        if (component->mOwnerEntity == nullptr)
        {
            mAttachedComponents += component;
            component->mOwnerComponent = this;
        }
        else
        {
            WARNING("An attempt to attach an component with owner [this: '%s'][component: '%s']",
                    getName(), component->getName());
        }
    }

} // namespace Berserk::EntitySystem