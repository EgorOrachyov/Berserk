//
// Created by Egor Orachyov on 01.04.2019.
//

#include "Foundation/IObject.h"

namespace Berserk::EngineSystem
{

    IObject::IObject(const IObjectInitializer& objectInitializer)
            : mObjectName(objectInitializer.getName()),
              mObjectAllocator(objectInitializer.getObjectAllocator()),
              mGeneralAllocator(objectInitializer.getAllocator())
    {
        mIsInitialized       = FIELD_OFF;
        mIsDestroyed         = FIELD_OFF;
        mIsRegistered        = FIELD_OFF;
        mIsUnregistered      = FIELD_OFF;
        mIsActive            = FIELD_ON;
        mIsPaused            = FIELD_OFF;
        mIsEditable          = FIELD_ON;
        mIsAttachable        = FIELD_ON;
        mIsToggleable        = FIELD_ON;
        mIsDamageable        = FIELD_OFF;
        mIsDirty             = FIELD_OFF;
        mIsMovable           = FIELD_ON;
        mIsModyfied          = FIELD_OFF;
        mIsMultiThreaded     = FIELD_OFF;
        mHasSceneComponent   = FIELD_OFF;
        mHasRenderComponent  = FIELD_OFF;
        mHasAudioComponent   = FIELD_OFF;
        mHasPhysicsComponent = FIELD_OFF;
        mHasAIComponent      = FIELD_OFF;
        mCanTick             = FIELD_OFF;
    }

    IObject::~IObject()
    {
        /** It is the last action in the procedure of objects destructing */
        mObjectName.~DynamicString();
        mObjectName.nullify();
        mObjectAllocator->free(this);
    }

    void IObject::rename(const char *name)
    {
        mObjectName = name;
    }

    void IObject::getMemoryUsage(MemorySizer *memorySizer)
    {
        memorySizer->addObject(mObjectName.get(), sizeof(IObject));
    }

    uint32 IObject::hash()
    {
        auto source = (char*)this;
        return Crc32::hash(source, sizeof(IObject));
    }

} // namespace Berserk::EntitySystem