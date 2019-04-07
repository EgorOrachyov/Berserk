//
// Created by Egor Orachyov on 02.04.2019.
//

#include "GameFramework/IEntity.h"

namespace Berserk::EngineSystem
{

    IEntity::IEntity(const IObjectInitializer &objectInitializer)
            : IObject(objectInitializer),
              mAttachedEntities(DEFAULT_ATTACHED_ENTITIES_COUNT, objectInitializer.getAllocator()),
              mAttachedComponents(DEFAULT_ATTACHED_COMPONENTS_COUNT, objectInitializer.getAllocator())
    {

    }

    IEntity::~IEntity()
    {
        mAttachedEntities.reset();
        mAttachedComponents.reset();
    }

    void IEntity::attachEntity(IEntity *child)
    {
        FAIL(child, "Null pointer child entity");

        if (!child->mIsAttachable)
        {
            return;
        }

        if (child->mOwnerEntity == nullptr)
        {
            mAttachedEntities += child;
            child->mOwnerEntity = this;
        }
        else
        {
            WARNING("An attempt to attach an entity with owner [entity: '%s'][child: '%s']",
                    getName(), child->getName());
        }
    }

    void IEntity::attachComponent(IEntityComponent *component)
    {
        FAIL(component, "Null pointer component");

        if (!component->mIsAttachable)
        {
            return;
        }

        if (component->mOwnerEntity == nullptr)
        {
            mAttachedComponents += component;
            component->mOwnerEntity = this;
        }
        else
        {
            WARNING("An attempt to attach a component with owner [entity: '%s'][component: '%s']",
                    getName(), component->getName());
        }
    }

    void IEntity::setGroupEntity(IEntity *group)
    {
        FAIL(group, "Null pointer group entity");
        mGroupEntity = group;
    }

    void IEntity::setRootComponent(SceneComponent *root)
    {
        FAIL(root, "Null pointer root component");
        mRootComponent = root;
    }

} // namespace Berserk::EntitySystem