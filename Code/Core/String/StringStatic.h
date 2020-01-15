/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STRINGSTATIC_H
#define BERSERK_STRINGSTATIC_H

#include <String/StringUtility.h>

namespace Berserk {

    template <typename Char, Char end, uint32 BUFFER_CAPACITY = 64>
    class StringStatic {
    public:

        using Strings = StringUtility<Char, end>;

        StringStatic() = default;

    private:

        Char mBuffer[BUFFER_CAPACITY] = {'\0'};

    };

}

#endif //BERSERK_STRINGSTATIC_H