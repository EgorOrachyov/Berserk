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

#ifndef BERSERK_SCHEDULER_HPP
#define BERSERK_SCHEDULER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Scheduler
 * @brief Scheduler is responsible for triggering the scheduled callbacks
 *
 * Use this class to:
 *  - schedule on game thread arbitrary user callback (thead-safe)
 *  - schedule functions to execute once
 *  - schedule timer functions
 *  - schedule repeated more then once function with fixed rate
 *
 *  Scheduler provides handlers for scheduler functions,
 *  so parameters and execution can be tweaked after scheduling.
 */
class BRK_API Scheduler final {
public:
    /** Function type to schedule */
    using ScheduledFunc = std::function<void(float)>;
    /** Function type to perform once on game thread */
    using PerformFunc = std::function<void()>;
    /** Handle to identify scheduled function entry */
    using Handle = size_t;
    /** Auxiliary constant to say, that function must be scheduled forever */
    static const uint32 REPEAT_FOREVER = 0x7fffffff;

    /**
     * @brief Schedule function for execution on game thread.
     *
     * The scheduled function will be called every `interval` seconds.
     * If `paused` is true, then it won't be called until it is resumed.
     * Param `repeat` lets the action be executed `repeat` + 1 times in total.
     * Param `delay` is how much to wait before scheduling first time.
     *
     * @note Must be called from game thread
     * @note Use `REPEAT_FOREVER` to schedule function to repeat forever.
     *
     * @param func Function to schedule
     * @param interval Interval in seconds to schedule; must be greater that 0
     * @param repeat Repeats count to schedule; pass 1 to schedule exactly once
     * @param delay Delay in seconds before first schedule
     * @param paused Pass true if paused; not scheduled until paused is set to false
     *
     * @return Handle of scheduled function
     */
    Handle Schedule(ScheduledFunc func, float interval, uint32 repeat = 0, float delay = 0.0f, bool paused = false);

    /**
     * @brief Schedule function for execution on game thread.
     *
     * The scheduled function will be called once.
     * If `paused` is true, then it won't be called until it is resumed.
     * Param `delay` is how much to wait before scheduling first time.
     *
     * @note Must be called from game thread
     *
     * @param func Function to schedule
     * @param delay Delay in seconds before first schedule
     * @param paused Pass true if paused; not scheduled until paused is set to false
     *
     * @return Handle of scheduled function
     */
    Handle ScheduleOnce(ScheduledFunc func, float delay, bool paused = false);

    /**
     * @brief Schedule function for execution on game thread.
     *
     * The scheduled function will be called every frame.
     * If `paused` is true, then it won't be called until it is resumed.
     * Param `repeat` lets the action be executed `repeat` + 1 times in total.
     * Param `delay` is how much to wait before scheduling first time.
     *
     * @note Must be called from game thread
     * @note Use `REPEAT_FOREVER` to schedule function to repeat forever.
     *
     * @param func Function to schedule
     * @param repeat Repeats count to schedule; pass 1 to schedule exactly once
     * @param delay Delay in seconds before first schedule
     * @param paused Pass true if paused; not scheduled until paused is set to false
     *
     * @return Handle of scheduled function
     */
    Handle ScheduleUpdate(ScheduledFunc func, uint32 repeat = 0, float delay = 0.0f, bool paused = false);

    /**
     * Schedule function to perform exactly on the game thread.
     * Use this function to safely notify user by callback after async operation completions.
     *
     * @note Thread-safe
     * @param func Function to schedule
     */
    void ScheduleOnGameThread(PerformFunc func);

    /**
     * Unschedule function associated with specified handle
     * @param handle Handle of scheduled function
     */
    void Cancel(Handle handle);

    /**
     * Pause scheduling of function associated with specified handle
     * @param handle Handle of scheduled function
     */
    void Pause(Handle handle);

    /**
     * Resume scheduling of function associated with specified handle
     * @param handle Handle of scheduled function
     */
    void Resume(Handle handle);

private:
    friend class Engine;

    /** Scheduled for execution entry */
    struct Scheduled {
        float elapsed = 0.0f;
        float interval = 0.0f;
        float delay = 0.0f;
        uint32 repeat = 0;
        uint32 executed = 0;
        bool paused = false;
        bool forever = false;
        bool update = false;
        bool delayed = false;
        ScheduledFunc callback;
    };

    void Update(float dt);
    bool Exhausted(Scheduled &scheduled) const;
    Handle GetNext();

private:
    /** New id (generate unique in sequence)*/
    Handle mNextHandle = 0;

    /** Internal self scheduler time scale */
    float mTimeScale = 1.0f;

    /** Scheduled entries */
    std::unordered_map<Handle, Scheduled> mScheduled;

    /** Entries to remove (accumulate to add at once) */
    std::vector<Handle> mPendingRemove;
    /** Entries to add (accumulate to add at once) */
    std::vector<std::pair<Handle, Scheduled>> mPendingAdd;

    /** Game thread pending execute functions */
    std::vector<PerformFunc> mToPerform;
    std::vector<PerformFunc> mToPerformExec;
    mutable std::mutex mMutex;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_SCHEDULER_HPP
