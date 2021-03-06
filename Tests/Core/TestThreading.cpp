/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Templates/CommandQueue.hpp>
#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Templates/AsyncCommandQueue.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Threading/AsyncOp.hpp>
#include <BerserkCore/Threading/Task.hpp>
#include <BerserkCore/Threading/TaskManager.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(ThreadingFixture)

TEST_F(ThreadingFixture, CommandQueue) {
    auto variable = 0;
    auto counter = 100;

    CallbackQueue queue;

    for (auto i = 0; i < counter; i++) {
        queue.Enqueue([&](){
           variable++;
        });
    }

    queue.Execute();

    EXPECT_EQ(variable, counter);
}

TEST_F(ThreadingFixture, AsyncCommandQueue) {
    uint64 variable = 0;
    uint64 counter = 100;
    uint64 waves = 100;
    uint64 executed = 0;

    AsyncCommandQueueConsumer<> consumer;
    AsyncCommandQueue<> producer = consumer.CreateQueue();

    auto t1 = ThreadManager::CreateThread("T1", [&](){
        for (auto i = 0; i < waves; i++) {
            for (auto j = 0; j < counter; j++) {
                producer.Submit([&](){
                    variable++;
                });
            }

            producer.Commit();
            ThreadManager::CurrentThreadSleep(1000);

            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Commit"));
        }
    });

    auto t2 = ThreadManager::CreateThread("T2", [&](){
        while (executed < waves) {
            executed += consumer.ExecutePending();
        }
    });

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0}"), variable);

    t1->Join();
    t2->Join();

    auto stats = consumer.GetStats();

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("{0} {1} {2} {3}"),
                          (uint64) stats.totalQueues, (uint64) stats.freeQueues, (uint64) stats.submitted, (uint64) stats.pendingExecution);


    EXPECT_EQ(variable, counter * waves);
}

TEST_F(ThreadingFixture, BasicThread) {
    AtomicUint64 total(0);
    size_t iterations = 1000;

    auto task = [=, &total](){
        auto thread = ThreadManager::GetCurrentThread();

        for (auto i = 0; i < iterations; i++) {
            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Thread[{0}]={1}"), thread->GetName(), i);
            ThreadManager::CurrentThreadYield();
            total.fetch_add(1);
        }
    };

    auto thread = ThreadManager::CreateThread(BERSERK_TEXT("TEST-THREAD"), task);

    auto thisThread = ThreadManager::GetCurrentThread();

    for (auto i = 0; i < iterations; i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Thread[{0}]={1}"), thisThread->GetName(), i);
        ThreadManager::CurrentThreadYield();
        total.fetch_add(1);
    }

    thread->Join();

    EXPECT_TRUE(thread->IsFinished());
    EXPECT_EQ(total.load(), iterations * 2);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Hardware concurrency: {0}"), (uint64) ThreadManager::GetHardwareConcurrency());
}

TEST_F(ThreadingFixture, BasicPool) {
    const size_t totalThreads = 4;
    const size_t totalWork = 1000;
    const size_t sleepTime = 100;
    AtomicUint64 counter(0);

    Array<SharedPtr<Thread>> threads;

    for (auto i = 0; i < totalThreads; i++) {
        auto thread = ThreadManager::CreateThread(String{BERSERK_TEXT("THREAD-")} + String::Fromi32(i), [&](){
            for (auto j = 0; j < totalWork; j++) {
                ThreadManager::CurrentThreadSleep(sleepTime);
                counter.fetch_add(1);
            }
        });

        threads.Add(thread);
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Join threads:"));
    for (const auto& thread: threads) {
        thread->Join();
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Name=\"{0}\" ManagedId={1}"), thread->GetName(), (uint64) thread->GetManagedId());
    }

    EXPECT_EQ(counter.load(), totalThreads * totalWork);
}

TEST_F(ThreadingFixture, AsyncOp) {
    auto data = SharedPtr<AsyncOpData<String>>::Make();
    AsyncOp<String> asyncOp(data);

    auto worker = ThreadManager::CreateThread(BERSERK_TEXT("WORKER-THREAD"), [=](){
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Start job"));

        ThreadManager::CurrentThreadSleep(1000 * 10);
        data->Finish(SharedPtr<String>::Make(BERSERK_TEXT("Some useful job")));

        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Finish job"));
    });

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Wait"));

    asyncOp.BlockUntilCompleted();

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Completed: result={0}"), *asyncOp.GetResult());

    worker->Join();
}

TEST_F(ThreadingFixture, Task) {
    AtomicUint64 counter{0};
    uint64 n = 1000;

    auto task = SharedPtr<Task>::Make(BERSERK_TEXT("test-task"), TaskPriority::Medium, [&](TaskContext& context){
        for (uint64 i = 0; i < n; ++i) {
            counter.fetch_add(1);
        }
    });

    TaskManager::SubmitTask(task);
    task->BlockUntilCompleted();

    EXPECT_EQ(n, counter.load());
}

BERSERK_GTEST_MAIN