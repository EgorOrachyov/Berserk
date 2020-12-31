/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxThreadManager.hpp>

namespace Berserk {
    namespace Platform {

        LinuxThreadManager::LinuxImpl::LinuxImpl() {
            Ref<LinuxThread> main = LinuxThread::Create(BERSERK_TEXT("MAIN-THREAD"), GetNextId());
            mThreads.Move(main);

            Provide(this);
        }

        LinuxThreadManager::LinuxImpl::~LinuxImpl() {
            for (auto& thread: mThreads) {
                thread->Join();
            }

            Remove(this);
        }

        Ref<Thread> LinuxThreadManager::LinuxImpl::CreateThread(const Function<void()> &runnable, const StringName &name) {
            Guard<Mutex> guard(mMutex);

            Ref<LinuxThread> thread = LinuxThread::Create(runnable, name, GetNextId());
            mThreads.Add(thread);

            return (Ref<Thread>) thread;
        }

        Ref<Thread> LinuxThreadManager::LinuxImpl::GetThreadByName(const StringName &name) {
            Guard<Mutex> guard(mMutex);

            for (auto& thread: mThreads) {
                if (thread->GetName() == name) {
                    return (Ref<Thread>) thread;
                }
            }

            return nullptr;
        }

        Ref<Thread> LinuxThreadManager::LinuxImpl::GetCurrentThread() {
            Guard<Mutex> guard(mMutex);

            std::thread::id id = std::this_thread::get_id();

            for (auto& thread: mThreads) {
                if (thread->GetNativeId() == id) {
                    return (Ref<Thread>) thread;
                }
            }

            return nullptr;
        }

        size_t LinuxThreadManager::LinuxImpl::GetHardwareConcurrency() {
            return std::thread::hardware_concurrency();
        }

        void LinuxThreadManager::LinuxImpl::CurrentThreadYield() {
            std::this_thread::yield();
        }

        void LinuxThreadManager::LinuxImpl::CurrentThreadSleep(size_t microseconds) {
            std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
        }

        Thread::ThreadId LinuxThreadManager::LinuxImpl::GetNextId() {
            return mManagedIdNext++;
        }

    }
}