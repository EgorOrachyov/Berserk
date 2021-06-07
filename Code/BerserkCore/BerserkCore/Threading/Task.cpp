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

#include <BerserkCore/Threading/Task.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {

    Task::Task(StringName name, TaskPriority priority, Callable callable)
        : mName(std::move(name)), mFunction(std::move(callable)), mPriority(priority) {

    }

    TaskStatus Task::GetStatus() const {
        return (TaskStatus)mStatus.load(std::memory_order_acquire);
    }

    bool Task::IsCompleted() const {
        return GetStatus() == TaskStatus::Completed;
    }

    bool Task::IsCanceled() const {
        return GetStatus() == TaskStatus::Canceled;
    }

    bool Task::IsStarted() const {
        auto status = GetStatus();
        return status == TaskStatus::InProgress || status == TaskStatus::Completed;
    }

    void Task::BlockUntilCompleted(size_t sleep) const {
        assert(IsStarted());

        if (GetStatus() == TaskStatus::Inactive)
            return;

        while (GetStatus() == TaskStatus::InProgress) {
            ThreadManager::CurrentThreadSleep(sleep);
        }
    }

    bool Task::CanStart(bool &shouldRemoveFromQueue) {
        return GetStatus() == TaskStatus::InProgress;
    }

    void Task::NotifyQueued() {
        assert(GetStatus() != TaskStatus::InProgress);
        mStatus.store((uint32)TaskStatus::InProgress, std::memory_order_release);
    }

    void Task::NotifyCompleted() {
        mStatus.store((uint32)TaskStatus::Completed, std::memory_order_release);
    }

    void Task::NotifyCanceled() {
        mStatus.store((uint32)TaskStatus::Canceled, std::memory_order_release);
    }

}