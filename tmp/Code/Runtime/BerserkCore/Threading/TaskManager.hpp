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

#ifndef BERSERK_TASKMANAGER_HPP
#define BERSERK_TASKMANAGER_HPP

#include <BerserkCore/Threading/Task.hpp>
#include <BerserkCore/Threading/TaskQueue.hpp>
#include <BerserkCore/Platform/Thread.hpp>
#include <BerserkCore/Templates/Singleton.hpp>

namespace Berserk {

    class TaskManager {
    public:

        /** Submit task for execution */
        static void SubmitTask(SharedPtr<Task> task) {
            Impl::Instance().SubmitTask(std::move(task));
        }

        /** Creates new task with specified properties and submits for execution */
        template<typename Callable>
        static SharedPtr<Task> SubmitTask(StringName name, TaskPriority priority, Callable&& callable) {
            auto task = SharedPtr<Task>::Make(std::move(name), priority, std::forward<Callable>(callable));
            SubmitTask(task);
            return task;
        }

        /** @return Number of worker threads in task manager */
        static uint32 GetWorkersCount() {
            return Impl::Instance().GetWorkersCount();
        }

    protected:

        class Impl: public Singleton<Impl> {
        public:
            explicit Impl(uint32 numWorkers);
            ~Impl();

            void SubmitTask(SharedPtr<Task> task);
            void TaskExecutor();
            uint32 GetWorkersCount() const;

        private:
            TaskQueue mTaskQueues[(uint32)TaskPriority::Max];
            Array<SharedPtr<Thread>> mWorkers;
            uint32 mWorkersCount;
            AtomicBool mCancelToken{false};
        };

    };

}

#endif //BERSERK_TASKMANAGER_HPP