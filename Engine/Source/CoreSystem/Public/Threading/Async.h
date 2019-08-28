//
// Created by Egor Orachyov on 2019-08-28.
//

#ifndef BERSERK_ASYNC_H
#define BERSERK_ASYNC_H

#include <Threading/AtomicTypes.h>
#include <Resource/TSharedPtr.h>
#include <Misc/Allocatable.h>

namespace Berserk
{

    /** Operation bool flag, marks operation is done */
    struct CORE_EXPORT AsyncData : public Allocatable
    {
        AsyncData();

        AtomicBool completed;
    };

    /** Allows to get state of async operation */
    class CORE_EXPORT Async
    {
    public:

        Async() = default;

        /** Construct handler from operation data */
        explicit Async(TSharedPtr<AsyncData> data);

        /** @return True, if operation completed */
        bool completed() const;

        /** @return True, whether thi is not empty call */
        bool isPresent() const;

        /** Blocks current thread until operation completed */
        void blockUntilCompleted() const;

        /** Marks operation as completed [for internal usage] */
        void _complete();

    private:

        TSharedPtr<AsyncData> mData;

    };

} // namespace Berserk

#endif //BERSERK_ASYNC_H