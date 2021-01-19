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
#include <BerserkCore/Threading/CommandBuffer.hpp>
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
    const size_t totalThreads = 4;
    const size_t totalWork = 1000;
    const size_t sleepTime = 100;
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

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Join threads:"));
    for (const auto& thread: threads) {
        thread->Join();
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Name=\"{0}\" ManagedId={1}"), thread->GetName(), thread->GetManagedId());
    }

    EXPECT_EQ(counter.load(), totalThreads * totalWork);
}

TEST_F(ThreadingFixture, CommandBuffer) {
    Ref<CommandBuffer> buffer = CommandBuffer::Create();

    const size_t N = 1000;
    size_t counter = 0;
    size_t reference = 0;

    for (auto i = 0; i < N; i++) {
        buffer->Enqueue([=, &counter](){
            counter += i * i + 3 * i + 13;
        });

        reference += i * i + 3 * i + 13;
    }

    for (auto cmd: buffer->GetCommands()) {
        cmd->Execute();
    }

    EXPECT_EQ(counter, reference);
}

TEST_F(ThreadingFixture, CommandBufferCycled) {
    Ref<CommandBuffer> buffer = CommandBuffer::Create();

    const size_t N = 1000;
    const size_t C = 10;
    size_t counter = 0;
    size_t reference = 0;

    for (auto j = 0; j < C; j++) {
        for (auto i = 0; i < N; i++) {
            buffer->Enqueue([=, &counter](){
                counter += i * i + 3 * i + 13;
            });

            reference += i * i + 3 * i + 13;
        }

        for (auto cmd: buffer->GetCommands()) {
            cmd->Execute();
        }

        buffer->Clear();
    }

    EXPECT_EQ(counter, reference);
}

BERSERK_GTEST_MAIN