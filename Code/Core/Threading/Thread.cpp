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

namespace Berserk {

    static Mutex gAccessMutex;
    static TArray<TPair<std::thread::id,CString>> gThreadsMarkers;

    void Thread::createThread(CString name, const Function<void()> &job) {
        Guard guard(gAccessMutex);

        std::thread thread(job);
        auto id = thread.get_id();
        thread.detach();

        auto& marker = gThreadsMarkers.emplace();
        marker.first() = id;
        marker.second() = std::move(name);
    }

    void Thread::getDebugName(CString &name) {
        Guard guard(gAccessMutex);

        auto id = std::this_thread::get_id();

        for (auto& marker: gThreadsMarkers) {
            if (marker.first() == id) {
                name = marker.second();
                return;
            }
        }

        BERSERK_ERROR("This thread has no debug marker");
    }

    void Thread::setDebugName(CString name) {
        Guard guard(gAccessMutex);

        auto id = std::this_thread::get_id();

        for (auto& marker: gThreadsMarkers) {
            if (marker.first() == id) {
                marker.second() = std::move(name);
                return;
            }
        }

        auto& marker = gThreadsMarkers.emplace();
        marker.first() = id;
        marker.second() = std::move(name);
    }

    void Thread::sleep(uint64 ns) {
        std::chrono::nanoseconds duration(ns);
        std::this_thread::sleep_for(duration);
    }

    void Thread::yield() {
        std::this_thread::yield();
    }
    
}