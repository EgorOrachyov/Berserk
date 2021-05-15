/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
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

        explicit Guard(MutexType &mutex)
                : mMutex(mutex) {
            mMutex.Lock();
        }

        ~Guard() {
            mMutex.Unlock();
        }

        Guard(const Guard &) = delete;

        Guard &operator=(const Guard &) = delete;

    private:
        MutexType &mMutex;
    };

}

#endif //BERSERK_SYNCHRONIZATION_HPP