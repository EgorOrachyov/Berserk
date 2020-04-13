/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ATOMIC_H
#define BERSERK_ATOMIC_H

#include <atomic>

namespace Berserk {

    /** STD atomic bool type */
    using AtomicBool = std::atomic_bool;

    /** STD atomic int 32 bit type */
    using AtomicInt32 = std::atomic_int32_t;

    /** STD atomic int 64 bit type */
    using AtomicInt64 = std::atomic_int64_t;

}

#endif //BERSERK_ATOMIC_H