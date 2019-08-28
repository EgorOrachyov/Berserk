//
// Created by Egor Orachyov on 2019-08-28.
//

#include <Threading/AsyncOperation.h>
#include <Threading/Thread.h>

namespace Berserk
{

    AsyncOperationData::AsyncOperationData()
    {
        completed.store(false, std::memory_order_acquire);
    }

    AsyncOperation::AsyncOperation(Berserk::TSharedPtr<Berserk::AsyncOperationData> data)
         : mData(std::move(data))
    {

    }

    bool AsyncOperation::completed() const
    {
        return mData->completed.load(std::memory_order_acquire);
    }

    void AsyncOperation::blockUntilCompleted() const
    {
        bool completed;

        do
        {
            completed = mData->completed.load(std::memory_order_release);

            if (!completed)
                Thread::yield();
        }

        while (!completed);
    }

    void AsyncOperation::_complete()
    {
        mData->completed.store(true, std::memory_order_release);
    }

} // namespace Berserk