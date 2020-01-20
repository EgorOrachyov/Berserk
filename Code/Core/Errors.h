/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ERRORS_H
#define BERSERK_ERRORS_H

#include <Typedefs.h>
#include <cstdlib>

namespace Berserk {

    enum class EErrorType {
        Warning,
        Error
    };

    class Errors {
    public:

        /** Add error into list of registered engine errors */
        static void addError(EErrorType type, const char *message, uint64 line, const char *function, const char *file);
        static const char* getErrorType(EErrorType type);
        static void forEachError(const Function<void(const char *message, uint64 line, const char* function, const char* file, EErrorType errorType)> &function);
    };
}

#define BERSERK_ERROR(message)                                                                  \
    do {                                                                                        \
        Errors::addError(EErrorType::Error, (message), __LINE__, __FUNCTION__, __FILE__);       \
    } while (0);

#define BERSERK_ERROR_RET(message)                                                              \
    do {                                                                                        \
        Errors::addError(EErrorType::Error, (message), __LINE__, __FUNCTION__, __FILE__);       \
        return;                                                                                 \
    } while (0);

#define BERSERK_ERROR_RET_VALUE(messgae, ret)                                                   \
    do {                                                                                        \
        Errors::addError(EErrorType::Error, (message), __LINE__, __FUNCTION__, __FILE__);       \
        return ret;                                                                             \
    } while (0);

#define BERSERK_ERROR_FAIL(message)                                                             \
    do {                                                                                        \
        Errors::addError(EErrorType::Warning, (message), __LINE__, __FUNCTION__, __FILE__);     \
        std::abort();                                                                           \
    } while (0);

#define BERSERK_WARNING(message)                                                                \
    do {                                                                                        \
        Errors::addError(EErrorType::Warning, (message), __LINE__, __FUNCTION__, __FILE__);     \
    } while (0);

#endif //BERSERK_ERRORS_H