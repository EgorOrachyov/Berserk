/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUXTHREADMANAGER_HPP
#define BERSERK_LINUXTHREADMANAGER_HPP

#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkLinux/LinuxThread.hpp>

namespace Berserk {
    namespace Platform {

        class LinuxThreadManager: public ThreadManager {
        public:

            class LinuxImpl final: public Impl {
            public:
                LinuxImpl();
                ~LinuxImpl() override;

                Ref<Thread> CreateThread(const Function<void()> &runnable, const StringName &name) override;
                Ref<Thread> GetThreadByName(const StringName &name) override;
                Ref<Thread> GetCurrentThread() override;

                size_t GetHardwareConcurrency() override;
                void CurrentThreadYield() override;
                void CurrentThreadSleep(size_t microseconds) override;

            private:
                Thread::ThreadId GetNextId();

                Array<Ref<LinuxThread>> mThreads;
                Thread::ThreadId mManagedIdNext = 0;

                mutable Mutex mMutex;
            };

        };

    }
}

#endif //BERSERK_LINUXTHREADMANAGER_HPP