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

        void setPosition(const Vector3f &position);
        void setDirection(const Vector3f &direction);
        void setOrientation(const Vector3f &orientation);

        const Vector3f& getPosition() const;
        const Vector3f& getDirection() const;
        const Vector3f& getOrientation() const;

        DirectionalLightComponent* getComponent();

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        Vector3f mPosition;
        Vector3f mDirection;
        Vector3f mOrientation;

        DirectionalLightComponent mDirectionalComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_DIRECTIONALLIGHT_H