//
// Created by Egor Orachyov on 2019-08-28.
//

#include <IO/OutputDevice.h>
#include "Threading/CommandQueue.h"

namespace Berserk
{

    CommandQueue::CommandQueue(Berserk::uint32 defaultSize,
            Berserk::IAllocator &miscAlloc,
            Berserk::IAllocator &queueAlloc)
            : mQueueAlloc(queueAlloc),
              mMiscAlloc(miscAlloc),
              mQueueDefaultSize(defaultSize),
              mSubmitedQueues(mQueueAlloc),
              mEmptyQueues(mQueueAlloc)
    {
        mCurrentQueue = mem_new_shared_alloc<CommandQueueBuffer>(
                mQueueAlloc, mQueueDefaultSize, mQueueAlloc);
    }

    void CommandQueue::queue(Berserk::StdFunction function)
    {
        mCurrentQueue->emplace(function);
    }

    void CommandQueue::queue(Berserk::EngineFunction function)
    {
        mCurrentQueue->emplace(function);
    }

    Async CommandQueue::queueReturn(Berserk::StdFunction function)
    {
        auto data = mem_new_shared_alloc<AsyncData>(mMiscAlloc);
        Async async(data);

        mCurrentQueue->emplace(function, async);
        return async;
    }

    Async CommandQueue::queueReturn(Berserk::EngineFunction function)
    {
        auto data = mem_new_shared_alloc<AsyncData>(mMiscAlloc);
        Async async(data);

        mCurrentQueue->emplace(function, async);
        return async;
    }

    void CommandQueue::flush()
    {
        CriticalSection section(mMutex);

        mSubmitedQueues.add(mCurrentQueue);

        if (mEmptyQueues.getSize() > 0)
        {
            OutputDevice::printf("Found empty queue\n");

            mCurrentQueue = mEmptyQueues.get(0);
            mEmptyQueues.remove(0);
        }
        else
        {
            OutputDevice::printf("Create new queue [cmd: %u][total: %u]\n",
                    sizeof(QueueCommand), sizeof(QueueCommand) * mQueueDefaultSize);

            mCurrentQueue = mem_new_shared_alloc<CommandQueueBuffer>(
                    mQueueAlloc, mQueueDefaultSize, mQueueAlloc);
        }
    }

    TSharedPtr<CommandQueueBuffer> CommandQueue::getSubmitQueue()
    {
        CriticalSection section(mMutex);

        if (mSubmitedQueues.getSize() > 0)
        {
            auto queue = mSubmitedQueues.get(0);
            mSubmitedQueues.remove(0);

            return queue;
        }

        return TSharedPtr<CommandQueueBuffer>();
    }

    void CommandQueue::addEmptyQueue(Berserk::TSharedPtr<Berserk::CommandQueueBuffer> queue)
    {
        CriticalSection section(mMutex);

        mEmptyQueues.emplace(std::move(queue));
    }

} // namespace Berserk