/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_REFLECTIONFLAGS_H
#define BERSERK_REFLECTIONFLAGS_H

#include <Typedefs.h>

namespace Berserk {

    enum class EAccessMode {
        Public,
        Private
    };

    enum class EAttributeOption {
        Readable = 0,
        Writeable = 1,
        UIVisible = 2,
        EditorVisible = 3
    };

}

#endif //BERSERK_REFLECTIONFLAGS_H
