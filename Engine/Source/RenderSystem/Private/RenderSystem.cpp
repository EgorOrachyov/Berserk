//
// Created by Egor Orachyov on 2019-08-15.
//

#include "RenderSystem.h"

namespace Berserk
{

    TSharedPtr<IRenderScene> RenderSystem::createScene()
    {
        return TSharedPtr<IRenderScene>();
    }

    void RenderSystem::addScene(const TSharedPtr<IRenderScene> &scene)
    {

    }

    void RenderSystem::removeScene(const TSharedPtr<IRenderScene> &scene) \
    {

    }

} // namespace Berserk