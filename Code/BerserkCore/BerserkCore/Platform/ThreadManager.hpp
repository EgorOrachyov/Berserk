/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_THREADMANAGER_HPP
#define BERSERK_THREADMANAGER_HPP

#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkCore/Platform/Thread.hpp>

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
        static Ref<Thread> CreateThread(const StringName &name, const Function<void()> &runnable) {
            return Impl::Instance().CreateThread(runnable, name);
        }

        /**
         * Attempts to find thread by name.
         *
         * @param name Thread name to find
         *
         * @return Thread if found or null
         */
        static Ref<Thread> GetThreadByName(const StringName &name) {
            return Impl::Instance().GetThreadByName(name);
        }

        /** @return Current thread */
        static Ref<Thread> GetCurrentThread() {
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

            virtual Ref<Thread> CreateThread(const Function<void()> &runnable, const StringName &name) = 0;
            virtual Ref<Thread> GetThreadByName(const StringName &name) = 0;
            virtual Ref<Thread> GetCurrentThread() = 0;

            virtual size_t GetHardwareConcurrency() = 0;
            virtual void CurrentThreadYield() = 0;
            virtual void CurrentThreadSleep(size_t microseconds) = 0;
        };

    };
}

#endif //BERSERK_THREADMANAGER_HPP