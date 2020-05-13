/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Threading/Thread.h>
#include <Platform/Mutex.h>
#include <Containers/TArray.h>
#include <TPair.h>
#include <ErrorMacro.h>
#include <thread>

namespace Berserk {

    struct ThreadData {
        CString name;
        EThreadType type = EThreadType::User;
    };

    static thread_local ThreadData gThreadLocalData;
    static std::thread::id gMainThread = std::this_thread::get_id();

    void Thread::createThread(const Function<void()> &job) {
        std::thread thread(job);
        thread.detach();
    }

    void Thread::setThreadInfo(CString name, EThreadType threadType) {
        gThreadLocalData.name = std::move(name);
        gThreadLocalData.type = threadType;
    }

    void Thread::getDebugName(CString &name) {
        name = gThreadLocalData.name;
    }

    void Thread::getThreadInfo(CString &name, EThreadType &threadType) {
        name = gThreadLocalData.name;
        threadType = gThreadLocalData.type;
    }

    void Thread::sleep(uint64 ns) {
        std::chrono::nanoseconds duration(ns);
        std::this_thread::sleep_for(duration);
    }

    void Thread::yield() {
        std::this_thread::yield();
    }

    bool Thread::isMainThread() {
        return std::this_thread::get_id() == gMainThread;
    }

    bool Thread::isJobThread() {
        return gThreadLocalData.type == EThreadType::Job;
    }

    uint32 Thread::getHardwareConcurrency() {
        return std::thread::hardware_concurrency();
    }

}