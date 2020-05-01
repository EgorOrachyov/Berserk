/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Threading/Thread.h>
#include <Platform/Mutex.h>
#include <TArray.h>
#include <TPair.h>
#include <ErrorMacro.h>
#include <thread>

namespace Berserk {

    struct ThreadData {
        CString name;
        EThreadType type;
    };

    static std::thread::id gMainThread;
    static Mutex gAccessMutex;
    static TArray<TPair<std::thread::id,ThreadData>> gThreadsData;

    void Thread::createThread(CString name, const Function<void()> &job) {
        Thread::createThread(std::move(name), EThreadType::User, job);
    }

    void Thread::createThread(CString name, EThreadType type, const Function<void()> &job) {
        Guard guard(gAccessMutex);

        std::thread thread(job);
        auto id = thread.get_id();
        thread.detach();

        auto& marker = gThreadsData.emplace();
        marker.first() = id;
        marker.second().name = std::move(name);
        marker.second().type = type;
    }

    void Thread::getDebugName(CString &name) {
        Guard guard(gAccessMutex);

        auto id = std::this_thread::get_id();

        for (auto& marker: gThreadsData) {
            if (marker.first() == id) {
                name = marker.second().name;
                return;
            }
        }

        BERSERK_ERROR("This thread has associated info");
    }

    void Thread::setDebugName(CString name) {
        Guard guard(gAccessMutex);

        auto id = std::this_thread::get_id();

        for (auto& marker: gThreadsData) {
            if (marker.first() == id) {
                marker.second().name = std::move(name);
                return;
            }
        }

        auto& marker = gThreadsData.emplace();
        marker.first() = id;
        marker.second().name = std::move(name);
    }

    void Thread::getThreadInfo(CString &name, EThreadType &threadType) {
        Guard guard(gAccessMutex);

        auto id = std::this_thread::get_id();

        for (auto& marker: gThreadsData) {
            if (marker.first() == id) {
                name = marker.second().name;
                threadType = marker.second().type;
                return;
            }
        }

        BERSERK_ERROR("This thread has associated info");
    }

    void Thread::sleep(uint64 ns) {
        std::chrono::nanoseconds duration(ns);
        std::this_thread::sleep_for(duration);
    }

    void Thread::yield() {
        std::this_thread::yield();
    }

    bool Thread::isMainThread() {
        Guard guard(gAccessMutex);
        return std::this_thread::get_id() == gMainThread;
    }

    bool Thread::isJobThread() {
        Guard guard(gAccessMutex);

        auto id = std::this_thread::get_id();

        for (auto& data: gThreadsData) {
            if (data.first() == id)
                return data.second().type == EThreadType::Job;
        }

        return false;
    }

    uint32 Thread::getHardwareConcurrency() {
        return std::thread::hardware_concurrency();
    }

    void Thread::registerMainThread() {
        Guard guard(gAccessMutex);
        gMainThread = std::this_thread::get_id();

        auto& marker = gThreadsData.emplace();
        marker.first() = gMainThread;
        marker.second().name = "Main";
        marker.second().type = EThreadType::Main;
    }

}