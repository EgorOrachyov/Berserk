//
// Created by Egor Orachyov on 27.06.2019.
//

#ifndef BERSERK_THREADINGTEST_H
#define BERSERK_THREADINGTEST_H

#include <Threading/Thread.h>
#include <Threading/ThreadManager.h>
#include <Threading/Async.h>
#include <Resource/PtrUtils.h>

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
                             thread1.getId(), thread1.getName().get(), thread1.isJoinable());
        OutputDevice::printf("Thread info: id: %u, name: '%s', joinable: %u \n",
                             thread2.getId(), thread2.getName().get(), thread2.isJoinable());
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

        TSharedPtr<Task> task1(mem.mem_new<Task>("hello"), &mem);
        TSharedPtr<Task> task2(mem.mem_new<Task>("world"), &mem);

        manager.createThread("thread_1", (TSharedPtr<IRunnable>) task1, false);
        manager.createThread("thread_2", (TSharedPtr<IRunnable>) task2, false);
    }

    static void ThreadAsyncTest()
    {
        class Task : public IRunnable
        {
        public:
            Task(std::function<void()> fun) : function(std::move(fun)) {}

            std::function<void()> function;
            uint32 run() override {
                function();
                return 0;
            }
        };

        IAllocator& allocator = Allocator::get();
        ThreadManager manager;

        auto data = mem_new_shared<AsyncData>();

        Async async(data);

        auto task1 = [=]() { OutputDevice::printf("wait\n"); async.blockUntilCompleted(); OutputDevice::printf("block\n"); };
        auto task2 = [=]() mutable { uint64 i = 0; while (i++ < 1000000000) {} async._complete(); OutputDevice::printf("done\n"); };

        auto proxy1 = mem_new_shared<Task>(task1);
        auto proxy2 = mem_new_shared<Task>(task2);

        manager.createThread("thread_1", (TSharedPtr<IRunnable>) proxy1, false);
        manager.createThread("thread_2", (TSharedPtr<IRunnable>) proxy2, false);
    }

    static void run()
    {
        //ThreadRunTest();
        //ThreadManagementTest();
        ThreadAsyncTest();
    }

};


#endif //BERSERK_THREADINGTEST_H
