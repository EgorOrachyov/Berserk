/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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