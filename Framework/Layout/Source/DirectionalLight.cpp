//
// Created by Egor Orachyov on 26.07.2018.
//

#include "Objects/Lights/DirectionalLight.h"

namespace Berserk
{

    DirectionalLight::DirectionalLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {
        mDirection = Vector3f(0,0,1);
    }

    void DirectionalLight::setDirection(const Vector3f &direction)
    {
        ASSERT(!(direction == Vector3f(0,0,0)), "Direction should be not 0 vector in Directional Light");

        if (mIsEditable)
        {
            mDirection = direction;
        }
    }

    Vector3f DirectionalLight::getDirection() const
    {
        return mDirection;
    }

} // namespace Berserk