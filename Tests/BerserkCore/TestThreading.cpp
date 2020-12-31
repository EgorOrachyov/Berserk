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
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Hardware concurrency: {0}"), Platform::ThreadManager::GetHardwareConcurrency());

    size_t iterations = 1000;

    auto task = [=](){
        auto thread = Platform::ThreadManager::GetCurrentThread();

        for (auto i = 0; i < iterations; i++) {
            BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Thread[{0}]={1}"), thread->GetName(), i);
            Platform::ThreadManager::CurrentThreadYield();
        }
    };

    Ref<Platform::Thread> thread = Platform::ThreadManager::CreateThread(task, BERSERK_TEXT("TEST-THREAD"));

    auto thisThread = Platform::ThreadManager::GetCurrentThread();

    for (auto i = 0; i < iterations; i++) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Thread[{0}]={1}"), thisThread->GetName(), i);
        Platform::ThreadManager::CurrentThreadYield();
    }

    thread->Join();

    EXPECT_TRUE(thread->IsFinished());
}

BERSERK_GTEST_MAIN