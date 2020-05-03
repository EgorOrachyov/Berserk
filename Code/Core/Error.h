/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ERROR_H
#define BERSERK_ERROR_H

#include <Typedefs.h>

namespace Berserk {

    enum class EError : uint32 {
        OK,
        FAILURE,
        FAILED_READ,
        FAILED_WRITE,
        FAILED_OPEN,
        FAILED_CLOSE,
        UNAVAILABLE,
        NO_SUCH_RESOURCE,
        FAILED_IMPORT_RESOURCE,
        INVALID_ARGS_COUNT,
        FAILED_TO_CALL_METHOD
    };

}

#endif //BERSERK_ERROR_H
