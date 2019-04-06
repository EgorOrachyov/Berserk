//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_IENTITY_H
#define BERSERK_IENTITY_H

#include <Foundation/IObject.h>
#include <Containers/ArrayList.h>
#include <Containers/SharedList.h>
#include <Components/SceneComponent.h>
#include <Components/IInputComponent.h>

namespace Berserk::EngineSystem
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

        GENERATE_CLASS_BODY(IEntity);

        /** Default object setup via initializer */
        explicit IEntity(const IObjectInitializer& objectInitializer);

        /** Primary destructor */
        ~IEntity() override;

        /** Show, that entity has no life time limit */
        static const int32 LIFE_TIME_FOREVER = -1;

        /** Default number of child entities to preallocate */
        static const uint32 DEFAULT_ATTACHED_ENTITIES_COUNT = 4;

        /** Default number of components to preallocate */
        static const uint32 DEFAULT_ATTACHED_COMPONENTS_COUNT = 4;

    public:

        /** Call after component registration */
        virtual void onRegister()   {};

        /** Called before component unregister action */
        virtual void onUnregister() {};

        /** Called each frame to pre-update before multi-threaded pass (should be fast) */
        virtual void onUpdate()     {};

        /** Called each frame after multi-threaded pass */
        virtual void onPostUpdate() {};

        /** Add new child for this entity */
        void attachEntity(IEntity* child);

        /** Attach new component to this entity */
        void attachComponent(IEntityComponent* component);

        /** Allows to define group entity, which belongs this entity */
        void setGroupEntity(IEntity* group);

        /** Set root transformation for this entity */
        void setRootComponent(SceneComponent* root);

    public:

        /** @return True, if object was register method called (ECS) */
        bool isRegistered()         { return mIsRegistered; }

        /** @return True, if object was unregister method called (ECS) */
        bool isUnregistered()       { return mIsUnregistered; }

        /** @return True, if system updates this object at all (ECS) */
        bool isActive()             { return mIsActive; }

        /** @return True, if object updated, but its progress is paused (game-play) */
        bool isPaused()             { return mIsPaused; }

        /** @return True, if can edit configuration of this object (ECS) */
        bool isEditable()           { return mIsEditable; }

        /** @return True, if can attach this object to others (ECS) */
        bool isAttachable()         { return mIsAttachable; }

        /** @return True, if can use toggle mechanics (game-play) */
        bool isToggleable()         { return mIsToggleable; }

        /** @return True, if can get damage (game-play) */
        bool isDamageable()         { return mIsDamageable; }

        /** @return True, if can move this object on the scene (ECS) */
        bool isMovable()            { return mIsMovable; }

        /** @return True, if object has scene component (transformation) (ECS) */
        bool hasSceneComponent()    { return mHasSceneComponent; }

        /** @return True, if object has rendering component (ECS) */
        bool hasRenderComponet()    { return mHasRenderComponent; }

        /** @return True, if object has audio component (ECS) */
        bool hasAudioComponet()     { return mHasAudioComponent; }

        /** @return True, if object has physics component (ECS) */
        bool hasPhysicsComponet()   { return mHasPhysicsComponent; }

        /** @return True, if object has AI component (ECS) */
        bool hasAIComponet()        { return mHasAIComponent; }

        /** @return True, if object can tick each frame (ECS) */
        bool canTick()              { return mCanTick; }

    public:

        /** @return Life time of this object (ECS) */
        float32 getLifeTime()                               { return mLifeTime; }

        /** @return Time stamp when object was created (ECS) */
        float32 getCreationTime()                           { return mCreationTime; }

        /** @return Last update time stamp (ECS) */
        float32 getLastUpdateTime()                         { return mLastUpdateTime; }

        /** @return Custom scale factor for delta time (game-play) */
        float32 getCustomTimeDilation()                     { return mCustomTimeDilation; }

        /** @return Owner entity of this entity (ECS) */
        IEntity* getOwnerEntity()                           { return mOwnerEntity; }

        /** @return Group of this entity (ECS) */
        IEntity* getGroupEntity()                           { return mGroupEntity; }

        /** @return List of entities, which owner this entity (ECS) */
        ArrayList<IEntity*> &getEntitiesList()              { return mAttachedEntities; }

        /** @return List of attached to this entity components (ECS) */
        ArrayList<IEntityComponent*> &getComponentsList()   { return mAttachedComponents; }

    private:

        /** Time of creation of this object */
        float32 mCreationTime = 0;

        /** Last time, when object was updated */
        float32 mLastUpdateTime = 0;

        /** Scale factor for delta time of entity (allows to create custom update time) */
        float32 mCustomTimeDilation = 1.0f;

        /** How long the entity will live */
        float32 mLifeTime = LIFE_TIME_FOREVER;

        /** Root transformation for this entity, which defines its position, rotation, scale */
        class SceneComponent* mRootComponent = nullptr;

        /** Owner of this entity (Entities hierarchy tree local root) */
        class IEntity* mOwnerEntity = nullptr;

        /** Group of this entity */
        class IEntity* mGroupEntity = nullptr;

        /** List of layers to which this object is attached */
        ArrayList<IEntity*> mAttachedEntities;

        /** Components, attached to this entity */
        ArrayList<IEntityComponent*> mAttachedComponents;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IENTITY_H