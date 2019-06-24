//
// Created by Egor Orachyov on 10.06.2019.
//

#ifndef BERSERK_ASSERTDEV_H
#define BERSERK_ASSERTDEV_H

#include <Exception/Exception.h>
#include <Utility/Printer.h>

namespace Berserk
{
    /**
     * Assert utility for dev purposes without mirroring to
     * log about assert cause
     *
     * @note Thread-Safe, no internal allocations
     */
    class AssertDev
    {
    public:

        template <typename ... TArgs>
        static void assertf(const char* condition, uint64 line, const char* function, const char* file,
                           const char* format, TArgs& ... args)
        {
            const uint32 messageSize = Buffers::KiB;
            char message[messageSize];
            Printer::print(message, messageSize, format, args ...);

            const uint32 resultSize = Buffers::KiB * 2;
            char result[resultSize];
            Printer::print(result, resultSize, "(CONDITION: '%s' LINE %lu: FUNCTION %s: FILE %s) %s", line, function, file, message);

            throw Exception(result);
        }

        static void assertm(const char* condition, uint64 line, const char* function, const char* file)
        {
            const uint32 resultSize = Buffers::KiB * 2;
            char result[resultSize];
            Printer::print(result, resultSize, "(CONDITION: '%s' LINE %lu: FUNCTION %s: FILE %s)", line, function, file);

            throw Exception(result);
        }

    };


#ifndef assert_dev
#define assert_dev(condition)                                                       \
        if (condition) { }                                                          \
        else { AssertDev::assertm(#condition, (uint64)(__LINE__), __FUNCTION__, __FILE__); }
#endif

} // namespace Berserk

#endif //BERSERK_ASSERTDEV_H
