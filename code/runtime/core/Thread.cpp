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

#include <core/Thread.hpp>
#include <core/io/Logger.hpp>

#include <algorithm>
#include <cassert>

BRK_NS_BEGIN

Thread::~Thread() {
    Update();
    ExecuteBefore();
    ExecuteAfter();

    assert(mQueueBefore.empty());
    assert(mQueueAfter.empty());
    assert(mExecuteBefore.empty());
    assert(mExecuteAfter.empty());
}

void Thread::Enqueue(Thread::Callable callable, Thread::Flag flag) {
    std::lock_guard<yamc::spin_ttas::mutex> lock(mMutex);

    if (flag == Flag::Before)
        mQueueBefore.push_back(std::move(callable));
    else if (flag == Flag::After)
        mQueueAfter.push_back(std::move(callable));
    else
        BRK_ERROR("Unknown thread flag");
}

void Thread::Update() {
    assert(OnThread());
    std::lock_guard<yamc::spin_ttas::mutex> lock(mMutex);
    std::swap(mQueueBefore, mExecuteBefore);
    std::swap(mQueueAfter, mExecuteAfter);
}

void Thread::ExecuteBefore() {
    assert(OnThread());
    std::for_each(mExecuteBefore.begin(), mExecuteBefore.end(), [](Callable &callable) { callable(); });
    mExecuteBefore.clear();
}

void Thread::ExecuteAfter() {
    assert(OnThread());
    std::for_each(mExecuteAfter.begin(), mExecuteAfter.end(), [](Callable &callable) { callable(); });
    mExecuteAfter.clear();
}

bool Thread::OnThread() const {
    return std::this_thread::get_id() == mThreadId;
}

BRK_NS_END