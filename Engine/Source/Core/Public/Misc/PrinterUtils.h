//
// Created by Egor Orachyov on 15.06.2019.
//

#ifndef BERSERK_PRINTERUTILS_H
#define BERSERK_PRINTERUTILS_H

#include <HAL/Platform.h>
#include <HAL/Memory.h>
#include <stdio.h>

namespace Berserk
{

    /**
     * Printer utility class, wraps printf functionality for
     * formatted char and wide char string output
     */
    class PrinterUtils
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
        static int32 print(char* buffer, const char* format, const TArgs&& ... args)
        {
            return sprintf(buffer, format, std::forward<TArgs>(args) ...);
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
        static int32 print(char* buffer, uint32 bufferSize, const char* format, TArgs&& ... args)
        {
            return snprintf(buffer, bufferSize, format, std::forward<TArgs>(args) ...);
        }

        /** Pretty bytes (memory) printing (with KiB, MiB, GiB, ...) */
        static char* print(uint32 bytes, char* buffer)
        {
            if (bytes / Memory::KiB == 0)
            {
                sprintf(buffer, "%5u Byte", bytes);
                return buffer;
            }
            else if (bytes / Memory::MiB == 0)
            {
                sprintf(buffer, "%5.2f KiBs", (float32)bytes / (float32)Memory::KiB);
                return buffer;
            }
            else if (bytes / Memory::GiB == 0)
            {
                sprintf(buffer, "%5.2f MiBs", (float32)bytes / (float32)Memory::MiB);
                return buffer;
            }
            else
            {
                sprintf(buffer, "%5.2f GiBs", (float32)bytes / (float32)Memory::GiB);
                return buffer;
            }
        }

        /**
         * Pretty bytes (memory) printing (with KiB, MiB, GiB, ...)
         * with alignment (max length) for printed number
         */
        static char* print(uint32 bytes, uint32 align, char* buffer)
        {
            if (bytes / Memory::KiB == 0)
            {
                sprintf(buffer, "%*u Byte", align, bytes);
                return buffer;
            }
            else if (bytes / Memory::MiB == 0)
            {
                sprintf(buffer, "%*.2f KiBs", align, (float32)bytes / (float32)Memory::KiB);
                return buffer;
            }
            else if (bytes / Memory::GiB == 0)
            {
                sprintf(buffer, "%*.2f MiBs", align, (float32)bytes / (float32)Memory::MiB);
                return buffer;
            }
            else
            {
                sprintf(buffer, "%*.2f GiBs", align, (float32)bytes / (float32)Memory::GiB);
                return buffer;
            }
        }

    };

} // namespace Berserk

#endif //BERSERK_PRINTERUTILS_H