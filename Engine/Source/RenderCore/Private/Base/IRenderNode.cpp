//
// Created by Egor Orachyov on 25.03.2019.
//

#include "Base/IRenderNode.h"

namespace Berserk
{

    IRenderNode::IRenderNode() : mRenderNodeType(eRNT_NOT_RENDER_NODE),
                                 mMaxViewDistance(0.0f),
                                 mLodRatio(0.0f),
                                 mLodLevels(0),
                                 mRenderNodeFlags(0)
    {

    }

    void IRenderNode::setRenderNodeFlag(RenderNodeFlag flag, bool active)
    {
        if (active) mRenderNodeFlags |= flag;
        else mRenderNodeFlags &= ~flag;
    }

} // namespace Berserk