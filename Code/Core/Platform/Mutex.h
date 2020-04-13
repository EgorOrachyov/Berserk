/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MUTEX_H
#define BERSERK_MUTEX_H

#include <mutex>

namespace Berserk {

    /** STD mutex for any platform */
    using Mutex = std::mutex;

    /** Rename guard for better usage */
    using Guard = std::lock_guard<std::mutex>;

}

#endif //BERSERK_MUTEX_H