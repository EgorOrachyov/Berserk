/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Debug/Debug.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(ThreadingFixture)

TEST_F(ThreadingFixture, BasicThread) {
    Platform::AtomicUint64 total(0);
    size_t iterations = 1000;

    auto task = [=, &total](){
        auto thread = Platform::ThreadManager::GetCurrentThread();

        for (auto i = 0; i < iterations; i++) {
            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Thread[{0}]={1}"), thread->GetName(), i);
            Platform::ThreadManager::CurrentThreadYield();
            total.fetch_add(1);
        }
    };

    Ref<Platform::Thread> thread = Platform::ThreadManager::CreateThread(BERSERK_TEXT("TEST-THREAD"), task);

    auto thisThread = Platform::ThreadManager::GetCurrentThread();

    for (auto i = 0; i < iterations; i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Thread[{0}]={1}"), thisThread->GetName(), i);
        Platform::ThreadManager::CurrentThreadYield();
        total.fetch_add(1);
    }

    thread->Join();

    EXPECT_TRUE(thread->IsFinished());
    EXPECT_EQ(total.load(), iterations * 2);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Hardware concurrency: {0}"), Platform::ThreadManager::GetHardwareConcurrency());
}

TEST_F(ThreadingFixture, BasicPool) {
    const Berserk::size_t totalThreads = 4;
    const Berserk::size_t totalWork = 1000;
    const Berserk::size_t sleepTime = 100;
    Platform::AtomicUint64 counter(0);

    Array<Ref<Platform::Thread>> threads;

    for (auto i = 0; i < totalThreads; i++) {
        auto thread = Platform::ThreadManager::CreateThread(String{BERSERK_TEXT("THREAD-")} + String::From(i), [&](){
            for (auto j = 0; j < totalWork; j++) {
                Platform::ThreadManager::CurrentThreadSleep(sleepTime);
                counter.fetch_add(1);
            }
        });

        threads.Add(thread);
    }

    for (const auto& thread: threads) {
        thread->Join();
    }

    EXPECT_EQ(counter.load(), totalThreads * totalWork);
}

BERSERK_GTEST_MAIN