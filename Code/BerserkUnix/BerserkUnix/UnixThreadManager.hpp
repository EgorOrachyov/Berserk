/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXTHREADMANAGER_HPP
#define BERSERK_UNIXTHREADMANAGER_HPP

#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkUnix/UnixThread.hpp>

namespace Berserk {

    class UnixThreadManager: public ThreadManager {
    public:

        class UnixImpl final: public Impl {
        public:
            UnixImpl();
            ~UnixImpl() override;

            Ref<Thread> CreateThread(const Function<void()> &runnable, const StringName &name) override;
            Ref<Thread> GetThreadByName(const StringName &name) override;
            Ref<Thread> GetCurrentThread() override;

            size_t GetHardwareConcurrency() override;
            void CurrentThreadYield() override;
            void CurrentThreadSleep(size_t microseconds) override;

        private:
            Thread::ThreadId GetNextId();

            Array<Ref<UnixThread>> mThreads;
            Thread::ThreadId mManagedIdNext = 0;

            mutable Mutex mMutex;
        };

    };
}

#endif //BERSERK_UNIXTHREADMANAGER_HPP