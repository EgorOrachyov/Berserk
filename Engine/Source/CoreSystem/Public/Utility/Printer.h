//
// Created by Egor Orachyov on 15.06.2019.
//

#ifndef BERSERK_PRINTER_H
#define BERSERK_PRINTER_H

#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Misc/Buffers.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * Printer utility class, wraps printf functionality for
     * formatted char and wide char string output
     */
    class CORE_API Printer
    {
    public:

        /**
         * Wrapper for printf function
         * @tparam TArgs Types of args
         * @param buffer Actual storage for printed message
         * @param format String which specifies where to insert args
         * @param args Actual arguments to insert i formatted string
         * @return Count of chars successfully written
         */
        template <typename ... TArgs>
        static int32 print(char* buffer, const char* format, const TArgs& ... args)
        {
            return sprintf(buffer, format, args ...);
        }

        /**
         * Wrapper for printf function
         * @tparam TArgs Types of args
         * @param buffer Actual storage for printed message
         * @param bufferSize Size of buffer to write result
         * @param format String which specifies where to insert args
         * @param args Actual arguments to insert i formatted string
         * @return Count of chars successfully written
         */
        template <typename ... TArgs>
        static int32 print(char* buffer, uint32 bufferSize, const char* format, const TArgs& ... args)
        {
            return snprintf(buffer, bufferSize, format, args ...);
        }

        /** Pretty bytes (memory) printing (with KiB, MiB, GiB, ...) */
        static char* print(uint32 bytes, char* buffer)
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

        /**
         * Pretty bytes (memory) printing (with KiB, MiB, GiB, ...)
         * with alignment (max length) for printed number
         */
        static char* print(uint32 bytes, uint32 align, char* buffer)
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

    };

} // namespace Berserk

#endif //BERSERK_PRINTER_H