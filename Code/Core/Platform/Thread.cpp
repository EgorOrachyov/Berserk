/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Thread.h>

namespace Berserk {

    /** Internal info to check threads */
    struct ThreadInfo {
        std::thread::id id;
        CString name;
    };

    Mutex Thread::mMutex;
    TArray<ThreadInfo> Thread::mThreads;

    Thread::Thread(Function<void()> execute)
        : mHandle(std::move(execute)) {

    }

    bool Thread::canJoin() const {
        return mHandle.joinable();
    }

    void Thread::tryJoin() {
        if (mHandle.joinable())
            mHandle.join();
    }

    void Thread::tryDetach() {
        mHandle.detach();
    }
    
    void Thread::setName(const CString &name) {
        Guard guard(mMutex);
        auto thisId = mHandle.get_id();

        for (auto& t: mThreads) {
            if (t.id == thisId) {
                t.name = name;
            }
        }

        addNullForCurrent(name);
    }
    
    CString Thread::getName() {
        Guard guard(mMutex);
        CString result;
        auto thisId = mHandle.get_id();
        
        for (auto& t: mThreads) {
            if (t.id == thisId) {
                result = t.name;
                return result;
            }
        }

        addNullForCurrent("");

        return "";
    }

    TPtrUnique<Thread> Thread::create(Function<void()> execute) {
        Guard guard(mMutex);
        auto thread = new (Memory::allocate(sizeof(Thread))) Thread(std::move(execute));
        TPtrUnique<Thread> ptr(thread, &Memory::DEFAULT_DEALLOC);
        auto& info = mThreads.emplace();
        info.id = thread->mHandle.get_id();

        return ptr;
    }

    void Thread::sleepCurrent(uint64 ns) {
        std::chrono::nanoseconds duration(ns);
        std::this_thread::sleep_for(duration);
    }

    void Thread::yieldCurrent() {
        std::this_thread::yield();
    }
    
    void Thread::setNameCurrent(const CString &name) {
        Guard guard(mMutex);
        auto thisId = std::this_thread::get_id();

        for (auto& t: mThreads) {
            if (t.id == thisId) {
                t.name = name;
            }
        }

        addNullForCurrent(name);
    }

    CString Thread::getNameCurrent() {
        Guard guard(mMutex);

        CString result;
        auto thisId = std::this_thread::get_id();

        for (auto& t: mThreads) {
            if (t.id == thisId) {
                result = t.name;
                return result;
            }
        }

        addNullForCurrent("");

        return "";
    }

    uint32 Thread::getHardwareConcurrency() {
        return std::thread::hardware_concurrency();
    }

    void Thread::addNullForCurrent(CString name) {
        auto& null = mThreads.emplace();
        null.name = std::move(name);
        null.id = std::this_thread::get_id();
    }

}
