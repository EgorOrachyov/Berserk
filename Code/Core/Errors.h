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

namespace Berserk {

    enum class ErrorType {
        Warning,
        Error
    };

    class Errors {
    public:

        /** Add error into list of registered engine errors */
        static void addError(ErrorType type, const char *message, uint64 line, const char *function, const char *file);
        static const char* getErrorType(ErrorType type);
        static void forEachError(const Function<void(const char *message, ErrorType errorType)> &function);

    };

}

#define BERSERK_ERROR(message)                                                                  \
    do {                                                                                        \
        Errors::addError(ErrorType::Error, (message), __LINE__, __FUNCTION__, __FILE__);        \
    } while (0);

#define BERSERK_WARNING(message)                                                                \
    do {                                                                                        \
        Errors::addError(ErrorType::Warning, (message), __LINE__, __FUNCTION__, __FILE__);      \
    } while (0);

#endif //BERSERK_ERRORS_H