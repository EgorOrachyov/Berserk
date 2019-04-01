//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_IENTITYCOMPONENT_H
#define BERSERK_IENTITYCOMPONENT_H

#include <Objects/IObject.h>

namespace Berserk::EntitySystem
{

    /**
     * Base class for any IEntity object. Only instances of classes derived from
     * IEntityComponent could be attached to the entity. Components describes any
     * common behaviour and processing params, which could be shared among entities.
     */
    class ENGINE_EXPORT IEntityComponent : public IObject
    {
    public:

        ~IEntityComponent() override = default;

        /** Call after component registration */
        virtual void onRegister() {}

        /** Called before component unregister action */
        virtual void onUnregister() {}

        /** Called each frame to pre-update before multi-threaded pass (should be fast) */
        virtual void onUpdate() {}

        /** Called each frame before multi-threaded pass as user defined action */
        virtual void onTick(float32 deltaSeconds) {}

        /** Called each frame after multi-threaded pass */
        virtual void onPostUpdate() {}

    public:

        /** Allows to register component in its type of system */
        virtual void registerComponet () = 0;

        /** Allows to unregister component in its type of system */
        virtual void unregisterComponet () = 0;

        /** Allows to attach component to chosen entity */
        virtual void attach(IEntity* entity) = 0;

    public:

        /** @return True, if object was register method called (ECS) */
        bool isRegistered()         { return mIsRegistered; }

        /** @return True, if object was unregister method called (ECS) */
        bool isUnregistered()       { return mIsUnregistered; }

        /** @return True, if object can tick each frame (ECS) */
        bool canTick()              { return mCanTick; }

        /** @return Owner entity or null */
        IEntity* getOwner()         { return mOwnerEntity; }

    protected:

        /** Pointer to component owner (updates automatically) */
        IEntity* mOwnerEntity = nullptr;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IENTITYCOMPONENT_H