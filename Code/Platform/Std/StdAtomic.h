/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STDATOMIC_H
#define BERSERK_STDATOMIC_H

#include <Platform/IAtomic.h>
#include <atomic>

namespace Berserk {

    class StdAtomic : public IAtomic {
    public:
        StdAtomic() : mAtomicInt(0) {}
        ~StdAtomic() override = default;

        int32 load() override {
            return mAtomicInt.load();
        }
        void store(int32 value) override {
            mAtomicInt.store(value);
        }
        int32 add(int32 value) override {
            return mAtomicInt.fetch_add(value);
        }
        int32 sub(int32 value) override {
            return mAtomicInt.fetch_sub(value);
        }
        bool cas(int32 expected, int32 desired) override {
            return mAtomicInt.compare_exchange_strong(expected, desired);
        }
    private:
        std::atomic_int mAtomicInt;
    };

}

#endif //BERSERK_STDATOMIC_H