//
// Created by Egor Orachyov on 27.06.2019.
//

#ifndef BERSERK_THREADINGTEST_H
#define BERSERK_THREADINGTEST_H

#include <Threading/Thread.h>
#include <Threading/ThreadManager.h>

using namespace Berserk;

class ThreadingTest
{
public:

    static void ThreadRunTest()
    {
        class Run1 : public IRunnable
        {
        public:

            uint32 run() override {
                for (uint32 i = 0; i < 1000; i++)
                {
                    OutputDevice::printf("Thread 1\n");
                }
                return 0;
            }
        };

        class Run2 : public IRunnable
        {
        public:

            uint32 run() override {
                for (uint32 i = 0; i < 1000; i++)
                {
                    OutputDevice::printf("Thread 2\n");
                }
                return 0;
            }
        };

        Run1 run1;
        Run2 run2;

        Thread thread1(TSharedPtr<IRunnable>(&run1, nullptr), 0, "thread1", false);
        Thread thread2(TSharedPtr<IRunnable>(&run2, nullptr), 1, "thread2", false);

        thread1.join();
        thread2.join();

        OutputDevice::printf("Thread info: id: %u, name: '%s', joinable: %u \n",
                             thread1.getId(), thread1.getName(), thread1.isJoinable());
        OutputDevice::printf("Thread info: id: %u, name: '%s', joinable: %u \n",
                             thread2.getId(), thread2.getName(), thread2.isJoinable());
    }

    static void ThreadManagementTest()
    {
        IAllocator& mem = Allocator::get();

        ThreadManager threadManager;
        IThreadManager& manager = threadManager;

        class Task : public IRunnable
        {
        public:

            String mText;

            explicit Task(const char* text) : mText(text) {}

            uint32 run() override
            {
                for (uint32 i = 0; i < 10; i++)
                {
                    OutputDevice::printf("%s \n", mText.get());
                }
            }

        };

        TSharedPtr<IRunnable> task1(mem.engine_new_const<Task>("hello"), &mem);
        TSharedPtr<IRunnable> task2(mem.engine_new_const<Task>("world"), &mem);

        manager.createThread("thread_1", task1, false);
        manager.createThread("thread_2", task2, false);
    }

    static void run()
    {
        //ThreadRunTest();
        ThreadManagementTest();
    }

};


#endif //BERSERK_THREADINGTEST_H
