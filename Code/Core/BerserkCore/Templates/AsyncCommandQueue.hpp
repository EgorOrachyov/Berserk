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

#ifndef BERSERK_ASYNCCOMMANDQUEUE_HPP
#define BERSERK_ASYNCCOMMANDQUEUE_HPP

#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/Queue.hpp>
#include <BerserkCore/Templates/RefCounted.hpp>
#include <BerserkCore/Templates/MessageQueue.hpp>
#include <BerserkCore/Templates/CommandQueue.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    /**
     * Async command queue, which allows to submit executable commands for later execution.
     * @tparam TArgs Type of the args used to execute commands.
     */
    template<typename ... TArgs>
    class AsyncCommandQueue {
    public:

        AsyncCommandQueue(AsyncCommandQueue&& other) noexcept {
            mProvider = std::move(other.mProvider);
            mQueue = other.mQueue;

            other.mQueue = nullptr;
        }

        ~AsyncCommandQueue() {
            if (mProvider) {
                mProvider->Deallocate(mQueue);
                mQueue = nullptr;
                mProvider = nullptr;
            }
        };

        /** Submit new command to queue */
        template<typename Callable>
        void Submit(Callable&& callable) {
            assert(mQueue);
            mQueue->Enqueue(std::forward<Callable>(callable));
        }

        /** Commit commands to make them visible for consumer */
        void Commit() {
            assert(mProvider);
            mQueue = mProvider->Submit(mQueue);
        }

    private:
        using QueueType = CommandQueue<TArgs...>;

        template<typename ... TTArgs>
        friend class AsyncCommandQueueConsumer;

        class Provider: public RefCountedThreadSafe {
        public:
            ~Provider() override = default;
            virtual QueueType* Allocate() = 0;
            virtual QueueType* Submit(QueueType* queue) = 0;
            virtual void Deallocate(QueueType* queue) = 0;
        };

        explicit AsyncCommandQueue(RefCounted<Provider> provider)
            : mProvider(std::move(provider)) {
            assert(mProvider);
            mQueue = mProvider->Allocate();
        }

        CommandQueue<TArgs...>* mQueue = nullptr;
        RefCounted<Provider> mProvider;
    };

    /**
     * Consumer, which consumes command queues from producers.
     * Use AsyncCommandQueue as fronted for this consumer communication.
     *
     * @tparam TArgs TArgs Type of the args used to execute commands.
     */
    template<typename ... TArgs>
    class AsyncCommandQueueConsumer {
    public:
        struct Stats {
            size_t totalQueues = 0;
            size_t freeQueues = 0;
            size_t submitted = 0;
            size_t pendingExecution = 0;
        };

        using QueueType = CommandQueue<TArgs...>;
        using AsyncCommandQueueType = AsyncCommandQueue<TArgs...>;
        static const size_t BUFFER_SIZE = QueueType::BUFFER_SIZE;
        static const size_t BUFFER_ALIGNMENT = QueueType::BUFFER_ALIGNMENT;
        static const size_t MAX_QUEUES_IN_FLY = 2;

        explicit AsyncCommandQueueConsumer(size_t bufferSize = BUFFER_SIZE,
                                           size_t bufferAlignment = BUFFER_ALIGNMENT,
                                           size_t queuesInFly = MAX_QUEUES_IN_FLY) {
            auto object = Memory::Make<ProviderImpl>(bufferSize, bufferAlignment, queuesInFly);
            mProvider = std::move(RefCounted<ProviderImpl>(object, RefCountedBoxing::AddRefs));
        }

        AsyncCommandQueueType CreateQueue() {
            return AsyncCommandQueueType((RefCounted<typename AsyncCommandQueueType::Provider>) mProvider);
        }

        size_t ExecutePending(TArgs&& ... args) {
            return mProvider->ExecutePending(std::forward<TArgs>(args)...);
        }

        Stats GetStats() const {
            return mProvider->GetStats();
        }

    private:

        class ProviderImpl final: public  AsyncCommandQueueType::Provider {
        public:
            ProviderImpl(size_t bufferSize,
                         size_t bufferAlignment,
                         size_t queuesInFly) {
                assert(queuesInFly <= MAX_QUEUES_IN_FLY);
                assert(queuesInFly >= 2);

                mBufferSize = bufferSize;
                mAlignment = bufferAlignment;
                mQueuesInFly = queuesInFly;
            }

            ~ProviderImpl() override {
                for (auto queue: mFreeQueues) {
                    Memory::Release(queue);
                    mTotalQueues--;
                }

                for (auto& pending: mPending) {
                    QueueType* queue;

                    while (pending.Pop(queue)) {
                        Memory::Release(queue);
                        mTotalQueues--;
                    }
                }

                assert(mTotalQueues == 0);
            }

            QueueType *Allocate() override {
                Guard<SpinMutex> guard(mMutex);
                return GetOrAllocateQueue();
            }

            QueueType *Submit(QueueType *queue) override {
                assert(queue);

                Guard<SpinMutex> guard(mMutex);

                assert(mPending[mCurrentSubmit].GetSize() < mQueueOverflowLimit);
                mPending[mCurrentSubmit].Push(queue);

                return GetOrAllocateQueue();
            }

            void Deallocate(QueueType *queue) override {
                assert(queue);

                Guard<SpinMutex> guard(mMutex);
                queue->Clear();
                mFreeQueues.Add(queue);
            }

            size_t ExecutePending(TArgs&& ... args) {
                // Pop one by one and execute
                size_t executed = 0;
                QueueType* queue;

                while (PopNextForExecution(queue)) {
                    queue->Execute(std::forward<TArgs>(args)...);
                    executed++;
                    Deallocate(queue);
                }

                // Swap submit and execute queues
                {
                    Guard<SpinMutex> guard(mMutex);
                    mCurrentExec = (mCurrentExec + 1) % mQueuesInFly;
                    mCurrentSubmit = (mCurrentSubmit + 1) % mQueuesInFly;
                }

                return executed;
            }

            Stats GetStats() const {
                Guard<SpinMutex> guard(mMutex);

                Stats stats{};
                stats.totalQueues = mTotalQueues;
                stats.freeQueues = mFreeQueues.GetSize();
                stats.submitted = mPending[mCurrentSubmit].GetSize();
                stats.pendingExecution = mPending[mCurrentExec].GetSize();

                return stats;
            }

        protected:

            void OnReleased() const override {
                Memory::Release(this);
            }

            QueueType* GetOrAllocateQueue() {
                if (mFreeQueues.IsNotEmpty())
                    return mFreeQueues.PopLast();

                mTotalQueues++;
                return Memory::Make<QueueType>();
            }

            bool PopNextForExecution(QueueType* &queue) {
                Guard<SpinMutex> guard(mMutex);
                return mPending[mCurrentExec].Pop(queue);
            }

        private:
            Array<QueueType*> mFreeQueues;
            Queue<QueueType*> mPending[MAX_QUEUES_IN_FLY];

            uint32 mCurrentExec = 0;
            uint32 mCurrentSubmit = 1;
            uint32 mQueueOverflowLimit = 1000;
            int32 mTotalQueues = 0;

            size_t mBufferSize;
            size_t mAlignment;
            size_t mQueuesInFly;

            mutable SpinMutex mMutex;
        };

        RefCounted<ProviderImpl> mProvider;
    };

}

#endif //BERSERK_ASYNCCOMMANDQUEUE_HPP
