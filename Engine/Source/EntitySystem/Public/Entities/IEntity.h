//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_IENTITY_H
#define BERSERK_IENTITY_H

#include <Objects/IObject.h>
#include <Containers/ArrayList.h>
#include <Containers/SharedList.h>

namespace Berserk::EntitySystem
{

    class ENGINE_EXPORT IEntity : public IObject
    {
    public:

        explicit IEntity(const IObjectInitializer& initializer) : IObject(initializer) {}

        ~IEntity() override = default;

        /** Call after component registration */
        virtual void onRegister()                   {}

        /** Called before component unregister action */
        virtual void onUnregister()                 {}

        /** Called each frame to pre-update before multi-threaded pass (should be fast) */
        virtual void onUpdate()                     {}

        /** Called each frame before multi-threaded pass as user defined action */
        virtual void onTick(float32 deltaSeconds)   {}

        /** Called each frame after multi-threaded pass */
        virtual void onPostUpdate()                 {}

        /** Called when there is some input for this entity */
        virtual void onInput()                      {}

    public:

        /** @return True, if object was register method called (ECS) */
        bool isRegistered()                                 { return mIsRegistered; }

        /** @return True, if object was unregister method called (ECS) */
        bool isUnregistered()                               { return mIsUnregistered; }

        /** @return True, if object can tick each frame (ECS) */
        bool canTick()                                      { return mCanTick; }

        /** @return Group of this entity */
        IEntity* getGroupEntity()                           { return mGroupEntity; }

        /** @return List of layers, which this entity belongs */
        SharedList<IEntity*> &getLayersList()               { return mAttachedLayers; }

        /** @return List of attached to this entity components */
        ArrayList<IEntityComponent*> &getComponentsList()   { return mAttachedComponents; }

    private:

        IEntity*                     mGroupEntity = nullptr;    //! Group of this entity
        SharedList<IEntity*>         mAttachedLayers;           //! List of layers to which this object is attached
        ArrayList<IEntityComponent*> mAttachedComponents;       //! Components, attached to this entity

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IENTITY_H