/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXSTRINGTABLE_HPP
#define BERSERK_UNIXSTRINGTABLE_HPP

#include <BerserkCore/Strings/StringName.hpp>

namespace Berserk {

    class UnixStringTable: public StringNameTable  {
    public:
        using UnixImpl = Impl;
    };

}

#endif //BERSERK_UNIXSTRINGTABLE_HPP