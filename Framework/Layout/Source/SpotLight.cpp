//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/SpotLight.h"

namespace Berserk
{

    SpotLight::SpotLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        mDirection = Vector3f(0, 0, 1);
        mInnerConeAngle = 0;
        mOuterConeAngle = 1;
        mAttenuationExponent = 0;
    }

    void SpotLight::setDirection(const Vector3f &direction)
    {
        ASSERT(!(direction == Vector3f(0,0,0)), "Direction should not be 0 vector in Spot Light");

        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    void SpotLight::setInnerConeAngle(FLOAT32 angle)
    {
        ASSERT(angle >= 0, "Inner cone angle should be more than 0")

        if (mIsEditable)
        {
            mInnerConeAngle = angle;
        }
    }

    void SpotLight::setOuterConeAngle(FLOAT32 angle)
    {
        ASSERT(angle >= 0, "Outer cone angle should be more than 0")

        if (mIsEditable)
        {
            mOuterConeAngle = angle;
        }
    }

    void SpotLight::setAttenuationExponent(FLOAT32 attenuation)
    {
        if (mIsEditable)
        {
            mAttenuationExponent = attenuation;
        }
    }

    Vector3f SpotLight::getDirection() const
    {
        return mDirection;
    }

    FLOAT32 SpotLight::getInnerConeAngle() const
    {
        return mInnerConeAngle;
    }

    FLOAT32 SpotLight::getOuterConeAngle() const
    {
        return mOuterConeAngle;
    }

    FLOAT32 SpotLight::getAttenuationExponent() const
    {
        return mAttenuationExponent;
    }

} // namespace Berserk