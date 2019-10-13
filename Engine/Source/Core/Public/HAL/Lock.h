//
// Created by Egor Orachyov on 2019-10-12.
//

#ifndef BERSERK_LOCK_H
#define BERSERK_LOCK_H

namespace Berserk {

    /** Allows to control access to resource from multiple threads */
    typedef PlatformLock Lock;

    /** Multiple-readers single-writer lock */
    typedef PlatformReadWriteLock ReadWriteLock;

    /** Lock safe access */
    typedef PlatformGuard Guard;

    /** Read lock safe access */
    typedef PlatformReadGuard ReadGuard;

    /** Write lock safe access */
    typedef PlatformWriteGuard WriteGuard;

}

#endif //BERSERK_LOCK_H