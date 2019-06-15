//
// Created by Egor Orachyov on 27.02.2019.
//

#ifndef BERSERK_PROFILINGUTILITY_H
#define BERSERK_PROFILINGUTILITY_H

#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/Include.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{
    class CORE_API ProfilingUtility
    {
    public:

        /** Pretty bytes (memory) printing (with KiB, MiB, GiB, ...) */
        static char* print(uint32 bytes, char* buffer);

        /**
         * Pretty bytes (memory) printing (with KiB, MiB, GiB, ...)
         * with alignment (max length) for printed number
         */
        static char* print(uint32 bytes, uint32 align, char* buffer);

    };

} // namespace Berserk

#endif //BERSERK_PROFILINGUTILITY_H
