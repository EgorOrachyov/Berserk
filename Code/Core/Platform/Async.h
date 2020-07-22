/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ASYNC_H
#define BERSERK_ASYNC_H

#include <TPtrShared.h>
#include <Platform/Atomic.h>

namespace Berserk {

    /**
     * Sync feature for threaded operations
     */
    class Async {
    public:

        Async() = default;
        Async(const Async& other) = default;
        Async(Async&& other) = default;
        ~Async() = default;

        Async& operator=(const Async& other) = default;
        Async& operator=(Async&& other) = default;

        bool isPresent() const { return mSyncData.isNotNull(); }
        bool isLoaded() const;
        void setSyncData(TPtrShared<AtomicBool> data);

        /** Mark async as loaded (may affect others async with shared data) */
        void markAsLoaded();
        /** Reset async */
        void reset();
        /** Releases async sync data */
        void release();
        /** Creates sync data */
        void create();
        /** Wait until async is true */
        void blockUntilLoaded();

    private:
        TPtrShared<AtomicBool> mSyncData;
    };

}

#endif //BERSERK_ASYNC_H