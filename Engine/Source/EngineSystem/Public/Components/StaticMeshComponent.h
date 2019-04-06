//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_STATICMESHCOMPONENT_H
#define BERSERK_STATICMESHCOMPONENT_H

#include <Components/IPrimitiveComponent.h>

namespace Berserk::EngineSystem
{
    using namespace RenderSystem;

    class ENGINE_EXPORT StaticMeshComponent : public IPrimitiveComponent
    {
    public:

        GENERATE_CLASS_BODY(StaticMeshComponent);

        explicit StaticMeshComponent(const IObjectInitializer& objectInitializer);

        ~StaticMeshComponent() override = default;

    protected:


    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_STATICMESHCOMPONENT_H