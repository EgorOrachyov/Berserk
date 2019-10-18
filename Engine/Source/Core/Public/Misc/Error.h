/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

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
            uint32 l = StringUtilsChar::length(message) + 1;
            uint32 size = MathUtils::min(BUFFER_SIZE, l);
            StringUtilsChar::copy((char*) mMessageBuffer, size, message);
        }

        /** @return Cause of error text*/
        const char *what() const noexcept override {
            return getMessage();
        }

        /** @return Error message (readonly) */
        const char* getMessage() const noexcept {
            return (char*) mMessageBuffer;
        }

    private:

        static const uint32 BUFFER_SIZE = 2 * Memory::KiB;
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
            PrinterUtils::print(result, resultSize, "\n(LINE %lu: FUNCTION %s: FILE %s)\n%s", line, function, file, message);

            throw Error(result);
        }

        template <typename ... TArgs>
        static void emmit(const char* condition, uint64 line, const char* function, const char* file, const char* format, TArgs&& ... args) {
            const uint32 messageSize = Memory::KiB;
            char message[messageSize];
            PrinterUtils::print(message, messageSize, format, std::forward<TArgs>(args) ...);

            const uint32 resultSize = Memory::KiB * 2;
            char result[resultSize];
            PrinterUtils::print(result, resultSize, "\nCONDITION: %s (LINE %lu: FUNCTION %s: FILE %s)\n%s", condition, line, function, file, message);

            throw Error(result);
        }

    };

}

#ifndef RAW_ERROR
#define RAW_ERROR(format, ...) {    \
    ErrorThrow::emmit((uint64)(__LINE__), __FUNCTION__, __FILE__, format, ##__VA_ARGS__); \
}
#endif

#ifndef RAW_ERROR_CONDITION
#define RAW_ERROR_CONDITION(condition, format, ...) { \
    if (condition) { \
    } else { \
        ErrorThrow::emmit(#condition, (uint64)(__LINE__), __FUNCTION__, __FILE__, format, ##__VA_ARGS__); \
    } \
}
#endif

#endif //BERSERK_ERROR_H