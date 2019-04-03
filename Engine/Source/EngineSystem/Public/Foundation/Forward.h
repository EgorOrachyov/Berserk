//
// Created by Egor Orachyov on 01.04.2019.
//

#ifndef BERSERK_FORWARD_H
#define BERSERK_FORWARD_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk::EntitySystem
{

    /** Forward declaration for base classes */

    class IObject;
    class IObjectUpdater;
    class IObjectInitializer;
    class IEntity;
    class IEntityComponent;
    class ISystem;
    class IFactory;
    class IFactoryInitializer;
    class World;
    class WorldChunk;
    class WorldManager;
    class WorldChunkManager;

} // namespace Berserk::EntitySystem

#endif //BERSERK_FORWARD_H
