/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Threading/Async.h>
#include <Threading/Thread.h>
#include <ErrorMacro.h>

namespace Berserk {

    bool Async::isLoaded() const {
        BERSERK_COND_ERROR_RET_VALUE(false, isPresent(), "Sync data is not present")
        return mSyncData->load();
    }

    void Async::setSyncData(TPtrShared<AtomicBool> data) {
        mSyncData = std::move(data);
    }

    void Async::markAsLoaded() {
        BERSERK_COND_ERROR_RET(isPresent(), "Sync data is not present");
        mSyncData->store(true);
    }

    void Async::reset() {
        BERSERK_COND_ERROR_RET(isPresent(), "Sync data is not present");
        mSyncData->store(false);
    }

    void Async::release() {
        mSyncData.free();
    }

    void Async::create() {
        mSyncData = TPtrShared<AtomicBool>::make(false);
    }

    void Async::blockUntilLoaded() {
        BERSERK_COND_ERROR_RET(isPresent(), "Sync data is not present");

        while (!mSyncData->load()) {
            Thread::yield();
        }
    }

}