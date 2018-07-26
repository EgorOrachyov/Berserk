//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_POINTLIGHT_H
#define BERSERKENGINE_POINTLIGHT_H

#include "Light.h"

namespace Berserk
{

    class PointLight : public Light
    {
    public:

        PointLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~PointLight() = default;

        void setPosition(const Vector3f &position);
        void setRadius(FLOAT32 radius);
        void setConstantAttenuation(FLOAT32 attenuation);
        void setLinearAttenuation(FLOAT32 attenuation);
        void setQuadraticAttenuation(FLOAT32 attenuation);

        Vector3f getPosition() const;
        FLOAT32 getRadius() const;
        FLOAT32 getConstantAttenuation(FLOAT32 attenuation) const;
        FLOAT32 getLinearAttenuation(FLOAT32 attenuation) const;
        FLOAT32 getQuadraticAttenuation(FLOAT32 attenuation) const;

    private:

        Vector3f mPosition;

        FLOAT32 mRadius;
        FLOAT32 mConstantAttenuation;
        FLOAT32 mLinearAttenuation;
        FLOAT32 mQuadraticAttenuation;

    };

} // namespace Berserk

#endif //BERSERKENGINE_POINTLIGHT_H