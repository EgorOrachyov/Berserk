/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixThread.hpp>

namespace Berserk {

    UnixThread::UnixThread(StringName name, ThreadId id)
        : mName(std::move(name)), mManagedId(id), mType(Type::Main), mCanJoin(false), mIsFinished(false) {

        mNativeId = std::this_thread::get_id();
    }

    UnixThread::UnixThread(const Function<void()> &runnable, StringName name, ThreadId id)
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

    void UnixThread::Join() {
        volatile bool willJoin = false;

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

    bool UnixThread::CanJoin() const {
        return mCanJoin.load();
    }

    bool UnixThread::IsFinished() const {
        return mIsFinished.load();
    }

    Thread::Type UnixThread::GetType() const {
        return mType;
    }

    Thread::ThreadId UnixThread::GetManagedId() const {
        return mManagedId;
    }

    StringName UnixThread::GetName() const {
        return mName;
    }

    std::thread::id UnixThread::GetNativeId() const {
        return mNativeId;
    }

    void UnixThread::OnReleased() const {
        BERSERK_ASSERT(mIsFinished || mType == Type::Main);
        this->~UnixThread();
        Allocator().Deallocate((void*) this);
    }

    void UnixThread::MarkFinished() {
        mIsFinished.store(true);
    }

    Ref<UnixThread> UnixThread::Create(StringName name, ThreadId id) {
        auto memory = Allocator().Allocate(sizeof(UnixThread));
        auto thread = new (memory) UnixThread(std::move(name), id);
        return Ref<UnixThread>(thread, false);
    }

    Ref<UnixThread> UnixThread::Create(const Function<void()> &runnable, StringName name, ThreadId id) {
        auto memory = Allocator().Allocate(sizeof(UnixThread));
        auto thread = new (memory) UnixThread(runnable, std::move(name), id);
        return Ref<UnixThread>(thread, false);
    }

}