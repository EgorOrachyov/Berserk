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

#include <BerserkCore/Threading/TaskManager.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {

    TaskManager::Impl::Impl(size_t numWorkers) {
        assert(numWorkers > 0);
        mWorkersCount = numWorkers;

        for (uint32 i = 0; i < numWorkers; i++) {
            auto thread = ThreadManager::CreateThread(BERSERK_TEXT("WORKER-") + String::From(i), [&](){
                TaskExecutor();
            });

            mWorkers.Move(std::move(thread));
        }

        Provide(this);
    }

    TaskManager::Impl::~Impl() {
        mCancelToken.store(true, std::memory_order_release);

        for (auto& worker: mWorkers) {
            worker->Join();
        }

        Remove(this);
    }

    void TaskManager::Impl::SubmitTask(SharedPtr<Task> task) {
        assert(task);
        assert(task->GetStatus() != TaskStatus::InProgress);

        auto priority = task->GetPriority();
        auto index = (uint32) priority;
        auto maxPriority = (uint32) TaskPriority::Max;

        assert(index < maxPriority);

        mTaskQueues[index].Submit(std::move(task));
    }

    void TaskManager::Impl::TaskExecutor() {
        TaskContext context;

        while (!mCancelToken.load(std::memory_order_acquire)) {
            SharedPtr<Task> next;

            for (auto& queue: mTaskQueues) {
                next = queue.GetNextToExec();

                if (next)
                    break;
            }

            if (next) {
                auto& function = next->GetFunction();
                function(context);
                next->NotifyCompleted();
            }
            else
                ThreadManager::CurrentThreadSleep(100);
        }
    }

    uint32 TaskManager::Impl::GetWorkersCount() const {
        return mWorkersCount;
    }

}
