//
// Created by Egor Orachyov on 2019-08-27.
//

#ifndef BERSERK_EXCEPTIONMACROS_H
#define BERSERK_EXCEPTIONMACROS_H

#include <IO/Printer.h>
#include <Logging/Debug.h>
#include <Misc/Exception.h>

namespace Berserk
{

    class ExceptionUtility final
    {
    public:

        template <typename ... TArgs>
        static void triggerf(uint64 line, const char* function, const char* file,
                            const char* format, const TArgs& ... args)
        {
            const uint32 messageSize = Buffers::KiB;
            char message[messageSize];
            Printer::print(message, messageSize, format, args ...);

            const uint32 resultSize = Buffers::KiB * 2;
            char result[resultSize];
            Printer::print(result, resultSize, "(EXCEPTION: (LINE %lu: FUNCTION %s: FILE %s) %s", line, function, file, message);
            Debug::get().addMessage(result, ELogVerbosity::Error, true);

            throw Exception(result);
        }

    };

} // namespace Berserk

#ifndef engine_exception
/** Engine utility macro used to throw an exception from an arbitrary file */
#define engine_exception(MSG, ...) \
        { ExceptionUtility::triggerf((uint64)(__LINE__), __FUNCTION__, __FILE__, MSG, ##__VA_ARGS__); }
#endif

#endif //BERSERK_EXCEPTIONMACROS_H
