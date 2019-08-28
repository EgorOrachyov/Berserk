//
// Created by Egor Orachyov on 2019-08-28.
//

#include <Threading/AsyncCall.h>
#include <Threading/Thread.h>

namespace Berserk
{

    AsyncCallData::AsyncCallData()
    {
        completed.store(false, std::memory_order_acquire);
    }

    AsyncCall::AsyncCall(Berserk::TSharedPtr<Berserk::AsyncCallData> data)
         : mData(std::move(data))
    {

    }

    bool AsyncCall::completed() const
    {
        return mData->completed.load(std::memory_order_acquire);
    }

    bool AsyncCall::isPresent() const
    {
        return mData.isPresent();
    }

    void AsyncCall::blockUntilCompleted() const
    {
        bool completed;

        do
        {
            completed = mData->completed.load(std::memory_order_release);

            if (!completed)
            {
                Thread::yield();
            }
        }

        while (!completed);
    }

    void AsyncCall::_complete()
    {
        mData->completed.store(true, std::memory_order_release);
    }

} // namespace Berserk