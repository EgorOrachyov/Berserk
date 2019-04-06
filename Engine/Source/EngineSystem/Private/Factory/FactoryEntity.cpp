//
// Created by Egor Orachyov on 03.04.2019.
//

#include "Factory/FactoryEntity.h"

namespace Berserk::EngineSystem
{

    IObject* FactoryEntity::CreateObject(const IFactoryInitializer &initializer)
    {
        auto ObjectBuilder = initializer.getObjectInitializer();
        auto ObjectAllocator = ObjectBuilder->getObjectAllocator();
        auto newObject = new(ObjectAllocator->allocate(sizeof(IEntity))) IEntity(*ObjectBuilder);

        return newObject;
    }

    IFactory* FactoryEntity::getSingleton()
    {
        static FactoryEntity factoryEntity;
        return &factoryEntity;
    }

} // namespace Berserk::EntitySystem