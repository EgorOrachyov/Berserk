//
// Created by Egor Orachyov on 2019-08-28.
//

#include <Threading/Async.h>
#include <Threading/Thread.h>

namespace Berserk
{

    AsyncData::AsyncData()
    {
        completed.store(false, std::memory_order_acquire);
    }

    Async::Async(Berserk::TSharedPtr<Berserk::AsyncData> data)
         : mData(std::move(data))
    {

    }

    bool Async::completed() const
    {
        return mData->completed.load(std::memory_order_acquire);
    }

    bool Async::isPresent() const
    {
        return mData.isPresent();
    }

    void Async::blockUntilCompleted() const
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

    void Async::_complete()
    {
        mData->completed.store(true, std::memory_order_release);
    }

} // namespace Berserk