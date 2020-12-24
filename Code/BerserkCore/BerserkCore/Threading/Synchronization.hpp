/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SYNCHRONIZATION_HPP
#define BERSERK_SYNCHRONIZATION_HPP

#include <ttas_spin_mutex.hpp>
#include <mutex>

namespace Berserk {

    class SpinMutex {
    public:
        void Lock() {
            mMutex.lock();
        }

        void Unlock() {
            mMutex.unlock();
        }

    private:
        yamc::spin_ttas::mutex mMutex;
    };

    class Mutex {
    public:
        void Lock() {
            mMutex.lock();
        }

        void Unlock() {
            mMutex.unlock();
        }

    private:
        std::mutex mMutex;
    };

    class RecursiveMutex {
    public:
        void Lock() {
            mMutex.lock();
        }

        void Unlock() {
            mMutex.unlock();
        }

    private:
        std::recursive_mutex mMutex;
    };

    template<typename MutexType>
    class Guard {
    public:

        explicit Guard(MutexType& mutex)
            : mMutex(mutex) {
            mMutex.Lock();
        }

        ~Guard() {
            mMutex.Unlock();
        }

        Guard(const Guard&) = delete;
        Guard& operator=(const Guard&) = delete;

    private:
        MutexType& mMutex;
    };

}

#endif //BERSERK_SYNCHRONIZATION_HPP