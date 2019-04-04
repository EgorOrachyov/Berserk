//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_IENTITYCOMPONENT_H
#define BERSERK_IENTITYCOMPONENT_H

#include <Foundation/IObject.h>

namespace Berserk::EntitySystem
{

    /**
     * Base class for any IEntity object. Only instances of classes derived from
     * IEntityComponent could be attached to the entity. Components describes any
     * common behaviour and processing params, which could be shared among entities.
     */
    class ENGINE_API IEntityComponent : public IObject
    {
    public:

        GENERATE_CLASS_BODY(IEntityComponent);

        /** Default object setup via initializer */
        explicit IEntityComponent(const IObjectInitializer& objectInitializer) : IObject(objectInitializer) {}

        ~IEntityComponent() override = default;

        /** Call after component registration */
        virtual void onRegister() {}

        /** Called before component unregister action */
        virtual void onUnregister() {}

        /** Called each frame to pre-update before multi-threaded pass (should be fast) */
        virtual void onUpdate() {}

        /** Called each frame after multi-threaded pass */
        virtual void onPostUpdate() {}

    public:

        /** Allows to register component in its type of system */
        virtual void registerComponent() {};

        /** Allows to unregister component in its type of system */
        virtual void unregisterComponent() {};

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

        /** @return True, if object can tick each frame (ECS) */
        bool canTick()              { return mCanTick; }

        /** @return Owner entity or null */
        IEntity* getOwner()         { return mOwnerEntity; }

    protected:

        friend class IEntity;

        /** Pointer to component owner (updates automatically) */
        class IEntity* mOwnerEntity = nullptr;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IENTITYCOMPONENT_H