//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/DirectionalLight.h"
#include "Math/UtilityMatrices.h"
#include "Render/RenderSystem.h"

#define DIR_SHADOW_VIEW_LEFT    (-16.0f)
#define DIR_SHADOW_VIEW_RIGHT   16.0f
#define DIR_SHADOW_VIEW_BOTTOM  (-16.0f)
#define DIR_SHADOW_VIEW_TOP     16.0f
#define DIR_SAHDOW_VIEW_NEAR    0.0f

namespace Berserk
{
    DirectionalLight::DirectionalLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        setLightIntensity(Vector3f(1.0));
        setPosition(Vector3f(0.0));
        setDirection(Vector3f(0,0,-1));
        setOrientation(Vector3f(0,1,0));
        setFarShadowPlane(25.0);
    }

    void DirectionalLight::setLightIntensity(const Vector3f &intensity)
    {
        if (mIsEditable)
        {
            mDirectionalComponent.mLightIntensity = intensity;
        }
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
        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    void DirectionalLight::setOrientation(const Vector3f &orientation)
    {
        if (mIsEditable)
        {
            mOrientation = orientation;
        }
    }

    void DirectionalLight::setFarShadowPlane(FLOAT32 distance)
    {
        if (mIsEditable)
        {
            mFarDistance = distance;
            mShadowComponent.mProjection = orthographic(DIR_SHADOW_VIEW_LEFT,
                                                        DIR_SHADOW_VIEW_RIGHT,
                                                        DIR_SHADOW_VIEW_BOTTOM,
                                                        DIR_SHADOW_VIEW_TOP,
                                                        DIR_SAHDOW_VIEW_NEAR,
                                                        mFarDistance);
        }
    }

    const Vector3f& DirectionalLight::getLightIntensity() const
    {
        return mDirectionalComponent.mLightIntensity;
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

    FLOAT32 DirectionalLight::getFarShadowDistance() const
    {
        return mFarDistance;
    }

    DirectionalLightComponent* DirectionalLight::getComponent()
    {
        return &mDirectionalComponent;
    }

    ShadowCasterComponent* DirectionalLight::getShadowCaster()
    {
        return &mShadowComponent;
    }

    void DirectionalLight::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        if (mIsActive)
        {
            processActor(delta, rootTransformation);
            processDirectionalLight();
        }
    }

    void DirectionalLight::processDirectionalLight()
    {
        const Matrix4x4f& ress = mWorldTransformation;

        mDirectionalComponent.mDirection = ress * Vector4f(mDirection.x, mDirection.y, mDirection.z, 0);

        if (mCastShadows)
        {
            Vector3f position = Vector3f(ress * Vector4f(mPosition.x, mPosition.y, mPosition.z, 1));
            Vector3f target = position + Vector3f(mDirectionalComponent.mDirection);
            Vector3f orientation = Vector3f(ress * Vector4f(mOrientation.x, mOrientation.y, mOrientation.z, 0));
            mShadowComponent.mView = lookAt(position,target,orientation);

            gRenderSystem->queueShadowLightSource(this);
        }
        else
        {
            gRenderSystem->queueLightSource(this);
        }
    }

} // namespace Berserk