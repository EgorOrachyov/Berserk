/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Errors.h>
#include <Platform/Memory.h>
#include <stdio.h>

namespace Berserk {

    /** Store errors data in private linked list */
    struct ErrorData {
        static const uint32 BUFFER_SIZE = 2048;
        char message[BUFFER_SIZE] = {0};
        ErrorData* prev = nullptr;
        ErrorType errorType = ErrorType::Error;
    };

    struct ErrorDataList {
        ~ErrorDataList() {
            auto current = lastError;
            while (current != nullptr) {
                auto next = current->prev;
                Memory::free(current);
                current = next;
            }
        }

        ErrorData* lastError = nullptr;
    };

    static ErrorDataList errorDataList;

    void Errors::addError(ErrorType type, const char *message, uint64 line, const char *function, const char *file) {
        auto errorData = (ErrorData*) Memory::allocate(sizeof(ErrorData));

        snprintf(errorData->message, ErrorData::BUFFER_SIZE, "LINE: %llu; FUNCTION: %s; FILE: %s\n%s", line, function, file, message);

        errorData->errorType = type;
        errorData->prev = errorDataList.lastError;
        errorDataList.lastError = errorData;
    }

    const char* Errors::getErrorType(Berserk::ErrorType type) {
        switch (type) {
            case ErrorType::Error:
                return "Error";
            case ErrorType::Warning:
                return "Warning";
            default:
                assert(false);
        }
    }

    void Errors::forEachError(const Berserk::Function<void(const char *, Berserk::ErrorType)> &function) {
        auto last = errorDataList.lastError;

        while (last != nullptr) {
            function(last->message, last->errorType);
            last = last->prev;
        }
    }

}