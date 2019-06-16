//
// Created by Egor Orachyov on 15.06.2019.
//

#ifndef BERSERK_PRINTER_H
#define BERSERK_PRINTER_H

#include <Misc/Types.h>
#include <Misc/Include.h>
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

        template <typename ... TArgs>
        static int32 print(char* buffer, const char* format, const TArgs& ... args)
        {
            return sprintf(buffer, format, args ...);
        }

        template <typename ... TArgs>
        static int32 print(char* buffer, uint32 bufferSize, const char* format, const TArgs& ... args)
        {
            return snprintf(buffer, bufferSize, format, args ...);
        }

    };

} // namespace Berserk

#endif //BERSERK_PRINTER_H