//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/DirectionalLight.h"
#include "Math/UtilityMatrices.h"
#include "Render/RenderSystem.h"

namespace Berserk
{

    DirectionalLight::DirectionalLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        mPosition = Vector3f(0,0,0);
        mDirection = Vector3f(0,0,-1);
        mOrientation = Vector3f(0,1,0);
    }

    void DirectionalLight::setPosition(const Vector3f &position)
    {
        if (mIsEditable)
        {
            mPosition = position;
        }
    }

    void DirectionalLight::setDirection(const Vector3f &direction)
    {
        ASSERT(!(direction == Vector3f(0,0,0)), "Direction should not be 0 vector in Directional Light");

        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    void DirectionalLight::setOrientation(const Vector3f &orientation)
    {
        ASSERT(!(orientation == Vector3f(0,0,0)), "Orientation should not be 0 vector in Directional Light");

        if (mIsEditable)
        {
            mOrientation = orientation;
        }
    }

    const Vector3f& DirectionalLight::getPosition() const
    {
        return mPosition;
    }

    const Vector3f& DirectionalLight::getDirection() const
    {
        return mDirection;
    }

    const Vector3f& DirectionalLight::getOrientation() const
    {
        return mOrientation;
    }

    DirectionalLightComponent* DirectionalLight::getComponent()
    {
        return &mDirectionalComponent;
    }

    void DirectionalLight::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        Light::process(delta, rootTransformation);

        if (mIsActive)
        {
            Matrix4x4f ress = rootTransformation * mTransformation;

            mDirectionalComponent.mLightIntensity = mLightIntensity;
            mDirectionalComponent.mDirection = ress * Vector4f(mDirection.x, mDirection.y, mDirection.z, 0);

            gRenderSystem->queueLightSource(this);

            if (mCastShadows)
            {
                Vector3f position = Vector3f(ress * Vector4f(mPosition.x, mPosition.y, mPosition.z, 1));
                Vector3f target = position + Vector3f(mDirectionalComponent.mDirection);
                Vector3f orientation = Vector3f(ress * Vector4f(mOrientation.x, mOrientation.y, mOrientation.z, 0));
                mShadowComponent.mView = lookAt(position,target,orientation);

                gRenderSystem->queueShadowLightSource(this);
            }
        }
    }

} // namespace Berserk