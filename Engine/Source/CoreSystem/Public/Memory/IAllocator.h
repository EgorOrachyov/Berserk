//
// Created by Egor Orachyov on 19.03.2019.
//

#ifndef BERSERK_IALLOCATOR_H
#define BERSERK_IALLOCATOR_H

#include "Misc/Types.h"

namespace Berserk
{

    class IAllocator
    {
    public:

        /** Allocates chosen size of continuous memory block */
        virtual void* allocate(uint32 size) = 0;

        /** Free memory block */
        virtual void free(void *pointer) = 0;

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        virtual uint32 getFreeCalls() const = 0;

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        virtual uint32 getAllocateCalls() const = 0;

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        virtual uint64 getTotalMemoryUsage() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_IALLOCATOR_H