//
// Created by Egor Orachyov on 30.05.2018.
//

#ifndef BERSERKENGINE_SYSTEMSIDANDFLAGS_H
#define BERSERKENGINE_SYSTEMSIDANDFLAGS_H

namespace Berserk
{
    // todo: more facilities for memory profiling

    /**
     * Systems' types available for memory profiling
     */
    enum SystemType
    {
        ST_CORE = 0,
        ST_ALLOCATION = 1,
        ST_MEMORY = 2,
        ST_DEBUG = 3,
        ST_MANAGMENT = 4,
        ST_CONTAINERS = 5
    };
    
    /**
     * Managers' types available for memory profiling
     */
    enum ManagerType
    {
        MT_TEXTURE = 0,
        MT_GPU_PROGRAM = 1,
        MT_AUDIO = 3,
        MT_MATERIAL = 4,
        MT_MESH = 5,
        MT_SCRIPT = 6
    }

} // namespace Berserk

#endif //BERSERKENGINE_SYSTEMSIDANDFLAGS_H
