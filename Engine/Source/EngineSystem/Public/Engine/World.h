//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_WORLD_H
#define BERSERK_WORLD_H

#include <Foundation/IObject.h>

namespace Berserk::EntitySystem
{

    /**
     * World is a top level object representing map or an area of game world, where
     * all the entities are located. Your game optionally could have any number of worlds.
     *
     * Each world supports seamless world chunks streaming, what provides dynamic and real-time
     * pre-loading of areas via scripts and loading volumes.
     *
     * Typical single player game can contain only one world with utilization of world
     * chunks streaming for dynamic loading in case where the game has only one huge area of action.
     */
    class ENGINE_API World final : public IObject
    {
    public:

        GENERATE_CLASS_BODY(World);

        explicit World(const IObjectInitializer& objectInitializer);

        ~World();

    private:

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_WORLD_H