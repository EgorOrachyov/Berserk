/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_THREAD_H
#define BERSERK_THREAD_H

#include <Typedefs.h>

namespace Berserk {

    /** Types of the threads */
    enum class EThreadType {
        Main,   /** Main thread - exists all the engine runtime */
        Job,    /** Worker threads for the job system */
        User    /** Thread created by user */
    };

    /**
     * Std based thread utility for thread access and debug namings
     */
    class Thread {
    public:

        /** Creates new thread with name */
        static void createThread(class CString name, const Function<void()> &job);

        /** Creates new thread with name */
        static void createThread(class CString name, EThreadType type, const Function<void()> &job);

        /** Set this thread debug name */
        static void setDebugName(class CString name);

        /** Reads this thread debug name */
        static void getDebugName(class CString& name);

        /** Query thread info */
        static void getThreadInfo(class CString& name, EThreadType& threadType);

        /** Sleep this thread for nano-seconds */
        static void sleep(uint64 ns);

        /** Yields this thread executions */
        static void yield();

        /** @return True, if it is main thread */
        static bool isMainThread();

        /** @return True, if it is job thread */
        static bool isJobThread();

        /** @return Number of the threads (or hyper-threads) */
        static uint32 getHardwareConcurrency();

    };

}

#endif //BERSERK_THREAD_H