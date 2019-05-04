//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_DIRECTIONALLIGHTCOMPONENT_H
#define BERSERK_DIRECTIONALLIGHTCOMPONENT_H

#include <Components/LightSourceComponent.h>

namespace Berserk::Engine
{

    class DirectionalLightComponent : public LightSourceComponent
    {
    public:

        GENERATE_CLASS_BODY(DirectionalLightComponent);

        DirectionalLightComponent(const IObjectInitializer& objectInitializer);

        ~DirectionalLightComponent() override;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_DIRECTIONALLIGHTCOMPONENT_H