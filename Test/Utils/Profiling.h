//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_PROFILING_H
#define BERSERK_PROFILING_H

#include <Misc/Include.h>
#include <Misc/Buffers.h>
#include <Memory/IAllocator.h>
#include <Profiling/ProfilingUtility.h>

using namespace Berserk;

class Profiling
{

public:

    static void allocatorInfo(IAllocator &allocator)
    {
        using namespace Berserk;

        char usage[Buffers::SIZE_64];
        char total[Buffers::SIZE_64];

        printf("Free calls: %u | Alloc calls: %u | Total mem: %s \n",
               allocator.getFreeCalls(),
               allocator.getAllocateCalls(),
               ProfilingUtility::print((uint32)allocator.getTotalMemoryUsage(), 0, total));
    }

};


#endif //BERSERK_PROFILING_H
