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

namespace Berserk::Engine
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

        /**
         * Primary destructor
         * @warning Remember to create virual destructors for all kinds of
         *          inheritors of superclass IObject, otherwise the memory used
         *          by object won't be freed
         */
        virtual ~IObject();

        /**
         * Allows to create object of chosen type from object initializer class
         * @warning Engine API internal usage only
         * @tparam T Type of the object to be created (should be child of IObject class)
         * @param objectInitializer Contains info, needed to create object
         * @return Pointer to allocated and initialized instance of class T
         */
        template <class T>
        static T* createObject(const IObjectInitializer& objectInitializer)
        {
            uint32 size = sizeof(T);
            IAllocator* allocator = objectInitializer.getAllocator();

            return new(allocator->allocate(size)) T(objectInitializer);
        }

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

        /** Initialize method was called */
        uint8 mIsInitialized : 1;

        /** Destroy method was called */
        uint8 mIsDestroyed : 1;

        /** Register method was called */
        uint8 mIsRegistered : 1;

        /** Unregister method was called */
        uint8 mIsUnregistered : 1;

        /** If engine systems will process this object */
        uint8 mIsActive : 1;

        /** Visible but pause movement updates */
        uint8 mIsPaused : 1;

        /** Can edit properties */
        uint8 mIsEditable : 1;

        /** Can attach this one to other objects */
        uint8 mIsAttachable : 1;

        /** Can use toggle mechanics */
        uint8 mIsToggleable : 1;

        /** Can be damaged by other objects */
        uint8 mIsDamageable : 1;

        /** Contains data modified in multi-thread mode execution */
        uint8 mIsDirty : 1;

        /** Can move on scene */
        uint8 mIsMovable : 1;

        /** Can process on other thread[s] */
        uint8 mIsMultiThreaded : 1;

        /** Was modified after last update */
        uint8 mIsModified : 1;

        /** If has attached scene component */
        uint8 mHasSceneComponent : 1;

        /** If has attached primitive component */
        uint8 mHasPrimitiveComponent : 1;

        /** If has attached audio component */
        uint8 mHasAudioComponent : 1;

        /** If has attached physics component */
        uint8 mHasPhysicsComponent : 1;

        /** If has attached AI component */
        uint8 mHasAIComponent : 1;

        /** If can tick - call tick update function */
        uint8 mCanTick : 1;

        /** Global object name */
        CString mObjectName;

        /** Allocator used to allocate memory for this object components */
        IAllocator* mGeneralAllocator = nullptr;


    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IOBJECT_H