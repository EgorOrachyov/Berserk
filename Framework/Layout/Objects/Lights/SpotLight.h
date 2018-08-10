//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_SPOTLIGHT_H
#define BERSERKENGINE_SPOTLIGHT_H

#include "Light.h"
#include "Components/SpotLightComponent.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    class ENGINE_EXPORT SpotLight : public Light
    {
    public:

        SpotLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~SpotLight() = default;

        void setPosition(const Vector3f &position);
        void setDirection(const Vector3f &direction);
        void setInnerCutoff(FLOAT32 angle);
        void setOuterCutoff(FLOAT32 angle);
        void setCutoff(FLOAT32 angle);
        void setAttenuationExponent(FLOAT32 attenuation);

        Vector3f getDirection() const;
        FLOAT32 getCutoff() const;
        FLOAT32 getInnerCutoff() const;
        FLOAT32 getOuterCutoff() const;
        FLOAT32 getAttenuationExponent() const;

        SpotLightComponent* getComponent();

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        Vector3f mPosition;
        Vector3f mDirection;
        SpotLightComponent mSpotComponent;

        FLOAT32 mCutoff;
        FLOAT32 mInnerCutoff;
        FLOAT32 mOuterCutoff;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SPOTLIGHT_H