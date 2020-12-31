/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUXSTRINGTABLE_HPP
#define BERSERK_LINUXSTRINGTABLE_HPP

#include <BerserkCore/Strings/StringName.hpp>

namespace Berserk {

    class LinuxStringTable: public StringNameTable  {
    public:
        using LinuxImpl = Impl;
    };

}

#endif //BERSERK_LINUXSTRINGTABLE_HPP