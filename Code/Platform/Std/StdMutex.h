/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STDMUTEX_H
#define BERSERK_STDMUTEX_H

#include <Platform/Mutex.h>
#include <mutex>

namespace Berserk {

    class StdMutex : public IMutex {
    public:
        ~StdMutex() override = default;
        void lock() override { mMutex.lock(); }
        void unlock() override { mMutex.unlock(); }
    private:
        std::mutex mMutex;
    };

}

#endif //BERSERK_STDMUTEX_H