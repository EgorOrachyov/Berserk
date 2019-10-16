/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

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
