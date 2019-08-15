//
// Created by Egor Orachyov on 2019-08-08.
//

#ifndef BERSERK_MESHCOMPONENT_H
#define BERSERK_MESHCOMPONENT_H

#include <Components/RenderComponent.h>

namespace Berserk
{

    /** Mesh component is collection of triangles with materials which could be rendered on sceen */
    class MeshComponent : public RenderComponent
    {
    public:

        REFLECTABLE_OBJECT(MeshComponent);

    public:

#if DEBUG
        MeshComponent() : RenderComponent() { }
#endif

        ~MeshComponent() override = default;

        // todo: add mesh utility functions

        /** Set renderable data for this mesh */
        void setRenderable(const TSharedPtr<Renderable>& renderable);

        /** @copydoc RenderComponent::getRenderable() */
        const TSharedPtr<Renderable> &getRenderable() const override;

    private:

        /** Mesh default renderable data */
        TSharedPtr<Renderable> mRenderable;

    };

} // namespace Berserk

#endif //BERSERK_MESHCOMPONENT_H