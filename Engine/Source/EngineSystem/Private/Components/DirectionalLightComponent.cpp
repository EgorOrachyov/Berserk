//
// Created by Egor Orachyov on 2019-07-31.
//

#include "Components/DirectionalLightComponent.h"

namespace Berserk
{

    REFLECTABLE_OBJECT_INIT(DirectionalLightComponent);

    const TSharedPtr<DirLightSceneInfo>& DirectionalLightComponent::createSceneInfo()
    {
        // todo: add proper allocation with memory manager

        IAllocator& allocator = Allocator::get();
        auto info = allocator.engine_new_no_args<DirLightSceneInfo>();
        info->castShadows = castShadows();
        info->isActive = isActive();
        info->distanceOfAction = getMaxLightDistance();
        info->distanceOfActionSq = info->distanceOfAction * info->distanceOfAction;
        info->lightColor = getLightColor();
        info->localToWorld = Transform::convertToMat4x4f(getWorldRotation(), getWorldPosition());
        info->worldDirection = getWorldDirection();

        mSceneInfo = TSharedPtr<DirLightSceneInfo>(info, &allocator);

        return mSceneInfo;
    }

} // namespace Berserk