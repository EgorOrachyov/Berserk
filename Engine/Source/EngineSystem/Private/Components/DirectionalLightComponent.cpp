//
// Created by Egor Orachyov on 2019-07-31.
//

#include "Components/DirectionalLightComponent.h"
#include <Rendering/RenderSceneObjects.h>

namespace Berserk
{

    REFLECTABLE_OBJECT_INIT(DirectionalLightComponent);

    class DirLightSceneInfo* DirectionalLightComponent::allocateSceneInfo() const
    {
        // todo: add memory managed allocation
        return (DirLightSceneInfo*) Allocator::get().allocate(sizeof(DirLightSceneInfo));
    }

} // namespace Berserk