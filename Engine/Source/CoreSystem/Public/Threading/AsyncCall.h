//
// Created by Egor Orachyov on 2019-08-28.
//

#ifndef BERSERK_ASYNCCALL_H
#define BERSERK_ASYNCCALL_H

#include <Threading/AtomicTypes.h>
#include <Resource/TSharedPtr.h>
#include <Misc/Allocatable.h>

namespace Berserk
{

    /** Operation bool flag, marks operation is done */
    struct CORE_EXPORT AsyncCallData : public Allocatable
    {
        AsyncCallData();

        AtomicBool completed;
    };

    /** Allows to get state of async operation */
    class CORE_EXPORT AsyncCall
    {
    public:

        /** Construct handler from operation data */
        explicit AsyncCall(TSharedPtr<AsyncCallData> data);

        /** @return True, if operation completed */
        bool completed() const;

        /** Blocks current thread until operation completed */
        void blockUntilCompleted() const;

        /** Marks operation as completed [for internal usage] */
        void _complete();

    private:

        TSharedPtr<AsyncCallData> mData;

    };

} // namespace Berserk

#endif //BERSERK_ASYNCCALL_H