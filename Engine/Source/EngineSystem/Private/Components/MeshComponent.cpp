//
// Created by Egor Orachyov on 2019-08-08.
//

#include "Components/MeshComponent.h"

namespace Berserk
{

    REFLECTABLE_OBJECT_INIT(MeshComponent);

    void MeshComponent::setRenderable(const Berserk::TSharedPtr<Berserk::Renderable> &renderable)
    {
        mRenderable = renderable;
        renderComponent_markDirty(ERenderDirtyFlags::RDF_Renderable);
    }

    const TSharedPtr<Renderable> &MeshComponent::getRenderable() const
    {
        return mRenderable;
    }

} // namespace Berserk