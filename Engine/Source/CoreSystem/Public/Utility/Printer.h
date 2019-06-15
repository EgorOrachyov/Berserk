//
// Created by Egor Orachyov on 15.06.2019.
//

#ifndef BERSERK_PRINTER_H
#define BERSERK_PRINTER_H

#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Misc/UsageDescriptors.h>
#include <Exception/Exception.h>

namespace Berserk
{

    /**
     * Printer utility class, wraps printf functionality for
     * formatted char and wide char string output
     */
    class CORE_API Printer
    {
    public:

        template <typename ... TArgs>
        static char* print(char* buffer, const char* format, TArgs& ... args)
        {
            sprintf(buffer, format, args ...);
            return buffer;
        }

        template <typename ... TArgs>
        static char* print(char* buffer, uint32 bufferSize, const char* format, TArgs& ... args)
        {
            snprintf(buffer, bufferSize, format, args ...);
            return buffer;
        }

    };

} // namespace Berserk

#endif //BERSERK_PRINTER_H