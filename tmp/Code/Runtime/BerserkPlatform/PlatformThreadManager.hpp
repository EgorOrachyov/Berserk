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

#ifndef BERSERK_PLATFORMTHREADMANAGER_HPP
#define BERSERK_PLATFORMTHREADMANAGER_HPP

#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkPlatform/PlatformThread.hpp>

namespace Berserk {

    class PlatformThreadManager: public ThreadManager {
    public:

    class PImpl final: public ThreadManager::Impl {
        public:
            PImpl();
            ~PImpl() override;

            SharedPtr<Thread> CreateThread(const Function<void()> &runnable, const StringName &name) override;
            SharedPtr<Thread> GetThreadByName(const StringName &name) override;
            SharedPtr<Thread> GetCurrentThread() override;

            uint64 GetHardwareConcurrency() override;
            void CurrentThreadYield() override;
            void CurrentThreadSleep(uint64 microseconds) override;

        private:
            Thread::ThreadId GetNextId();

            Array<SharedPtr<PlatformThread>> mThreads;
            Thread::ThreadId mManagedIdNext = 0;

            mutable Mutex mMutex;
        };

    };
}

#endif //BERSERK_PLATFORMTHREADMANAGER_HPP