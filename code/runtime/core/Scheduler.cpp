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

#include <core/Scheduler.hpp>
#include <core/io/Logger.hpp>

#include <algorithm>
#include <cassert>

BRK_NS_BEGIN

Scheduler::Handle Scheduler::Schedule(Scheduler::ScheduledFunc func, float interval, uint32 repeat, float delay, bool paused) {
    assert(interval > 0.0f);
    assert(delay >= 0.0f);

    Scheduled scheduled;
    scheduled.interval = interval;
    scheduled.delay = delay;
    scheduled.repeat = repeat;
    scheduled.paused = paused;
    scheduled.forever = repeat == REPEAT_FOREVER;
    scheduled.delayed = delay > 0.0f;
    scheduled.callback = std::move(func);

    auto hnd = GetNext();
    mPendingAdd.emplace_back(hnd, std::move(scheduled));

    return hnd;
}

Scheduler::Handle Scheduler::ScheduleOnce(Scheduler::ScheduledFunc func, float delay, bool paused) {
    assert(delay >= 0.0f);

    Scheduled scheduled;
    scheduled.delay = delay;
    scheduled.paused = paused;
    scheduled.delayed = delay > 0.0f;
    scheduled.callback = std::move(func);

    auto hnd = GetNext();
    mPendingAdd.emplace_back(hnd, std::move(scheduled));

    return hnd;
}

Scheduler::Handle Scheduler::ScheduleUpdate(Scheduler::ScheduledFunc func, uint32 repeat, float delay, bool paused) {
    assert(delay >= 0.0f);

    Scheduled scheduled;
    scheduled.delay = delay;
    scheduled.repeat = repeat;
    scheduled.paused = paused;
    scheduled.forever = repeat == REPEAT_FOREVER;
    scheduled.update = true;
    scheduled.delayed = delay > 0.0f;
    scheduled.callback = std::move(func);

    auto hnd = GetNext();
    mPendingAdd.emplace_back(hnd, std::move(scheduled));

    return hnd;
}

void Scheduler::ScheduleOnGameThread(Scheduler::PerformFunc func) {
    std::lock_guard<std::mutex> guard(mMutex);
    mToPerform.push_back(std::move(func));
}

void Scheduler::Cancel(Scheduler::Handle handle) {
    mPendingRemove.push_back(handle);
}

void Scheduler::Pause(Scheduler::Handle handle) {
    mPendingPause.emplace_back(handle, true);
}

void Scheduler::Resume(Scheduler::Handle handle) {
    mPendingPause.emplace_back(handle, false);
}

void Scheduler::Update(float dt) {
    // Add pending add scheduled functions
    for (auto &entry : mPendingAdd)
        mScheduled.emplace(entry.first, std::move(entry.second));
    mPendingAdd.clear();

    // Update state
    for (const auto &entry : mPendingPause) {
        auto query = mScheduled.find(entry.first);

        if (query == mScheduled.end()) {
            BRK_ERROR("No such scheduled function with handle=" << entry.first);
            continue;
        }

        query->second.paused = entry.second;
    }
    mPendingPause.clear();

    // Remove pending remove
    for (auto entry : mPendingRemove)
        mScheduled.erase(entry);
    mPendingRemove.clear();

    // Scale time
    dt = mTimeScale == 1.0f ? dt : mTimeScale * dt;

    // Process active scheduled functions
    for (auto &entry : mScheduled) {
        auto &scheduled = entry.second;

        if (scheduled.paused)
            continue;

        scheduled.elapsed += dt;

        if (scheduled.delayed) {
            if (scheduled.elapsed < scheduled.delay)
                continue;

            scheduled.executed += 1;
            scheduled.elapsed -= scheduled.delay;
            scheduled.delayed = false;
            scheduled.callback(scheduled.delay);

            if (Exhausted(scheduled)) {
                mPendingRemove.push_back(entry.first);
                continue;
            }
        }

        bool notFinished = true;
        float interval = scheduled.update ? scheduled.elapsed : scheduled.interval;
        while (scheduled.elapsed >= interval && notFinished && scheduled.elapsed > 0.0f) {
            scheduled.executed += 1;
            scheduled.elapsed -= interval;
            scheduled.callback(interval);

            if (Exhausted(scheduled)) {
                mPendingRemove.push_back(entry.first);
                notFinished = false;
            }
        }
    }

    // Perform on game thread
    {
        std::lock_guard<std::mutex> guard(mMutex);
        std::swap(mToPerform, mToPerformExec);
    }
    std::for_each(mToPerformExec.begin(), mToPerformExec.end(), [](PerformFunc &func) { func(); });
    mToPerformExec.clear();
}

bool Scheduler::Exhausted(struct Scheduled &scheduled) const {
    return !scheduled.forever && scheduled.executed > scheduled.repeat;
}

Scheduler::Handle Scheduler::GetNext() {
    return mNextHandle++;
}

BRK_NS_END
