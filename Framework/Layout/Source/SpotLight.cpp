//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/SpotLight.h"
#include "Managers/SceneManager.h"
#include "Math/UtilityVectors.h"
#include <cmath>

namespace Berserk
{

    SpotLight::SpotLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        setPosition(Vector3f(0,0,0));
        setDirection(Vector3f(0,1,0));
        setCutoff(toRadians(10));
        setInnerCutoff(toRadians(6.5));
        setOuterCutoff(toRadians(10.5));
        setAttenuationExponent(8);
    }

    void SpotLight::setPosition(const Vector3f &position)
    {
        if (mIsEditable)
        {
            mPosition = position;
        }
    }

    void SpotLight::setDirection(const Vector3f &direction)
    {
        ASSERT(!(direction == Vector3f(0,0,0)), "Direction should not be 0 vector in Spot Light");

        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    void SpotLight::setInnerCutoff(FLOAT32 angle)
    {
        if (mIsEditable)
        {
            mInnerCutoff = angle;
            mSpotComponent.mEpsilon = cosf(mInnerCutoff) - mSpotComponent.mOuterCutoff;
        }
    }

    void SpotLight::setOuterCutoff(FLOAT32 angle)
    {
        if (mIsEditable)
        {
            mOuterCutoff = angle;
            mSpotComponent.mOuterCutoff = cosf(mOuterCutoff);
            mSpotComponent.mEpsilon = cosf(mInnerCutoff) - mSpotComponent.mOuterCutoff;
        }
    }

    void SpotLight::setCutoff(FLOAT32 angle)
    {
        if (mIsEditable)
        {
            mCutoff = angle;
            mSpotComponent.mCutoff = cosf(angle);
            mSpotComponent.mEpsilon = cosf(mInnerCutoff) - cosf(mOuterCutoff);
        }
    }

    void SpotLight::setAttenuationExponent(FLOAT32 attenuation)
    {
        if (mIsEditable)
        {
            mSpotComponent.mAttenuationExponent = attenuation;
        }
    }

    Vector3f SpotLight::getDirection() const
    {
        return mDirection;
    }

    FLOAT32 SpotLight::getCutoff() const
    {
        return mCutoff;
    }

    FLOAT32 SpotLight::getInnerCutoff() const
    {
        return mInnerCutoff;
    }

    FLOAT32 SpotLight::getOuterCutoff() const
    {
        return mOuterCutoff;
    }

    FLOAT32 SpotLight::getAttenuationExponent() const
    {
        return mSpotComponent.mAttenuationExponent;
    }

    void SpotLight::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        Light::process(delta, rootTransformation);

        if (mIsActive)
        {
            Matrix4x4f ress = rootTransformation * mTransformation;

            mSpotComponent.mCastShadows = mCastShadows;
            mSpotComponent.mLightIntensity = mLightIntensity;
            mSpotComponent.mDirection = normalize(ress * Vector4f(mDirection.x, mDirection.y, mDirection.z, 0));
            mSpotComponent.mPosition = ress * Vector4f(mPosition.x, mPosition.y, mPosition.z, 1);

            gSceneManager->getRenderManager().queueLight(&mSpotComponent);
        }
    }

} // namespace Berserk