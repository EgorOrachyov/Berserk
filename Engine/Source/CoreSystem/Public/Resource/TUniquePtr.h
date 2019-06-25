//
// Created by Egor Orachyov on 13.05.2019.
//

#ifndef BERSERK_TUNIQUEPTR_H
#define BERSERK_TUNIQUEPTR_H

#include <Memory/IAllocator.h>

namespace Berserk
{

    /**
     * Unique pointer class, which allows only one pointer to has
     * the ownership of the resource pointer of class T. Does not allow
     * to copy unique pointers. Provides assign method to copy pointer with
     * the following invalidation of it.
     *
     * Usage case:
     * UniquePointer<Type> p(new (allocator.alloc) Type(), &allocator);
     * ... some actions with p
     *
     * @warning Single-threaded
     * @tparam T Class type to store pointer to that
     */
    template <class T>
    class ENGINE_API TUniquePtr
    {
    public:

        GENERATE_NEW_DELETE(TUniquePtr);

        /**
         * Null unique pointer. Nothing to delete when
         * its destructor method is called
         */
        TUniquePtr()
        {}

        /**
         * Creates unique pointer instance to the source, which will be
         * deleted, when this unique pointer destructor method is called
         * @param source
         * @param allocator
         */
        TUniquePtr(T* source, IAllocator* allocator)
                : mSource(source), mAllocator(allocator)
        {}

        /**
         * Allows to initialize resource inside the unique ptr
         * @tparam TArgs Arguments type for T source constructor
         * @param allocator Allocator to allocate a memory
         * @param args Arguments for constructor
         */
        template <typename ... TArgs>
        explicit TUniquePtr(IAllocator* allocator, const TArgs& ... args)
        {
            mSource = new (allocator->allocate(sizeof(T))) T(args ...);
            mAllocator = allocator;
        }

        /**
         * Creates unique pointer instance to the source, which will be
         * deleted, when this unique pointer destructor method is called.
         *
         * Uses null allocator as default
         * @note default delete will be called for source
         *
         * @param source
         */
        explicit TUniquePtr(T* source)
        {
            mSource = source;
        }

        /**
         * Creates new unique handler from other 'move' pointer.
         * Moves unique resource handler from move ptr to this handler
         * @note move Pointer becomes invalid (or null)
         * @param move Pointer which handles resource
         */
        TUniquePtr(TUniquePtr& move)
        {
            mSource = move.mSource;
            mAllocator = move.mAllocator;

            move.mSource = nullptr;
            move.mAllocator = nullptr;
        }

        /**
         * Creates new unique handler from other 'move' pointer.
         * Moves unique resource handler from move ptr to this handler
         * @note move Pointer becomes invalid (or null)
         * @param move Pointer which handles resource
         */
        TUniquePtr(TUniquePtr&& move)
        {
            mSource = move.mSource;
            mAllocator = move.mAllocator;

            move.mSource = nullptr;
            move.mAllocator = nullptr;
        }

        /**
         * Deletes object via default destructor/delete fuction,
         * whether the allocator presented or not
         */
        ~TUniquePtr()
        {
            if (mSource != nullptr)
            {
                if (mAllocator)
                {
                    mSource->~T();
                    mAllocator->free(mSource);
                }
                else
                {
                    delete mSource;
                }

                mSource = nullptr;
                mAllocator = nullptr;
            }
        }

        /**
         * Frees internal data, whether it is not null.
         * Assigns to THIS pointer the contents of the OTHER pointer.
         * Invalidates data of the other pointer - set it to null
         * @param other Source to assign
         */
        TUniquePtr<T> &operator=(TUniquePtr<T> &other)
        {
            assign(other);
            return *this;
        }

        /**
         * Returns reference to the stored resource.
         * Should be called with !isNull()
         * @warning assert fall is pointer is null
         * @return reference to the stored resource
         */
        T& get() const
        {
            assertion_dev(mSource != nullptr);
            return *mSource;
        }

        /** @return raw resource pointer */
        T* operator->() const
        { return mSource; }

        /** @return */
        bool isNull() const
        {
            return (mSource == nullptr);
        }

        /** @return this == other */
        bool operator==(const TUniquePtr& other) const
        {
            return mSource == other.mSource;
        }

        /** @return this != other */
        bool operator!=(const TUniquePtr& other) const
        {
            return mSource != other.mSource;
        }

        /** @return this <= other */
        bool operator<=(const TUniquePtr& other) const
        {
            return mSource <= other.mSource;
        }

        /** @return this >= other */
        bool operator>=(const TUniquePtr& other) const
        {
            return mSource >= other.mSource;
        }

        /** @return this < other */
        bool operator<(const TUniquePtr& other) const
        {
            return mSource < other.mSource;
        }

        /** @return this > other */
        bool operator>(const TUniquePtr& other) const
        {
            return mSource > other.mSource;
        }

        /**
         * Frees internal data, whether it is not null.
         * Assigns to THIS pointer the contents of the OTHER pointer.
         * Invalidates data of the other pointer - set it to null
         * @param other Source to assign
         */
        void assign(TUniquePtr<T> &other)
        {
            this->~TUniquePtr();

            mSource = other.mSource;
            mAllocator = other.mAllocator;

            other.mSource = nullptr;
            other.mAllocator = nullptr;
        }

    private:

        /** Raw pointer */
        T* mSource = nullptr;

        /** Allocator, which used to free memory for the resource */
        IAllocator* mAllocator = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_TUNIQUEPTR_H