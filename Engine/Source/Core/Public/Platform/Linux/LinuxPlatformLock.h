//
// Created by Egor Orachyov on 2019-10-13.
//

#ifndef BERSERK_LINUXPLATFORMLOCK_H
#define BERSERK_LINUXPLATFORMLOCK_H

#include <mutex>
#include <pthread.h>

namespace Berserk {

    /** Linux mutex */
    class LinuxPlatformLock {
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

    /** Linux read/write posix based mutex */
    class LinuxPlatformReadWriteLock {
    public:
        inline LinuxPlatformReadWriteLock() {
            pthread_rwlock_init(&mLock, nullptr);
        }
        inline ~LinuxPlatformReadWriteLock() {
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
    class LinuxPlatformGuard {
    public:
        inline LinuxPlatformGuard(LinuxPlatformLock& lock) : mLock(lock) {
            mLock.lock();
        }
        inline ~LinuxPlatformGuard() {
            mLock.unlock();
        }
    private:
        LinuxPlatformLock& mLock;
    };

    /** Read guard */
    class LinuxPlatformReadGuard {
    public:
        inline LinuxPlatformReadGuard(LinuxPlatformReadWriteLock& lock) : mLock(lock) {
            mLock.lock_read();
        }
        inline ~LinuxPlatformReadGuard() {
            mLock.unlock_read();
        }
    private:
        LinuxPlatformReadWriteLock& mLock;
    };

    /** Write guard */
    class LinuxPlatformWriteGuard {
    public:
        inline LinuxPlatformWriteGuard(LinuxPlatformReadWriteLock& lock) : mLock(lock) {
            mLock.lock_write();
        }
        inline ~LinuxPlatformWriteGuard() {
            mLock.unlock_write();
        }
    private:
        LinuxPlatformReadWriteLock& mLock;
    };

    /** Export platform functionality */
    typedef LinuxPlatformLock PlatformLock;
    typedef LinuxPlatformReadWriteLock PlatformReadWriteLock;
    typedef LinuxPlatformGuard PlatformGuard;
    typedef LinuxPlatformReadGuard PlatformReadGuard;
    typedef LinuxPlatformWriteGuard PlatformWriteGuard;

}

#endif //BERSERK_LINUXPLATFORMLOCK_H
