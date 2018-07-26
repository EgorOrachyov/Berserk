//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/Light.h"

namespace Berserk
{

    Light::Light(const CStaticString &name, FLOAT32 lifeTime) : Actor(name, lifeTime)
    {
        mIsToggleable = true;
        mCastShadows = false;

        mLightIntensity = Vector3f(1,1,1);
    }

    Light::~Light()
    {

    }

    void Light::toggle()
    {
        if (mIsToggleable)
        {
            mIsActive = !mIsActive;
        }
    }

    void Light::setLightIntensity(const Vector3f &intensity)
    {
        if (mIsEditable)
        {
            mLightIntensity = intensity;
        }
    }

    void Light::setToggleable(bool setIn)
    {
        mIsToggleable = setIn;
    }

    void Light::setCastShadows(bool setIn)
    {
        if (mIsEditable)
        {
            mCastShadows = setIn;
        }
    }

    Vector3f Light::getLightIntensity() const
    {
        return mLightIntensity;
    }

    bool Light::isToggleable() const
    {
        return mIsToggleable;
    }

    bool Light::isCastShadows() const
    {
        return mCastShadows;
    }

} // namespace Berserk