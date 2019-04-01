//
// Created by Egor Orachyov on 01.04.2019.
//

#include "Objects/IObject.h"

namespace Berserk::EntitySystem
{

    IObject::IObject() : mIsActive(1),
                         mIsAttachable(1),
                         mIsDamageable(0),
                         mIsDirty(0),
                         mIsEditable(1),
                         mIsMovable(1),
                         mIsMultiThreaded(0),
                         mIsPaused(0),
                         mIsToggleable(1),
                         mHasAIComponent(0),
                         mHasAudioComponent(0),
                         mHasPhysicsComponent(0),
                         mHasRenderComponent(0),
                         mHasTransformation(0)

    {
        /** Do nothing */
    }

    void IObject::rename(const char *name)
    {
        mObjectName = name;
    }

    void IObject::getMemoryUsage(MemorySizer *sizer)
    {
        sizer->addObject(mObjectName.get(), sizeof(IObject));
    }

    uint32 IObject::hash()
    {
        auto source = (char*)this;
        return Crc32::hash(source, sizeof(IObject));
    }

} // namespace Berserk::EntitySystem