//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_POINTLIGHT_H
#define BERSERKENGINE_POINTLIGHT_H

#include "Light.h"
#include "Components/PointLightComponent.h"
#include "Components/PointShadowCasterComponent.h"
#include "Essential/UsageDescriptors.h"

namespace Berserk
{

    class ENGINE_EXPORT PointLight : public Light
    {
    public:

        PointLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~PointLight() = default;

        void setLightIntensity(const Vector3f &intensity) override;
        void setPosition(const Vector3f &position);

        void setRadius(FLOAT32 radius);
        void setConstantAttenuation(FLOAT32 attenuation);
        void setLinearAttenuation(FLOAT32 attenuation);
        void setQuadraticAttenuation(FLOAT32 attenuation);

        const Vector3f& getLightIntensity() const override;
        const Vector3f& getPosition() const;

        FLOAT32 getRadius() const;
        FLOAT32 getConstantAttenuation(FLOAT32 attenuation) const;
        FLOAT32 getLinearAttenuation(FLOAT32 attenuation) const;
        FLOAT32 getQuadraticAttenuation(FLOAT32 attenuation) const;

        PointLightComponent* getComponent();
        PointShadowCasterComponent* getShadowCaster();

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        Vector3f mPosition;

        PointShadowCasterComponent mShadowComponent;
        PointLightComponent mPointComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_POINTLIGHT_H