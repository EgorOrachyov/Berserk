/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Errors.h>
#include <Platform/System.h>

namespace Berserk {

    /** Store errors data in private linked list */
    struct ErrorData {
        static const uint32 MESSAGE_SIZE = 2048;
        static const uint32 FUNCTION_SIZE = 64;
        static const uint32 FILE_SIZE = 256;

        char message[MESSAGE_SIZE] = {0};
        char function[FUNCTION_SIZE] = {0};
        char file[FILE_SIZE] = {0};

        uint64 line = 0;

        ErrorData* prev = nullptr;
        EErrorType errorType = EErrorType::Error;
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

    void Errors::addError(EErrorType type, const char *message, uint64 line, const char *function, const char *file) {
        System::getSingleton().getErrorSyncMutex()->lock();

        using string = TStringUtility<char,'\0'>;
        auto errorData = objectNew<ErrorData>();

        string::copy(errorData->message, ErrorData::MESSAGE_SIZE, message);
        string::copy(errorData->file, ErrorData::FILE_SIZE, file);
        string::copy(errorData->function, ErrorData::FUNCTION_SIZE, function);

        errorData->line = line;
        errorData->errorType = type;
        errorData->prev = errorDataList.lastError;
        errorDataList.lastError = errorData;

        System::getSingleton().getErrorSyncMutex()->unlock();
    }

    const char* Errors::getErrorType(Berserk::EErrorType type) {
        switch (type) {
            case EErrorType::Error:
                return "Error";
            case EErrorType::Warning:
                return "Warning";
            default:
                assert(false);
        }
    }

    void Errors::forEachError(
            const Berserk::Function<void(const char *, unsigned long long int, const char *, const char *,
                                         Berserk::EErrorType)> &function) {
        auto last = errorDataList.lastError;

        while (last != nullptr) {
            function(last->message, last->line, last->function, last->file, last->errorType);
            last = last->prev;
        }
    }


}