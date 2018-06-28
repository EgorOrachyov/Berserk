//
// Created by Egor Orachyov on 25.05.2018.
//

#include "../Profiling/MemoryProfiler.h"
#include "../Logging/LogManager.h"

namespace Berserk
{
    MemoryProfiler::MemoryProfiler()
    {
        /* Set up profiled systems*/

        mSystems[DEBUG_CORE].SetSystemName("Core");
        mSystems[DEBUG_CORE].SetLimitation(DEBUG_NO_MEM_LIMITATION);

        mSystems[DEBUG_DEBUG].SetSystemName("Debug");
        mSystems[DEBUG_DEBUG].SetLimitation(DEBUG_NO_MEM_LIMITATION);

        mSystems[DEBUG_ALLOCATORS].SetSystemName("Memory Allocators");
        mSystems[DEBUG_ALLOCATORS].SetLimitation(DEBUG_NO_MEM_LIMITATION);

        mSystems[DEBUG_POOLALLOCATORS].SetSystemName("Pool Allocators");
        mSystems[DEBUG_POOLALLOCATORS].SetLimitation(DEBUG_NO_MEM_LIMITATION);

        /* Add static info about singleton objects */

        AddAllocatedMemory(DEBUG_DEBUG, sizeof(MemoryProfiler));
        AddAllocatedMemory(DEBUG_DEBUG, sizeof(LogManager));

        SetLimitation(DEBUG_DEBUG, MEM_MiB);
    }

    void MemoryProfiler::AddAllocatedMemory(int16 system, int32 bytes)
    {
        ASSERT(system >= 0, "Wrong system identification (>=)");
        ASSERT(system < DEBUG_NUM_OF_SYSTEMS, "Wrong system identification (<)");

        mSystems[system].AddAllocatedMemory(bytes);
    }

    void MemoryProfiler::ReduceAllocatedMemory(int16 system, int32 bytes)
    {
        ASSERT(system >= 0, "Wrong system identification (>=)");
        ASSERT(system < DEBUG_NUM_OF_SYSTEMS, "Wrong system identification (<)");

        mSystems[system].ReduceAllocatedMemory(bytes);
    }

    void MemoryProfiler::SetLimitation(int16 system, int32 limitation)
    {
        ASSERT(system >= 0, "Wrong system identification (>=)");
        ASSERT(system < DEBUG_NUM_OF_SYSTEMS, "Wrong system identification (<)");

        mSystems[system].SetLimitation(limitation);
    }

    void MemoryProfiler::PushInfoIntoLog()
    { /*
        globalLogManager.PushMessage(LMI_INFO, "Memory Profiler (Data)");
        globalLogManager.PushMessage(LMI_INFO, "--------------------[System Name]-----[Alloc Memory]----[Available mem]-------[Limitation]------[Total Calls]");

        int32 totalAlloc = 0;
        int32 totalCalls = 0;
        char buffer[BUFFER_SIZE_128];

        for(int16 i = 0; i < DEBUG_NUM_OF_SYSTEMS; i++)
        {
            mSystems[i].PushInfo(buffer);
            totalAlloc += mSystems[i].GetAllocatedMemory();
            totalCalls += mSystems[i].GetMemoryCalls();
            globalLogManager.PushMessage(LMI_INFO, buffer);
        }

        char allocBuffer[BUFFER_SIZE_16];
        PushInfoAboutBytes(allocBuffer, totalAlloc);

        globalLogManager.PushMessage(LMI_INFO, "--------------------------[Total]-------[Full Alloc]----[Full free mem]-------[Full Limit]-------[Full Calls]");

        sprintf(buffer, "%32s: %16s | %16s   %16s   %16i |", NULL, allocBuffer, NULL, NULL, totalCalls);
        globalLogManager.PushMessage(LMI_INFO, buffer);

        globalLogManager.PushMessage(LMI_INFO, "-------------------------------------------------------------------------------------------------------------"); */
    }

    void MemoryProfiler::PushNoteToLog()
    { /*
        globalLogManager.PushMessage(LMI_INFO, "Memory Profiler (Note)");
        globalLogManager.PushMessage(LMI_INFO, "1) System name shows which type of classes and structures gives this data");
        globalLogManager.PushMessage(LMI_INFO, "2) Allocated memory is data which was given by mem_alloc (involves data from used)");
        globalLogManager.PushMessage(LMI_INFO, "3) Available memory (could be use without expansion)");
        globalLogManager.PushMessage(LMI_INFO, "4) Limitation shows the value of data which can be used by this system");
        globalLogManager.PushMessage(LMI_INFO, "5) Calls shows the number of attempts to get memory \n"); */
    }

    void MemoryProfiler::PushInfoAboutBytes(char * destination, int32 value)
    {
        if (value < 1024)
            sprintf(destination, "%4i bytes", value);
        else if (value / MEM_KiB < 1024)
            sprintf(destination, "%6.2f KiB", (float32)(value / MEM_KiB));
        else if (value / MEM_MiB < 1024)
            sprintf(destination, "%6.2f MiB", (float32)(value / MEM_MiB));
        else
            sprintf(destination, "%6.2f GiB", (float32)(value / MEM_GiB));
    }

    MemoryProfiler globalMemoryProfiler;

} // namespace Berserk
