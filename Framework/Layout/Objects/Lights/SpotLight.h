//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_SPOTLIGHT_H
#define BERSERKENGINE_SPOTLIGHT_H

#include "Light.h"

namespace Berserk
{

    class SpotLight : public Light
    {
    public:

        SpotLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~SpotLight() = default;

        void setDirection(const Vector3f &direction);
        void setInnerConeAngle(FLOAT32 angle);
        void setOuterConeAngle(FLOAT32 angle);
        void setAttenuationExponent(FLOAT32 attenuation);

        Vector3f getDirection() const;
        FLOAT32 getInnerConeAngle() const;
        FLOAT32 getOuterConeAngle() const;
        FLOAT32 getAttenuationExponent() const;

    private:

        Vector3f mDirection;
        FLOAT32 mInnerConeAngle;
        FLOAT32 mOuterConeAngle;
        FLOAT32 mAttenuationExponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SPOTLIGHT_H