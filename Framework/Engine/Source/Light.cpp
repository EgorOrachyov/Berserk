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
        mCastLightShafts = false;
    }

    void Light::toggle()
    {
        if (mIsToggleable)
        {
            mIsActive = !mIsActive;
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

    void Light::setCastLightShafts(bool setIn)
    {
        if (mIsEditable)
        {
            mCastLightShafts = setIn;
        }
    }

    bool Light::isToggleable() const
    {
        return mIsToggleable;
    }

    bool Light::castShadows() const
    {
        return mCastShadows;
    }

    bool Light::castLightShafts() const
    {
        return mCastLightShafts;
    }

} // namespace Berserk