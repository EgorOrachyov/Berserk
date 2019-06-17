//
// Created by Egor Orachyov on 13.05.2019.
//

#ifndef BERSERK_TPTR_H
#define BERSERK_TPTR_H

#include <Misc/Assert.h>
#include <Memory/IAllocator.h>
#include <Exception/CoreException.h>

namespace Berserk
{

    #ifndef PROFILE_POINTERS
        #define PROFILE_POINTERS 1
    #endif

    /**
     * General purpose pointer class wrapper. Provides default
     * pointers logic. Prohibits creating instances of this class.
     *
     * @tparam T Class type to store pointer to that
     */
    template <class T>
    class TPtr
    {
    protected:

        /** Not allowed to create instances of this class */
        TPtr(T* source, IAllocator* allocator)
                : mSource(source), mAllocator(allocator) {};

        /** Trivial set utility */
        void set(T* source, IAllocator* allocator)
        { mSource = source; mAllocator = allocator; };

        /** Trivial get utility */
        void get(T* &source, IAllocator* &allocator)
        { source = mSource; allocator = mAllocator; };

    public:

        /**
         * Returns reference to the stored resource.
         * Should be called with !isNull()
         * @warning assert fall is pointer is null
         * @return reference to the stored resource
         */
        T& get() const
        {
            if (mSource == nullptr) throw CoreException("IPointer: null pointer source");
            return *mSource;
        }

        /** @return raw resource pointer */
        T* operator->() const
        { return mSource; }

        /** @return */
        bool isNull() const
        { return (mSource == nullptr); }

        /** @return this == other */
        bool operator==(const TPtr<T>& other) const
        { return mSource == other.mSource; }

        /** @return this != other */
        bool operator!=(const TPtr<T>& other) const
        { return mSource != other.mSource; }

        /** @return this <= other */
        bool operator<=(const TPtr<T>& other) const
        { return mSource <= other.mSource; }

        /** @return this >= other */
        bool operator>=(const TPtr<T>& other) const
        { return mSource >= other.mSource; }

        /** @return this < other */
        bool operator<(const TPtr<T>& other) const
        { return mSource < other.mSource; }

        /** @return this > other */
        bool operator>(const TPtr<T>& other) const
        { return mSource > other.mSource; }

    protected:

        /** Raw pointer */
        T* mSource = nullptr;

        /** Allocator, which used to free memory for the resource */
        IAllocator* mAllocator = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_TPTR_H