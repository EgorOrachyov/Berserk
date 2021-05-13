/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXTHREAD_HPP
#define BERSERK_UNIXTHREAD_HPP

#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <thread>

namespace Berserk {

    class UnixThread final: public Thread {
    public:
        explicit UnixThread(StringName name, ThreadId id);
        explicit UnixThread(const Function<void()> &runnable, StringName name, ThreadId id);
        ~UnixThread() override = default;

        void Join() override;

        bool CanJoin() const override;
        bool IsFinished() const override;

        Type GetType() const override;
        ThreadId GetManagedId() const override;
        StringName GetName() const override;

        std::thread::id GetNativeId() const;

    protected:
        void MarkFinished();

    private:
        StringName mName;
        ThreadId mManagedId;
        Type mType = Type::Secondary;
        AtomicBool mCanJoin;
        AtomicBool mIsFinished;
        std::thread mNative;
        std::thread::id mNativeId;

        mutable SpinMutex mMutex;
    };
}

#endif //BERSERK_UNIXTHREAD_HPP