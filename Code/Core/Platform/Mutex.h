/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MUTEX_H
#define BERSERK_MUTEX_H

namespace Berserk {

    /** Platform independent mutex class for threads synchronization */
    class Mutex {
    public:
        virtual ~Mutex() = default;
        virtual void lock() = 0;
        virtual void unlock() = 0;
    };

    class Guard {
    public:
        explicit Guard(Mutex& mutex) : mMutex(mutex) { mutex.lock(); }
        ~Guard() { mMutex.unlock(); }
    private:
        Mutex& mMutex;
    };

}

#endif //BERSERK_MUTEX_H