/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ATOMIC_HPP
#define BERSERK_ATOMIC_HPP

#include <atomic>

namespace Berserk {
    using AtomicBool = std::atomic_bool;
    using AtomicInt32 = std::atomic_int32_t;
    using AtomicInt64 = std::atomic_int64_t;
}

#endif //BERSERK_ATOMIC_HPP