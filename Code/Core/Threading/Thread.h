/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_THREAD_H
#define BERSERK_THREAD_H

#include <Containers/TArray.h>
#include <Platform/Mutex.h>
#include <String/CString.h>
#include <TPtrUnique.h>
#include <thread>

namespace Berserk {

    class Thread {
    public:

        /** @return True if thread can be joined */
        bool canJoin() const;

        /** Attempts to join thread */
        void tryJoin();

        /** Attempts to make daemon */
        void tryDetach();

        /** Sets name for the thread for debug purposes */
        void setName(const CString& name);

        /** @return Thread name (for debugging) */
        CString getName();

        /** @return Creates thread and runs execute function */
        static TPtrUnique<Thread> create(Function<void()> execute);

        /** Sleep this thread for nano-seconds */
        static void sleepCurrent(uint64 ns);

        /** Yields this thread executions */
        static void yieldCurrent();

        /** Sets name for the thread for debug purposes */
        static void setNameCurrent(const CString& name);

        /** @return Thread name (for debugging) */
        static CString getNameCurrent();

        /** @return Number of the threads (or hyper-threads) */
        static uint32 getHardwareConcurrency();

    private:

        static void addNullForCurrent(CString name);

        Thread(Function<void()> execute);

        std::thread mHandle;

        static Mutex mMutex;
        static TArray<class ThreadInfo> mThreads;

        friend class NullThreadMain;
    };

}

#endif //BERSERK_THREAD_H