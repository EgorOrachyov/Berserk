//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ASSERT_H
#define BERSERK_ASSERT_H

#include <Misc/Exception.h>
#include <IO/Printer.h>
#include <Logging/Debug.h>

namespace Berserk
{

    /**
     * Assert utility which throws an exception and writes
     * error message into the debug engine log
     *
     * @note Thread-Safe, no internal allocations
     */
    class Assert
    {
    public:

        template <typename ... TArgs>
        static void assertf(const char* condition, uint64 line, const char* function, const char* file,
                            const char* format, const TArgs& ... args)
        {
            const uint32 messageSize = Buffers::KiB;
            char message[messageSize];
            Printer::print(message, messageSize, format, args ...);

            const uint32 resultSize = Buffers::KiB * 2;
            char result[resultSize];
            Printer::print(result, resultSize, "(CONDITION: '%s' LINE %lu: FUNCTION %s: FILE %s) %s", condition, line, function, file, message);
            Debug::get().addMessage(result, ELogVerbosity::Error, true);

            throw Exception(result);
        }

        static void assertm(const char* condition, uint64 line, const char* function, const char* file)
        {
            const uint32 resultSize = Buffers::KiB * 2;
            char result[resultSize];
            Printer::print(result, resultSize, "(CONDITION: '%s' LINE %lu: FUNCTION %s: FILE %s)", condition, line, function, file);
            Debug::get().addMessage(result, ELogVerbosity::Error, true);

            throw Exception(result);
        }

    };

#ifndef assertion
/**
 * Check the condition and if it is false print the error message
 * with mapped args in that like a mask and throws an exception
 */
#define assertion(condition)                                                                                            \
        if (condition) { }                                                                                              \
        else { Assert::assertm(#condition, (uint64)(__LINE__), __FUNCTION__, __FILE__); }
#endif

#ifndef assertion_msg
/**
 * Check the condition and if it is false print the error message
 * with mapped args in that like a mask and throws an exception
 */
#define assertion_msg(condition, MSG, ...)                                                                              \
        if (condition) { }                                                                                              \
        else { Assert::assertf(#condition, (uint64)(__LINE__), __FUNCTION__, __FILE__, MSG, ##__VA_ARGS__); }
#endif

}

#endif //BERSERK_ASSERT_H
