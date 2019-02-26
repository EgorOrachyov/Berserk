//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_FILEUTILITY_H
#define BERSERK_FILEUTILITY_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Utils for work with platform file system
     */
    class CORE_API FileUtility
    {
    public:

        /** @return True if file exist */
        static bool exist(const char* filename);

        /** Reads file content in the specified buffer */
        static void read(const char *filename, void *buffer);

        /** Reads file content in the specified buffer with size limitation */
        static void read(const char *filename, void *buffer, uint32 size);

    };

} // namespace Berserk

#endif //BERSERK_FILEUTILITY_H