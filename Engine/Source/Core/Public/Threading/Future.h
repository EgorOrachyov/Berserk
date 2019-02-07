//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_FUTURE_H
#define BERSERK_FUTURE_H

#include "IRunnable.h"

namespace Berserk
{

    /**
     * Allows to know whether submitted runnable was finished and get
     * pointer to this runnable
     */
    class Future
    {
    public:

        Future() : mDone(false), mResult(0), mRunnable(nullptr) {}

        ~Future() = default;

        /** @return True whether the runnable is done */
        bool done() const { return mDone; }

        /** @return Exit code of runnable function run */
        int32 result() const { return mResult; }

        /** @return Pointer to submitted runnable object */
        IRunnable* runnable() const { return mRunnable; }

    private:

        friend class ThreadPool;

        bool       mDone;
        int32      mResult;
        IRunnable* mRunnable;

    };

} // namespace Berserk

#endif //BERSERK_FUTURE_H