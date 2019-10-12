//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOCK_H
#define BERSERK_MUTEX_H

#include <mutex>

namespace Berserk
{
    typedef std::mutex Mutex;
    typedef std::lock_guard<std::mutex> CriticalSection;
}

#endif //BERSERK_LCOK_H
