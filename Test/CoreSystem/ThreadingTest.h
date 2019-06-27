//
// Created by Egor Orachyov on 27.06.2019.
//

#ifndef BERSERK_THREADINGTEST_H
#define BERSERK_THREADINGTEST_H

#include <Threading/Thread.h>

using namespace Berserk;

class ThreadingTest
{
public:

    static void ThreadRunTest()
    {
        auto run1 = []()
        {
            for (uint32 i = 0; i < 1000; i++)
            {
                LOG(ELogVerbosity::Display, "Thread 1");
            }
        };

        auto run2 = []()
        {
            for (uint32 i = 0; i < 1000; i++)
            {
                LOG(ELogVerbosity::Display, "Thread 2");
            }
        };

        Thread thread1(run1, 0, "thread1", false);
        Thread thread2(run2, 1, "thread2", false);

        thread1.join();
        thread2.join();
    }

    static void run()
    {
        ThreadRunTest();
    }

};


#endif //BERSERK_THREADINGTEST_H
