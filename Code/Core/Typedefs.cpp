/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Typedefs.h>

namespace Berserk {
    namespace {
        static_assert(sizeof(wchar) == 4,   "BerserkCore: incompatible size of type");
        static_assert(sizeof(float) == 4,   "BerserkCore: incompatible size of type");
        static_assert(sizeof(double) == 8,  "BerserkCore: incompatible size of type");
    }
}