//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_ERROR_H
#define BERSERK_ERROR_H

#include <Misc/StringUtils.h>
#include <Misc/PrinterUtils.h>
#include <Misc/MathUtils.h>
#include <exception>

namespace Berserk {

    /** Basic throwable object for any type of runtime error */
    class Error : public std::exception {
    public:

        /** Error exception form string message (size is limited) */
        explicit Error(const char* message) {
            uint32 l = StringUtilsChar::length(message);
            uint32 size = MathUtils::min(BUFFER_SIZE, l);
            StringUtilsChar::copy((char*) mMessageBuffer, size, message);
        }

        /** @return Error message (readonly) */
        const char* getMessage() const {
            return (char*) mMessageBuffer;
        }

    private:

        static const uint32 BUFFER_SIZE = Memory::KiB;
        uint8 mMessageBuffer[BUFFER_SIZE] = { '\0' };

    };

    /** Error throw mechanism without log mirror (only for core basic development)*/
    class ErrorThrow {
    public:

        template <typename ... TArgs>
        static void emmit(uint64 line, const char* function, const char* file, const char* format, TArgs&& ... args) {
            const uint32 messageSize = Memory::KiB;
            char message[messageSize];
            PrinterUtils::print(message, messageSize, format, std::forward<TArgs>(args) ...);

            const uint32 resultSize = Memory::KiB * 2;
            char result[resultSize];
            PrinterUtils::print(result, resultSize, "(LINE %lu: FUNCTION %s: FILE %s) %s", line, function, file, message);

            throw Error(result);
        }

    };

}

#ifndef dev_error
#define dev_error(format, ...) {    \
    ErrorThrow::emmit((uint64)(__LINE__), __FUNCTION__, __FILE__, format, ##__VA_ARGS__); \
}
#endif

#endif //BERSERK_ERROR_H