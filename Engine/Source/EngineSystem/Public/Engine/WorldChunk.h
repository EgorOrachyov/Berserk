//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_WORLDCHUNK_H
#define BERSERK_WORLDCHUNK_H

#include <Foundation/IObject.h>

namespace Berserk::EngineSystem
{

    /**
     * World chunk represent any kind of subarea for a world object.
     * Provides dynamic steaming of world data in real-time mode.
     *
     * In its core has own chunk area allocator, which provides memory management for
     * chunk entities and their components. Allows AT ONCE load and unload level data.
     */
    class ENGINE_API WorldChunk final : public IObject
    {
    public:

        GENERATE_CLASS_BODY(WorldChunk);

        /** Default object setup via initializer */
        WorldChunk(const IObjectInitializer& objectInitializer);

        ~WorldChunk();

    private:

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_WORLDCHUNK_H