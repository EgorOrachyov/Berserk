/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ERRORLIST_H
#define BERSERK_ERRORLIST_H

#include <Typedefs.h>

namespace Berserk {

    enum class Error : uint32 {
        OK,
        FAILURE,
        FAILED_READ,
        FAILED_WRITE,
        FAILED_OPEN,
        FAILED_CLOSE,
    };

}

#endif //BERSERK_ERRORLIST_H
