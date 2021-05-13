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
        Ref<UnixThread> main = UnixThread::Create(BERSERK_TEXT("MAIN-THREAD"), GetNextId());
        mThreads.Move(main);

        Provide(this);
    }

    UnixThreadManager::UnixImpl::~UnixImpl() {
        for (auto& thread: mThreads) {
            thread->Join();
        }

        Remove(this);
    }

    Ref<Thread> UnixThreadManager::UnixImpl::CreateThread(const Function<void()> &runnable, const StringName &name) {
        Guard<Mutex> guard(mMutex);

        Ref<UnixThread> thread = UnixThread::Create(runnable, name, GetNextId());
        mThreads.Add(thread);

        return (Ref<Thread>) thread;
    }

    Ref<Thread> UnixThreadManager::UnixImpl::GetThreadByName(const StringName &name) {
        Guard<Mutex> guard(mMutex);

        for (auto& thread: mThreads) {
            if (thread->GetName() == name) {
                return (Ref<Thread>) thread;
            }
        }

        return nullptr;
    }

    Ref<Thread> UnixThreadManager::UnixImpl::GetCurrentThread() {
        Guard<Mutex> guard(mMutex);

        std::thread::id id = std::this_thread::get_id();

        for (auto& thread: mThreads) {
            if (thread->GetNativeId() == id) {
                return (Ref<Thread>) thread;
            }
        }

        return nullptr;
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