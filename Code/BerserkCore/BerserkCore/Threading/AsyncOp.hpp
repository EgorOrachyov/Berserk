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

#ifndef BERSERK_ASYNCOP_HPP
#define BERSERK_ASYNCOP_HPP

#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

namespace Berserk {

    template<typename Result = void*>
    struct AsyncOpData {
    public:

        /** Signal finished, no result assigned */
        void Finish() {
            assert(!finished.load(std::memory_order_acquire));
            finished.store(std::memory_order_release);
        }

        /** Finish with specified result */
        void Finish(SharedPtr<Result> r) {
            assert(!finished.load(std::memory_order_acquire));
            result = std::move(r);
            finished.store(std::memory_order_release);
        }

    private:
        template<typename R>
        friend class AsyncOp;

        AtomicBool finished{false};
        SharedPtr<Result> result;
    };

    /**
     * Async handler, used to signal when async task is finished.
     * @tparam Result Type of the result, returned after operation
     */
    template<typename Result = void*>
    class AsyncOp {
    public:
        explicit AsyncOp(SharedPtr<AsyncOpData<Result>> data) : mSync(data) { assert(mSync); }
        AsyncOp() = default;
        AsyncOp(const AsyncOp&) = default;
        AsyncOp(AsyncOp&&) noexcept = default;
        ~AsyncOp() = default;

        AsyncOp& operator=(const AsyncOp&) = default;
        AsyncOp& operator=(AsyncOp&&) noexcept = default;

        /** Block current thread execution until is finished */
        void BlockUntilCompleted(size_t sleep = 1000) const {
            while (!IsCompleted()) {
                ThreadManager::CurrentThreadSleep(sleep);
            }
        }

        /** Releases internal data reference */
        void Reset() {
            mSync = nullptr;
        }

        /** @return True if task completed */
        bool IsCompleted() const {
            assert(mSync);
            return mSync->finished.load(std::memory_order_acquire);
        }

        /** @return True if result is available and not null */
        bool HasResult() const {
            return IsCompleted() && mSync->result;
        }

        /** @return Result pointer */
        const SharedPtr<Result> &GetResult() {
            assert(IsCompleted());
            return mSync->result;
        }

    private:
        SharedPtr<AsyncOpData<Result>> mSync;
    };

}

#endif //BERSERK_ASYNCOP_HPP