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

#ifndef BERSERK_TASK_HPP
#define BERSERK_TASK_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Strings/StringName.hpp>
#include <BerserkCore/Threading/TaskContext.hpp>

namespace Berserk {

    /** Priority to execute task in task manager */
    enum class TaskPriority {
        /** Run as soon as no task with higher priority in queue */
        Low = 0,
        /** Run as soon as no tasks with high priority */
        Medium = 1,
        /** Run as soon as added in the queue */
        High = 2,
        /** Total count */
        Max = 3
    };

    /** Task lifetime status */
    enum class TaskStatus {
        /** Not active or not pending execution */
        Inactive = 0,
        /** Pending to be executed */
        InProgress = 1,
        /** Successfully finished execution */
        Completed = 2,
        /** Execution canceled (removed from queue) */
        Canceled = 3
    };

    /** Basic task to run in task manager */
    class Task {
    public:
        using Callable = Function<void(TaskContext&)>;

        /**
         * Creates inactive task for future run.
         *
         * @param name Task name for debugging
         * @param callable Function to execution when task is running
         * @param priority Task priority
         */
        Task(StringName name, TaskPriority priority, Callable callable);
        Task(const Task&) = delete;
        Task(Task&&) noexcept = delete;

        virtual ~Task() = default;

        /** @return Task debug name */
        const StringName& GetName() const { return mName; }

        /** @return Task function to executed */
        const Callable& GetFunction() const { return mFunction; }

        /** @return Task priority for execution */
        TaskPriority GetPriority() const { return mPriority; }

        /** @return Task status */
        TaskStatus GetStatus() const;

        /** @return True if the task has completed. */
        bool IsCompleted() const;

        /**	@return True if the task has been canceled. */
        bool IsCanceled() const;

        /** @return True if the task has started or completed execution. */
        bool IsStarted() const;

        /** Blocks thread execution until task is completed */
        void BlockUntilCompleted(uint64 sleep = 1000) const;

    private:
        friend class TaskManager;
        friend class TaskQueue;

        virtual bool CanStart(bool& shouldRemoveFromQueue);
        virtual void NotifyQueued();
        virtual void NotifyCompleted();
        virtual void NotifyCanceled();

        Callable mFunction;
        StringName mName;
        TaskPriority mPriority;
        AtomicUint32 mWaiting{0};
        AtomicUint32 mStatus{0};
    };

}

#endif //BERSERK_TASK_HPP