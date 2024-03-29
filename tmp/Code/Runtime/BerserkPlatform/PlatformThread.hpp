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

#ifndef BERSERK_PLATFORMTHREAD_HPP
#define BERSERK_PLATFORMTHREAD_HPP

#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <thread>

namespace Berserk {

    class PlatformThread final: public Thread {
    public:
        explicit PlatformThread(StringName name, ThreadId id);
        explicit PlatformThread(const Function<void()> &runnable, StringName name, ThreadId id);
        ~PlatformThread() override = default;

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

#endif //BERSERK_PLATFORMTHREAD_HPP