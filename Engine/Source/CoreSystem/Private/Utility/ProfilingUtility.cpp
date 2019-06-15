//
// Created by Egor Orachyov on 27.02.2019.
//

#include "Utility/ProfilingUtility.h"

namespace Berserk
{

    char* ProfilingUtility::print(uint32 bytes, char *buffer)
    {
        if (bytes / Buffers::KiB == 0)
        {
            sprintf(buffer, "%5u Byte", bytes);
            return buffer;
        }
        else if (bytes / Buffers::MiB == 0)
        {
            sprintf(buffer, "%5.2f KiBs", (float32)bytes / (float32)Buffers::KiB);
            return buffer;
        }
        else if (bytes / Buffers::GiB == 0)
        {
            sprintf(buffer, "%5.2f MiBs", (float32)bytes / (float32)Buffers::MiB);
            return buffer;
        }
        else
        {
            sprintf(buffer, "%5.2f GiBs", (float32)bytes / (float32)Buffers::GiB);
            return buffer;
        }
    }

    char* ProfilingUtility::print(uint32 bytes, uint32 align, char *buffer)
    {
        if (bytes / Buffers::KiB == 0)
        {
            sprintf(buffer, "%*u Byte", align, bytes);
            return buffer;
        }
        else if (bytes / Buffers::MiB == 0)
        {
            sprintf(buffer, "%*.2f KiBs", align, (float32)bytes / (float32)Buffers::KiB);
            return buffer;
        }
        else if (bytes / Buffers::GiB == 0)
        {
            sprintf(buffer, "%*.2f MiBs", align, (float32)bytes / (float32)Buffers::MiB);
            return buffer;
        }
        else
        {
            sprintf(buffer, "%*.2f GiBs", align, (float32)bytes / (float32)Buffers::GiB);
            return buffer;
        }
    }

} // namespace Berserk