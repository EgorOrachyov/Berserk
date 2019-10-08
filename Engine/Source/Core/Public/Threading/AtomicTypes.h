//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_ATOMICTYPES_H
#define BERSERK_ATOMICTYPES_H

#include <atomic>

namespace Berserk
{

    typedef std::atomic_bool AtomicBool;
    typedef std::atomic_int AtomicInt;
    typedef std::atomic_long AtomicLong;

} // namespace Berserk

#endif //BERSERK_ATOMICTYPES_H
