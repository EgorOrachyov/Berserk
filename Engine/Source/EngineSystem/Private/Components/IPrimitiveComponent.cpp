//
// Created by Egor Orachyov on 05.04.2019.
//

#include "Components/IPrimitiveComponent.h"

namespace Berserk::Engine
{

    IPrimitiveComponent::IPrimitiveComponent(const IObjectInitializer &objectInitializer)
            : SceneComponent(objectInitializer),
              mPrimitiveType(ePT_NOT_PRIMITIVE),
              mRenderQueueType(eRQT_GEOMETRY),
              mCastShadows(FIELD_ON),
              mCastFarShadows(FIELD_OFF),
              mCanApplyCulling(FIELD_ON),
              mDrawWireFrame(FIELD_OFF),
              mDrawWireFrameOnly(FIELD_OFF),
              mDrawBoundingBox(FIELD_OFF)
    {

    }

} // namespace Berserk::EntitySystem