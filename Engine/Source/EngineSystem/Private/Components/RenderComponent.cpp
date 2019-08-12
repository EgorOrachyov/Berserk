//
// Created by Egor Orachyov on 2019-08-07.
//

#include "Components/RenderComponent.h"

namespace Berserk
{

    REFLECTABLE_OBJECT_INIT(RenderComponent);

    /*
    const TSharedPtr<RenderableSceneInfo>& RenderComponent::createSceneInfo()
    {
        // todo: add proper allocation with memory manager

        IAllocator& allocator = Allocator::get();
        auto info = allocator.engine_new_no_args<RenderableSceneInfo>();
        info->localToWorld = Transform::convertToMat4x4f(getWorldRotation(), getWorldPosition());
        info->renderable = getRenderable();
        info->castShadows = castShadows();
        info->applyCulling = applyCulling();
        info->drawWireframe = drawWireframe();
        info->drawWireframeOnly = drawWireframeOnly();
        info->drawBoundingVolume = drawBoundingVolume();
        info->isActive = isActive();

        mSceneInfo = TSharedPtr<RenderableSceneInfo>(info, &allocator);

        return mSceneInfo;
    }
    */

} // namespace Berserk
