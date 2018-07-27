//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_SPOTLIGHT_H
#define BERSERKENGINE_SPOTLIGHT_H

#include "Light.h"
#include "Components/SpotLightComponent.h"

namespace Berserk
{

    class SpotLight : public Light
    {
    public:

        SpotLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~SpotLight() = default;

        void setPosition(const Vector3f &position);
        void setDirection(const Vector3f &direction);
        void setInnerConeAngle(FLOAT32 angle);
        void setOuterConeAngle(FLOAT32 angle);
        void setAttenuationExponent(FLOAT32 attenuation);

        Vector3f getDirection() const;
        FLOAT32 getInnerConeAngle() const;
        FLOAT32 getOuterConeAngle() const;
        FLOAT32 getAttenuationExponent() const;

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        Vector3f mPosition;
        Vector3f mDirection;
        SpotLightComponent mSpotComponent;

        FLOAT32 mInnerConeAngle;
        FLOAT32 mOuterConeAngle;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SPOTLIGHT_H