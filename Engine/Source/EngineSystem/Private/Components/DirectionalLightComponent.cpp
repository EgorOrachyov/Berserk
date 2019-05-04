//
// Created by Egor Orachyov on 03.04.2019.
//

#include "Components/DirectionalLightComponent.h"

namespace Berserk::Engine
{

    DirectionalLightComponent::DirectionalLightComponent(const IObjectInitializer &objectInitializer)
            : LightComponent(objectInitializer)
    {
        mLightSourceType = eLST_DIRECTIONAL_LIGHT;
    }

    void DirectionalLightComponent::onTransformUpdate()
    {
        PUSH("It is dir light component '%s'", mObjectName.get());

        // Update light source direction vector
        // to handle updated movements in current frame

        mLocalDirection = Vec3f(toLocalSpace() * Vec4f(Vec3f::axisZ, 0.0f));
        mLocalUp = Vec3f(toLocalSpace() * Vec4f(Vec3f::axisY, 0.0f));

        mWorldDirection = Vec3f(toGlobalSpace() * Vec4f(Vec3f::axisZ, 0.0f));
        mWorldUp = Vec3f(toGlobalSpace() * Vec4f(Vec3f::axisY, 0.0f));
    }

} // namespace Berserk::EntitySystem