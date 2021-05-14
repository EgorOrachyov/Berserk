/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixThreadManager.hpp>

namespace Berserk {

    UnixThreadManager::UnixImpl::UnixImpl() {
        SharedPtr<UnixThread> main = SharedPtr<UnixThread>::Make(BERSERK_TEXT("MAIN-THREAD"), GetNextId());
        mThreads.Move(std::move(main));

        Provide(this);
    }

    UnixThreadManager::UnixImpl::~UnixImpl() {
        for (auto& thread: mThreads) {
            thread->Join();
        }

        Remove(this);
    }

    SharedPtr<Thread> UnixThreadManager::UnixImpl::CreateThread(const Function<void()> &runnable, const StringName &name) {
        Guard<Mutex> guard(mMutex);

        SharedPtr<UnixThread> thread = SharedPtr<UnixThread>::Make(runnable, name, GetNextId());
        mThreads.Add(thread);

        return (SharedPtr<Thread>) thread;
    }

    SharedPtr<Thread> UnixThreadManager::UnixImpl::GetThreadByName(const StringName &name) {
        Guard<Mutex> guard(mMutex);

        for (auto& thread: mThreads) {
            if (thread->GetName() == name) {
                return (SharedPtr<Thread>) thread;
            }
        }

        return {};
    }

    SharedPtr<Thread> UnixThreadManager::UnixImpl::GetCurrentThread() {
        Guard<Mutex> guard(mMutex);

        std::thread::id id = std::this_thread::get_id();

        for (auto& thread: mThreads) {
            if (thread->GetNativeId() == id) {
                return (SharedPtr<Thread>) thread;
            }
        }

        return {};
    }

    size_t UnixThreadManager::UnixImpl::GetHardwareConcurrency() {
        return std::thread::hardware_concurrency();
    }

    void UnixThreadManager::UnixImpl::CurrentThreadYield() {
        std::this_thread::yield();
    }

    void UnixThreadManager::UnixImpl::CurrentThreadSleep(size_t microseconds) {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }

    Thread::ThreadId UnixThreadManager::UnixImpl::GetNextId() {
        return mManagedIdNext++;
    }
}