/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CSTRINGSTATIC_H
#define BERSERK_CSTRINGSTATIC_H

#include <String/TStringStaticN.h>

namespace Berserk {
    /** Static string with 64 bytes buffer size (max 63 chars + null) */
    using CStringStatic = TStringStaticN<char,'\0',64>;
}

#endif //BERSERK_CSTRINGSTATIC_H
