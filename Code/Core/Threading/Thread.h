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
#include <thread>

namespace Berserk {

    /**
     * Std based thread utility for thread access and debug namings
     */
    class Thread {
    public:

        /** Creates new thread with debug marker */
        static void createThread(class CString name, const Function<void()> &job);

        /** Set this thread debug name */
        static void setDebugName(class CString name);

        /** Reads this thread debug name */
        static void getDebugName(class CString& name);

        /** Sleep this thread for nano-seconds */
        static void sleep(uint64 ns);

        /** Yields this thread executions */
        static void yield();

    };

}

#endif //BERSERK_THREAD_H