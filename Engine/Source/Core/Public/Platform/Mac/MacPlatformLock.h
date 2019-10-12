//
// Created by Egor Orachyov on 2019-10-12.
//

#ifndef BERSERK_MACPLATFORMLOCK_H
#define BERSERK_MACPLATFORMLOCK_H

#include <mutex>
#include <pthread.h>

namespace Berserk {

    /** MacOS mutex */
    class MacPlatformLock {
    public:
        inline void lock() {
            mMutex.lock();
        }
        inline void unlock() {
            mMutex.unlock();
        }
    private:
        std::mutex mMutex;
    };

    /** MacOS read/write posix based mutex */
    class MacPlatformReadWriteLock {
    public:
        inline MacPlatformReadWriteLock() {
            pthread_rwlock_init(&mLock, nullptr);
        }
        inline ~MacPlatformReadWriteLock() {
            pthread_rwlock_destroy(&mLock);
        }
        inline void lock_read() {
            pthread_rwlock_rdlock(&mLock);
        }
        inline void unlock_read() {
            pthread_rwlock_unlock(&mLock);
        }
        inline void lock_write() {
            pthread_rwlock_wrlock(&mLock);
        }
        inline void unlock_write() {
            pthread_rwlock_unlock(&mLock);
        }
    private:
        pthread_rwlock_t mLock;
    };

    /** Guard for basic lock */
    class MacPlatformGuard {
    public:
        inline MacPlatformGuard(MacPlatformLock& lock) : mLock(lock) {
            mLock.lock();
        }
        inline ~MacPlatformGuard() {
            mLock.unlock();
        }
    private:
        MacPlatformLock& mLock;
    };

    /** Read guard */
    class MacPlatformReadGuard {
    public:
        inline MacPlatformReadGuard(MacPlatformReadWriteLock& lock) : mLock(lock) {
                mLock.lock_read();
        }
        inline ~MacPlatformReadGuard() {
            mLock.unlock_read();
        }
    private:
        MacPlatformReadWriteLock& mLock;
    };

    /** Write guard */
    class MacPlatformWriteGuard {
    public:
        inline MacPlatformWriteGuard(MacPlatformReadWriteLock& lock) : mLock(lock) {
            mLock.lock_write();
        }
        inline ~MacPlatformWriteGuard() {
            mLock.unlock_write();
        }
    private:
        MacPlatformReadWriteLock& mLock;
    };

    /** Export platform functionality */
    typedef MacPlatformLock PlatformLock;
    typedef MacPlatformReadWriteLock PlatformReadWriteLock;
    typedef MacPlatformGuard PlatformGuard;
    typedef MacPlatformReadGuard PlatformReadGuard;
    typedef MacPlatformWriteGuard PlatformWriteGuard;

}

#endif //BERSERK_MACPLATFORMLOCK_H
