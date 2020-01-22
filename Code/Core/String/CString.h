/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CSTRING_H
#define BERSERK_CSTRING_H

#include <String/TString.h>

namespace Berserk {
    /** Char string of dynamic size with small buffer optimization (capacity of 23 symbols + end)*/
    using CString = TString<char,'\0', 24>;
}

#endif //BERSERK_CSTRING_H