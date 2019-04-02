//
// Created by Egor Orachyov on 02.04.2019.
//

#ifndef BERSERK_ENTITYSYSTEMTEST_H
#define BERSERK_ENTITYSYSTEMTEST_H

#include <Foundation/IObject.h>
#include <Foundation/IEntity.h>
#include <Memory/LinearAllocator.h>

void TraverseEntity(Berserk::EntitySystem::IEntity *entity, Berserk::uint32 offset = 0)
{
    using namespace Berserk;
    using namespace Berserk::EntitySystem;

    if (entity == nullptr) return;

    printf("%*s %s\n", offset, "", entity->getName());

    ArrayList<IEntity*> &list = entity->getEntitiesList();

    for (uint32 i = 0; i < list.getSize(); i++)
    {
        TraverseEntity(list[i], offset + 2);
    }
}

void BasicClassesTest()
{
    using namespace Berserk;
    using namespace Berserk::EntitySystem;

    printf("Type name: %s \n", IObject::getType());
    printf("Type name: %s \n", IEntity::getType());
    printf("Type name: %s \n", IEntityComponent::getType());

    LinearAllocator allocator(Buffers::MiB);

    auto mRoot    = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Root"));
    auto mEntity1 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity1"));
    auto mEntity2 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity2"));
    auto mEntity3 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity3"));
    auto mEntity4 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity4"));

    mRoot->attachEntity(mEntity1);
    mRoot->attachEntity(mEntity2);

    mEntity2->attachEntity(mEntity3);
    mEntity2->attachEntity(mEntity4);

    TraverseEntity(mRoot);

    delete (mEntity4);
    delete (mEntity3);
    delete (mEntity2);
    delete (mEntity1);
    delete (mRoot);

}

#endif //BERSERK_ENTITYSYSTEMTEST_H
