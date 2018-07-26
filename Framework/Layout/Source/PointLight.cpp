//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/PointLight.h"

namespace Berserk
{

    PointLight::PointLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        mPosition = Vector3f(0, 0, 0);

        mRadius = 1;
        mConstantAttenuation = 1;
        mLinearAttenuation = 0;
        mQuadraticAttenuation = 0;
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
            mRadius = radius;
        }
    }

    void PointLight::setConstantAttenuation(FLOAT32 attenuation)
    {
        ASSERT(attenuation > 0, "Constant attenuation should be more than 0");

        if (mIsEditable)
        {
            mConstantAttenuation = attenuation;
        }
    }

    void PointLight::setLinearAttenuation(FLOAT32 attenuation)
    {
        ASSERT(attenuation >= 0, "Linear attenuation should be more than 0");

        if (mIsEditable)
        {
            mLinearAttenuation = attenuation;
        }
    }

    void PointLight::setQuadraticAttenuation(FLOAT32 attenuation)
    {
        ASSERT(attenuation >= 0, "Quadratic attenuation should be more than 0");

        if (mIsEditable)
        {
            mQuadraticAttenuation = attenuation;
        }
    }

    Vector3f PointLight::getPosition() const
    {
        return mPosition;
    }

    FLOAT32 PointLight::getRadius() const
    {
        return mRadius;
    }

    FLOAT32 PointLight::getConstantAttenuation(FLOAT32 attenuation) const
    {
        return mConstantAttenuation;
    }

    FLOAT32 PointLight::getLinearAttenuation(FLOAT32 attenuation) const
    {
        return mLinearAttenuation;
    }

    FLOAT32 PointLight::getQuadraticAttenuation(FLOAT32 attenuation) const
    {
        return mQuadraticAttenuation;
    }

} // namespace Berserk