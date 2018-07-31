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

        void setDirection(const Vector3f &direction);
        Vector3f getDirection() const;

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

    private:

        Vector3f mDirection;
        DirectionalLightComponent mDirectionalComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_DIRECTIONALLIGHT_H