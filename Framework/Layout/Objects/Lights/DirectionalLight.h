//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_DIRECTIONALLIGHT_H
#define BERSERKENGINE_DIRECTIONALLIGHT_H

#include "Light.h"
#include "Components/DirectionalLightComponent.h"
#include "Essential/UsageDescriptors.h"

namespace ENGINE_EXPORT Berserk
{

    class DirectionalLight : public Light
    {
    public:

        DirectionalLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~DirectionalLight() = default;

        void setLightIntensity(const Vector3f &intensity) override;
        void setPosition(const Vector3f &position);
        void setDirection(const Vector3f &direction);
        void setOrientation(const Vector3f &orientation);
        void setFarShadowPlane(FLOAT32 distance);

        const Vector3f& getLightIntensity() const override;
        const Vector3f& getPosition() const;
        const Vector3f& getDirection() const;
        const Vector3f& getOrientation() const;
        FLOAT32 getFarShadowDistance() const;

        DirectionalLightComponent* getComponent();
        ShadowCasterComponent* getShadowCaster();

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        Vector3f mPosition;
        Vector3f mDirection;
        Vector3f mOrientation;

        FLOAT32 mFarDistance;

        ShadowCasterComponent mShadowComponent;
        DirectionalLightComponent mDirectionalComponent;
    };

} // namespace Berserk

#endif //BERSERKENGINE_DIRECTIONALLIGHT_H