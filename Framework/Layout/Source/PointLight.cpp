//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/PointLight.h"
#include "Managers/SceneManager.h"
#include "Render/RenderSystem.h"

namespace Berserk
{

    PointLight::PointLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        setLightIntensity(Vector3f(1.0));
        setPosition(Vector3f(0.0));

        setRadius(1);
        setConstantAttenuation(1);
        setLinearAttenuation(0);
        setQuadraticAttenuation(0);
    }

    void PointLight::setLightIntensity(const Vector3f &intensity)
    {
        if (mIsEditable)
        {
            mPointComponent.mLightIntensity = intensity;
        }
    }

    void PointLight::setPosition(const Vector3f &position)
    {
        if (mIsEditable)
        {
            mPosition = position;
        }
    }

    void PointLight::setRadius(FLOAT32 radius)
    {
        ASSERT(radius > 0, "Point light radius should be more than 0");

        if (mIsEditable)
        {
            mPointComponent.mRadius = radius;
        }
    }

    void PointLight::setConstantAttenuation(FLOAT32 attenuation)
    {
        ASSERT(attenuation > 0, "Constant attenuation should be more than 0");

        if (mIsEditable)
        {
            mPointComponent.mConstantAttenuation = attenuation;
        }
    }

    void PointLight::setLinearAttenuation(FLOAT32 attenuation)
    {
        ASSERT(attenuation >= 0, "Linear attenuation should be more than 0");

        if (mIsEditable)
        {
            mPointComponent.mLinearAttenuation = attenuation;
        }
    }

    void PointLight::setQuadraticAttenuation(FLOAT32 attenuation)
    {
        ASSERT(attenuation >= 0, "Quadratic attenuation should be more than 0");

        if (mIsEditable)
        {
            mPointComponent.mQuadraticAttenuation = attenuation;
        }
    }

    const Vector3f& PointLight::getLightIntensity() const
    {
        return mPointComponent.mLightIntensity;
    }

    const Vector3f & PointLight::getPosition() const
    {
        return mPosition;
    }

    FLOAT32 PointLight::getRadius() const
    {
        return mPointComponent.mRadius;
    }

    FLOAT32 PointLight::getConstantAttenuation(FLOAT32 attenuation) const
    {
        return mPointComponent.mConstantAttenuation;
    }

    FLOAT32 PointLight::getLinearAttenuation(FLOAT32 attenuation) const
    {
        return mPointComponent.mLinearAttenuation;
    }

    FLOAT32 PointLight::getQuadraticAttenuation(FLOAT32 attenuation) const
    {
        return mPointComponent.mQuadraticAttenuation;
    }

    PointLightComponent* PointLight::getComponent()
    {
        return &mPointComponent;
    }

    void PointLight::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        Light::process(delta, rootTransformation);

        if (mIsActive)
        {
            mPointComponent.mCastShadows = mCastShadows;
            mPointComponent.mPosition = rootTransformation * (mTransformation * Vector4f(mPosition.x, mPosition.y, mPosition.z, 1));

            gRenderSystem->queueLightSource(this);
        }
    }

} // namespace Berserk