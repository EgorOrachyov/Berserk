//
// Created by Egor Orachyov on 02.04.2019.
//

#include "Foundation/IEntity.h"

namespace Berserk::EntitySystem
{

    IEntity::IEntity(const IObjectInitializer &initializer)
            : IObject(initializer),
              mAttachedEntities(DEFAULT_ATTACHED_ENTITIES_COUNT, initializer.getAllocator()),
              mAttachedComponents(DEFAULT_ATTACHED_COMPONENTS_COUNT, initializer.getAllocator())
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

    void IEntity::setRootComponent(SceneComponet *root)
    {
        FAIL(root, "Null pointer root component");
        mRootComponent = root;
    }

} // namespace Berserk::EntitySystem