/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ERRORMACRO_H
#define BERSERK_ERRORMACRO_H

#include <Typedefs.h>
#include <cstdlib>

namespace Berserk {

    enum class EErrorType {
        Warning,
        Error
    };

    class ErrorMacro {
    public:
        /** Release all errors before shut-down to avoid memory leaks */
        static void releaseAllErrors();
        /** Add error into list of registered engine errors */
        static void addError(EErrorType type, const char *message, uint64 line, const char *function, const char *file);
        static const char* getErrorType(EErrorType type);
        static void forEachError(const Function<void(const char *message, uint64 line, const char* function, const char* file, EErrorType errorType)> &function);
    };
}

#define BERSERK_ERROR(...)                                                                          \
    do {                                                                                            \
        char _message_[2000];                                                                       \
        snprintf(_message_, 2000, __VA_ARGS__);                                                     \
        ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);       \
    } while (0);

#define BERSERK_ERROR_RET(...)                                                                      \
    do {                                                                                            \
        char _message_[2000];                                                                       \
        snprintf(_message_, 2000, __VA_ARGS__);                                                     \
        ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);       \
        return;                                                                                     \
    } while (0);

#define BERSERK_ERROR_RET_VALUE(ret, ...)                                                           \
    do {                                                                                            \
        char _message_[2000];                                                                       \
        snprintf(_message_, 2000, __VA_ARGS__);                                                     \
        ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);       \
        return ret;                                                                                 \
    } while (0);

#define BERSERK_ERROR_FAIL(...)                                                                     \
    do {                                                                                            \
        char _message_[2000];                                                                       \
        snprintf(_message_, 2000, __VA_ARGS__);                                                     \
        ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);     \
        std::abort();                                                                               \
    } while (0);

#define BERSERK_COND_ERROR(condition, ...)                                                          \
    do {                                                                                            \
        if (condition) { } else {                                                                   \
            char _message_[2000];                                                                   \
            snprintf(_message_, 2000, __VA_ARGS__);                                                 \
            ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);   \
        }                                                                                           \
    } while (0);

#define BERSERK_COND_ERROR_RET(condition, ...)                                                      \
    do {                                                                                            \
        if (condition) { } else {                                                                   \
            char _message_[2000];                                                                   \
            snprintf(_message_, 2000, __VA_ARGS__);                                                 \
            ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);   \
            return;                                                                                 \
        }                                                                                           \
    } while (0);

#define BERSERK_COND_ERROR_RET_VALUE(ret, condition, ...)                                           \
    do {                                                                                            \
        if (condition) { } else {                                                                   \
            char _message_[2000];                                                                   \
            snprintf(_message_, 2000, __VA_ARGS__);                                                 \
            ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);   \
            return ret;                                                                             \
        }                                                                                           \
    } while (0);

#define BERSERK_COND_ERROR_FAIL(condition, ...)                                                     \
    do {                                                                                            \
        if (condition) { } else {                                                                   \
            char _message_[2000];                                                                   \
            snprintf(_message_, 2000, __VA_ARGS__);                                                 \
            ErrorMacro::addError(EErrorType::Error, _message_, __LINE__, __FUNCTION__, __FILE__);   \
            std::abort();                                                                           \
        }                                                                                           \
    } while (0);

#define BERSERK_WARNING(...)                                                                        \
    do {                                                                                            \
        char _message_[2000];                                                                       \
        snprintf(_message_, 2000, __VA_ARGS__);                                                     \
        ErrorMacro::addError(EErrorType::Warning, _message_, __LINE__, __FUNCTION__, __FILE__);     \
    } while (0);

#endif //BERSERK_ERRORMACRO_H