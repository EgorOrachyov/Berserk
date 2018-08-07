//
// Created by Egor Orachyov on 29.07.2018.
//

#include "Render/RenderSystem.h"

namespace Berserk
{

    RenderSystem& RenderSystem::getRenderSystemRef()
    {
        return *gRenderSystem;
    }

    RenderSystem* RenderSystem::getRenderSystemPtr()
    {
        return gRenderSystem;
    }

    RenderSystem *gRenderSystem;

} // namespace Berserk