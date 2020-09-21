/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORY_HPP
#define BERSERK_MEMORY_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    /** Memory utility (wraps standard C operations) */
    class Memory {
    public:
        static void copy(void* destination, const void* source, uint64 size) { std::memcpy(destination, source, size); }
        static void set(void* source, uint32 value, uint64 size) { std::memset(source, value, size); }
        static uint64 alignAs(uint64 size, uint64 alignment) { return size + (size % alignment ? alignment - (size % alignment): 0); }
    };

}

#endif //BERSERK_MEMORY_HPP