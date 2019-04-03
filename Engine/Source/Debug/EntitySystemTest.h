//
// Created by Egor Orachyov on 02.04.2019.
//

#ifndef BERSERK_ENTITYSYSTEMTEST_H
#define BERSERK_ENTITYSYSTEMTEST_H

#include <Foundation/Entity.h>
#include <Factory/FactoryEntity.h>
#include <Memory/LinearAllocator.h>

void TraverseEntity(Berserk::EntitySystem::IEntity *entity, Berserk::uint32 offset = 0)
{
    using namespace Berserk;
    using namespace Berserk::EntitySystem;

    if (entity == nullptr) return;

    auto ownerName = (entity->getOwnerEntity() ? entity->getOwnerEntity()->getName() : nullptr);
    printf("%*s %s -> %s \n", offset, "", entity->getName(), ownerName);

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

    auto mRoot    = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Root"   , &allocator));
    auto mEntity1 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity1", &allocator));
    auto mEntity2 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity2", &allocator));
    auto mEntity3 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity3", &allocator));
    auto mEntity4 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity4", &allocator));
    auto mEntity5 = new(allocator.allocate(sizeof(IEntity))) IEntity(IObjectInitializer("Entity5", &allocator));

    mRoot->attachEntity(mEntity1);
    mRoot->attachEntity(mEntity2);
    mEntity2->attachEntity(mEntity3);
    mEntity2->attachEntity(mEntity4);
    mEntity4->attachEntity(mEntity5);

    TraverseEntity(mRoot);

    printf("Free calls: %u Alloc calls: %u Total mem: %lu \n",
           allocator.getFreeCalls(), allocator.getAllocateCalls(), allocator.getTotalMemoryUsage());

    delete (mEntity5);
    delete (mEntity4);
    delete (mEntity3);
    delete (mEntity2);
    delete (mEntity1);
    delete (mRoot);

    printf("Free calls: %u Alloc calls: %u Total mem: %lu \n",
           allocator.getFreeCalls(), allocator.getAllocateCalls(), allocator.getTotalMemoryUsage());
}

void FactoryCreationTest()
{
    using namespace Berserk;
    using namespace Berserk::EntitySystem;

    printf("Type name: %s size %lu \n", IObject::getType(), sizeof(IObject));
    printf("Type name: %s size %lu \n", IEntity::getType(), sizeof(IEntity));
    printf("Type name: %s size %lu \n", IEntityComponent::getType(), sizeof(IEntityComponent));

    LinearAllocator allocator(Buffers::MiB);

    auto entityFactory = FactoryEntity::getSingleton();
    auto objectInitializer = IObjectInitializer("root", &allocator, &allocator);
    auto factoryInitializer = IFactoryInitializer(&objectInitializer);

    auto root = (Entity*) entityFactory->CreateObject(factoryInitializer);

    printf("Free calls: %u Alloc calls: %u Usage: %u Total mem: %lu \n",
           allocator.getFreeCalls(), allocator.getAllocateCalls(), allocator.getUsage(), allocator.getTotalMemoryUsage());

    TraverseEntity(root);

    delete(root);
}

#endif //BERSERK_ENTITYSYSTEMTEST_H
