//
// Created by Egor Orachyov on 25.03.2019.
//

#include "Base/IRenderComponent.h"

namespace Berserk::EntitySystem
{

    IRenderComponent::IRenderComponent()
            : IObject(),
              mRenderComponentType(eRCT_NOT_RENDER_NODE),
              mMaxViewDistance(0.0f),
              mLodRatio(0.0f),
              mLodLevels(0),
              mRenderComponentFlags(0)
    {

    }

    void IRenderComponent::setRenderComponentFlag(RenderComponentFlag flag, bool active)
    {
        if (active) mRenderComponentFlags |= flag;
        else mRenderComponentFlags &= ~flag;
    }

} // namespace Berserk::EntitySystem