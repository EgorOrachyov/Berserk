/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxThread.hpp>

namespace Berserk {
    namespace Platform {

        LinuxThread::LinuxThread(StringName name, ThreadId id)
            : mName(std::move(name)), mManagedId(id), mType(Type::Main), mCanJoin(false), mIsFinished(false) {

            mNativeId = std::this_thread::get_id();
        }

        LinuxThread::LinuxThread(const Function<void()> &runnable, StringName name, ThreadId id)
            : mName(std::move(name)), mManagedId(id), mType(Type::Secondary), mCanJoin(false), mIsFinished(false) {

            auto runContext = [=]() {
                // Run actual task
                runnable();
                // Notify us that we finished
                this->MarkFinished();
            };

            mNative = std::move(std::thread(runContext));
            mNativeId = mNative.get_id();
            mCanJoin.store(true);
        }

        void LinuxThread::Join() {
            bool willJoin = false;

            {
                Guard<SpinMutex> guard(mMutex);
                if (mCanJoin.load() && mNative.joinable()) {
                    mCanJoin.store(false);
                    willJoin = true;
                }
            }

            if (willJoin) {
                mNative.join();
            }
        }

        bool LinuxThread::CanJoin() const {
            return mCanJoin.load();
        }

        bool LinuxThread::IsFinished() const {
            return mIsFinished.load();
        }

        Thread::Type LinuxThread::GetType() const {
            return mType;
        }

        Thread::ThreadId LinuxThread::GetManagedId() const {
            return mManagedId;
        }

        StringName LinuxThread::GetName() const {
            return mName;
        }

        std::thread::id LinuxThread::GetNativeId() const {
            return mNativeId;
        }

        void LinuxThread::OnReleased() const {
            BERSERK_ASSERT(mIsFinished || mType == Type::Main);
            this->~LinuxThread();
            Platform::Allocator().Deallocate((void*) this);
        }

        void LinuxThread::MarkFinished() {
            mIsFinished.store(true);
        }

        Ref<LinuxThread> LinuxThread::Create(StringName name, ThreadId id) {
            auto memory = Platform::Allocator().Allocate(sizeof(LinuxThread));
            auto thread = new (memory) LinuxThread(std::move(name), id);
            return Ref<LinuxThread>(thread, false);
        }

        Ref<LinuxThread> LinuxThread::Create(const Function<void()> &runnable, StringName name, ThreadId id) {
            auto memory = Platform::Allocator().Allocate(sizeof(LinuxThread));
            auto thread = new (memory) LinuxThread(runnable, std::move(name), id);
            return Ref<LinuxThread>(thread, false);
        }

    }
}