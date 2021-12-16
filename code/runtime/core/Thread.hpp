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

#ifndef BERSERK_THREAD_HPP
#define BERSERK_THREAD_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include <ttas_spin_mutex.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Thread
 * @brief Represents thread wrapper used to enqueue commands to execute
 *
 * @tparam TArgs Args to call callbacks
 */
class Thread final {
public:
    /** @brief Flag to enqueue callback */
    enum class Flag {
        /** @brief Execute before thread `main update` */
        Before,
        /** @brief Execute after thread `main update` */
        After
    };

    BRK_API Thread() = default;
    BRK_API ~Thread();

    /** @brief Thread callback function type */
    using Callable = std::function<void()>;

    /** Enqueue command to execute on thread */
    BRK_API void Enqueue(Callable callable, Flag flag);

    /** Enqueue command to execute on thread before `update` */
    BRK_API void EnqueueBefore(Callable callable) { Enqueue(std::move(callable), Flag::Before); }

    /** Enqueue command to execute on thread after `update` */
    BRK_API void EnqueueAfter(Callable callable) { Enqueue(std::move(callable), Flag::After); }

    /** @note Internal: must be called on thread */
    BRK_API void Update();

    /** @note Internal: must be called on thread */
    BRK_API void ExecuteBefore();

    /** @note Internal: must be called on thread */
    BRK_API void ExecuteAfter();

    /** @return True if currently on this thread */
    BRK_API bool OnThread() const;

private:
    /** For queueing */
    std::vector<Callable> mQueueBefore;
    std::vector<Callable> mQueueAfter;

    /** For execution */
    std::vector<Callable> mExecuteBefore;
    std::vector<Callable> mExecuteAfter;

    /** Id for checks before enqueuing */
    std::thread::id mThreadId = std::this_thread::get_id();

    mutable yamc::spin_ttas::mutex mMutex;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_THREAD_HPP
