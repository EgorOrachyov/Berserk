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
                OutputDevice::printf("Thread 1\n");
            }
        };

        auto run2 = []()
        {
            for (uint32 i = 0; i < 1000; i++)
            {
                OutputDevice::printf("Thread 2\n");
            }
        };

        Thread thread1(run1, 0, "thread1", false);
        Thread thread2(run2, 1, "thread2", false);

        thread1.join();
        thread2.join();

        OutputDevice::printf("Thread info: id: %u, name: '%s', joinable: %u \n",
                             thread1.getId(), thread1.getName(), thread1.isJoinable());
        OutputDevice::printf("Thread info: id: %u, name: '%s', joinable: %u \n",
                             thread2.getId(), thread2.getName(), thread2.isJoinable());
    }

    static void run()
    {
        ThreadRunTest();
    }

};


#endif //BERSERK_THREADINGTEST_H
