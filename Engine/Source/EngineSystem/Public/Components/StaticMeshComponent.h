//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_STATICMESHCOMPONENT_H
#define BERSERK_STATICMESHCOMPONENT_H

#include <Components/MeshComponent.h>

namespace Berserk
{

    /**
     * Mesh geometry which supposed to be static [for performance]
     * @note All the geometry is preferred to be static
     */
    class ENGINE_API StaticMeshComponent final : public MeshComponent
    {
    public:

        REFLECTABLE_OBJECT(StaticMeshComponent);

    public:

#if DEBUG
        StaticMeshComponent() : MeshComponent() { }
#endif

        ~StaticMeshComponent() override = default;

    protected:

        /** @copydoc RenderComponent::renderComponent_markDirty() */
        void renderComponent_markDirty(uint32 flags) override
        {
            markDirty(flags);
        }

    };

} // namespace Berserk

#endif //BERSERK_STATICMESHCOMPONENT_H