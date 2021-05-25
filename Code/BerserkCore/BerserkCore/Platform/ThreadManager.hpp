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

#ifndef BERSERK_THREADMANAGER_HPP
#define BERSERK_THREADMANAGER_HPP

#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

namespace Berserk {

    /**
     * @brief Thread manager
     *
     * Manages all application threads.
     */
    class ThreadManager {
    public:

        /**
         * Creates and runs new thread with specified runnable function
         *
         * @param runnable Task to run on thread
         * @param name Thread user-friendly name
         *
         * @return Created thread
         */
        static SharedPtr<Thread> CreateThread(const StringName &name, const Function<void()> &runnable) {
            return Impl::Instance().CreateThread(runnable, name);
        }

        /**
         * Attempts to find thread by name.
         *
         * @param name Thread name to find
         *
         * @return Thread if found or null
         */
        static SharedPtr<Thread> GetThreadByName(const StringName &name) {
            return Impl::Instance().GetThreadByName(name);
        }

        /** @return Current thread */
        static SharedPtr<Thread> GetCurrentThread() {
            return Impl::Instance().GetCurrentThread();
        }

        /** @return Platform physical or logical CPU cores count */
        static size_t GetHardwareConcurrency() {
            return Impl::Instance().GetHardwareConcurrency();
        }

        /** Yield execution of the current thread */
        static void CurrentThreadYield() {
            Impl::Instance().CurrentThreadYield();
        }

        /** Sleep current thread for specified microseconds time */
        static void CurrentThreadSleep(size_t microseconds) {
            Impl::Instance().CurrentThreadSleep(microseconds);
        }

    protected:

        class Impl : public Singleton<Impl> {
        public:
            virtual ~Impl() = default;

            virtual SharedPtr<Thread> CreateThread(const Function<void()> &runnable, const StringName &name) = 0;
            virtual SharedPtr<Thread> GetThreadByName(const StringName &name) = 0;
            virtual SharedPtr<Thread> GetCurrentThread() = 0;

            virtual size_t GetHardwareConcurrency() = 0;
            virtual void CurrentThreadYield() = 0;
            virtual void CurrentThreadSleep(size_t microseconds) = 0;
        };

    };
}

#endif //BERSERK_THREADMANAGER_HPP