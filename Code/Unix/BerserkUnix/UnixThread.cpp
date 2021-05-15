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

    void UnixThread::MarkFinished() {
        mIsFinished.store(true);
    }

}