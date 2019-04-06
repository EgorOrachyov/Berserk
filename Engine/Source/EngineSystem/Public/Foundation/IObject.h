//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_IOBJECT_H
#define BERSERK_IOBJECT_H

#include <Misc/Crc32.h>
#include <Misc/Types.h>
#include <Strings/String.h>
#include <Memory/MemorySizer.h>
#include <Misc/UsageDescriptors.h>
#include <Foundation/IObjectMacros.h>
#include <Foundation/IObjectUpdater.h>
#include <Foundation/IObjectInitializer.h>

namespace Berserk::EngineSystem
{

    /**
     * The base class for all engine game-play foundation system objects
     * and components. Root class for all objects in the Entity Component System module.
     * Its interface should not be changed.
     */
    class ENGINE_API IObject
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_OBJECT_BODY(IObject);

        /** Default object setup via initializer */
        explicit IObject(const IObjectInitializer& objectInitializer);

        /** Primary destructor */
        virtual ~IObject() = default;

    public:

        /** Called after object creation */
        virtual void onInitialize()     {};

        /** Called before object disposing */
        virtual void onDestroy()        {};

        /** Called to serialize object */
        virtual void serialize()        {};

        /** Called to handle any received message [later] */
        virtual void handleMessage()    {};

        /** Used to show that bool filed is active */
        static const uint8 FIELD_ON  = 1;

        /** Used to show that bool filed is disabled */
        static const uint8 FIELD_OFF = 0;

        /** Signature for all custom user-defined functions */
        typedef void (*CallableSignature)(const IObjectUpdater& updater);

    public:

        /** @return True, if object was properly initialized (ECS) */
        bool isInitialized()        { return mIsInitialized; }

        /** @return True, if object was properly destroyed (ECS) */
        bool isDestroyed()          { return mIsDestroyed; }

        /** @return True, if object should be setup after multi-threaded execution (ECS) */
        bool isDirty()              { return mIsDirty; }

        /** @return True, if can process this object in multi-threaded mode (ECS) */
        bool isMultiThreaded()      { return mIsMultiThreaded; }

        /** Allows to rename object */
        virtual void rename(const char* name);

        /** Allows to profile memory usage by this object */
        virtual void getMemoryUsage(MemorySizer *memorySizer);

        /** @return This hashed object by default crc32 algorithm */
        virtual uint32 hash();

        /** @return One-line string info about object */
        virtual const char* toString() { return ""; }

        /** @return Poiner to char* object name */
        virtual const char* getName()  { return mObjectName.get(); }

    protected:

        uint8 mIsInitialized        : 1;
        uint8 mIsDestroyed          : 1;
        uint8 mIsRegistered         : 1;
        uint8 mIsUnregistered       : 1;
        uint8 mIsActive             : 1;
        uint8 mIsPaused             : 1;
        uint8 mIsEditable           : 1;
        uint8 mIsAttachable         : 1;
        uint8 mIsToggleable         : 1;
        uint8 mIsDamageable         : 1;
        uint8 mIsDirty              : 1;
        uint8 mIsMovable            : 1;
        uint8 mIsMultiThreaded      : 1;
        uint8 mIsModyfied           : 1;
        uint8 mHasSceneComponent    : 1;
        uint8 mHasRenderComponent   : 1;
        uint8 mHasAudioComponent    : 1;
        uint8 mHasPhysicsComponent  : 1;
        uint8 mHasAIComponent       : 1;
        uint8 mCanTick              : 1;

        /** Object world name */
        CString mObjectName;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IOBJECT_H