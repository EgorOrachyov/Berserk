//
// Created by Egor Orachyov on 13.05.2019.
//

#ifndef BERSERK_IPOINTER_H
#define BERSERK_IPOINTER_H

#include <Misc/Assert.h>
#include <Memory/IAllocator.h>

namespace Berserk
{

    #ifndef PROFILE_POINERS
        #define PROFILE_POINERS 1
    #endif

    /**
     * General purpose pointer class wrapper. Provides default
     * pointers logic. Prohibits creating instances of this class.
     *
     * @tparam T Class type to store pointer to that
     */
    template <class T>
    class IPointer
    {
    protected:

        /** Not allowed to create instances of this class */
        IPointer(T* source, IAllocator* allocator) : mSource(source), mAllocator(allocator) {};

        /** No custom logic, should be overridden by inheritors */
        ~IPointer() = default;

        /** Trivial set utility */
        void set(T* source, IAllocator* allocator) { mSource = source; mAllocator = allocator; };

        /** Trivial get utility */
        void get(T* &source, IAllocator* &allocator) { source = mSource; allocator = mAllocator; };

    public:

        /**
         * Returns reference to the stored resource.
         * Should be called with !isNull()
         * @warning assert fall is pointer is null
         * @return reference to the stored resource
         */
        T& get() const
        {
            FAIL(mSource, "Null pointer source could not be referenced");
            return *mSource;
        }

        /** @return raw resource pointer */
        T* operator->() const
        { return mSource; }

        /** @return */
        bool isNull() const
        { return (mSource == nullptr); }

        /** @return this == other */
        bool operator==(const IPointer<T>& other) const
        { return mSource == other.mSource; }

        /** @return this != other */
        bool operator!=(const IPointer<T>& other) const
        { return mSource != other.mSource; }

        /** @return this <= other */
        bool operator<=(const IPointer<T>& other) const
        { return mSource <= other.mSource; }

        /** @return this >= other */
        bool operator>=(const IPointer<T>& other) const
        { return mSource >= other.mSource; }

        /** @return this < other */
        bool operator<(const IPointer<T>& other) const
        { return mSource < other.mSource; }

        /** @return this > other */
        bool operator>(const IPointer<T>& other) const
        { return mSource > other.mSource; }

    protected:

        /** Raw pointer */
        T* mSource = nullptr;

        /** Allocator, which used to free memory for the resource */
        IAllocator* mAllocator = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_IPOINTER_H