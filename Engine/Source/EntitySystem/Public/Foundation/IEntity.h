//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_IENTITY_H
#define BERSERK_IENTITY_H

#include <Foundation/IObject.h>
#include <Containers/ArrayList.h>
#include <Containers/SharedList.h>
#include <Components/SceneComponet.h>
#include <Components/IInputComponent.h>

namespace Berserk::EntitySystem
{

    /**
     * Entity is the base class for an Object that can be placed or spawned in a level.
     * Entities may contain a collection of IEntityComponents, which can be used to control
     * how entities move, how they are rendered, etc.
     *
     * Also allows to group entities, attach it to multi-layers, in order to get
     * access to a set of some objects at once.
     */
    class ENGINE_EXPORT IEntity : public IObject
    {
    public:

        explicit IEntity(const IObjectInitializer& initializer) : IObject(initializer) {}

        ~IEntity() override = default;

        /** Show, that entity has no life time limit */
        static const int32 LIFE_TIME_FOREVER = -1;

    public:

        /** Call after component registration */
        virtual void onRegister()                   {}

        /** Called before component unregister action */
        virtual void onUnregister()                 {}

        /** Called each frame to pre-update before multi-threaded pass (should be fast) */
        virtual void onUpdate()                     {}

        /** Called each frame after multi-threaded pass */
        virtual void onPostUpdate()                 {}

    public:

        typedef CallableSignature ActionOnInput;
        ActionOnInput OnInput = nullptr;

    public:

        /** @return True, if object was register method called (ECS) */
        bool isRegistered()                                 { return mIsRegistered; }

        /** @return True, if object was unregister method called (ECS) */
        bool isUnregistered()                               { return mIsUnregistered; }

        /** @return True, if object can tick each frame (ECS) */
        bool canTick()                                      { return mCanTick; }

        /** @return Life time of this object (ECS) */
        float32 getLifeTime()                               { return mLifeTime; }

        /** @return Time stamp when object was created (ECS) */
        float32 getCreationTime()                           { return mCreationTime; }

        /** @return Last update time stamp (ECS) */
        float32 getLastUpdateTime()                         { return mLastUpdateTime; }

        /** @return Custom scale factor for delta time (game-play) */
        float32 getCustomTimeDilation()                     { return mCustomTimeDilation; }

        /** @return Group of this entity (ECS) */
        IEntity* getGroupEntity()                           { return mGroupEntity; }

        /** @return List of layers, which this entity belongs (ECS) */
        SharedList<IEntity*> &getLayersList()               { return mAttachedLayers; }

        /** @return List of attached to this entity components (ECS) */
        ArrayList<IEntityComponent*> &getComponentsList()   { return mAttachedComponents; }

    private:

        float32 mCreationTime       = 0;                        //! Time of creation of this object
        float32 mLastUpdateTime     = 0;                        //! Last time, when object was updated
        float32 mCustomTimeDilation = 1.0f;                     //! Scale factor for delta time of entity (allows to create custom update time)
        float32 mLifeTime           = LIFE_TIME_FOREVER;        //! How long the entity will live

        SceneComponet*      mSceneComponent = nullptr;          //!
        IInputComponent*    mInputComponent = nullptr;          //!

        IEntity*                     mGroupEntity = nullptr;    //! Group of this entity
        SharedList<IEntity*>         mAttachedLayers;           //! List of layers to which this object is attached
        ArrayList<IEntityComponent*> mAttachedComponents;       //! Components, attached to this entity

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IENTITY_H