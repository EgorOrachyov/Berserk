//
// Created by Egor Orachyov on 13.05.2019.
//

#ifndef BERSERK_TUNIQUEPTR_H
#define BERSERK_TUNIQUEPTR_H

#include <Resource/TPtr.h>

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
    class TUniquePtr : public TPtr<T>
    {
    public:

        /**
         * Null unique pointer. Nothing to delete when
         * its destructor method is called
         */
        TUniquePtr() : TPtr<T>(nullptr, nullptr) {};

        /**
         * Creates unique pointer instance to the source, which will be
         * deleted, when this unique pointer destructor method is called
         * @param source
         * @param allocator
         */
        TUniquePtr(T* source, IAllocator* allocator) : TPtr<T>(source, allocator) {};

        /**
         * Creates unique pointer instance to the source, which will be
         * deleted, when this unique pointer destructor method is called.
         *
         * Uses null allocator as default
         *
         * @param source
         */
        explicit TUniquePtr(T* source) : TPtr<T>(source, nullptr) {};

        /**
         * Creates new unique handler from other 'move' pointer.
         * Moves unique resource handler from move ptr to this handler
         * @note move Pointer becomes invalid (or null)
         * @param move Pointer which handles resource
         */
        TUniquePtr(TUniquePtr& move) : TPtr<T>(move.mSource, move.mAllocator)
        {
            move.set(nullptr, nullptr);
        }

        /**
         * Creates new unique handler from other 'move' pointer.
         * Moves unique resource handler from move ptr to this handler
         * @note move Pointer becomes invalid (or null)
         * @param move Pointer which handles resource
         */
        explicit TUniquePtr(TUniquePtr&& move) : TPtr<T>(move.mSource, move.mAllocator)
        {
            move.set(nullptr, nullptr);
        }

        /**
         * Deletes object via default destructor/delete fuction,
         * whether the allocator presented or not
         */
        ~TUniquePtr()
        {
            T* source;
            IAllocator* allocator;

            this->get(source, allocator);

            if (source)
            {
                if (allocator)
                {
                    source->~T();
                    allocator->free(source);
                }
                else
                {
                    source->~T();
                }

                this->set(nullptr, nullptr);
            }
        }

        /** @warning You cannot copy unique pointers */
        const TUniquePtr<T> &operator = (const TUniquePtr<T> &other) = delete;

        /**
         * Frees internal data, whether it is not null.
         * Assigns to THIS pointer the contents of the OTHER pointer.
         * Invalidates data of the other pointer - set it to null
         * @param other Source to assign
         */
        void assign(TUniquePtr<T> &other)
        {
            this->TUniquePtr();
            this->set(other.mSource, other.mAllocator);
            other.set(nullptr, nullptr);
        }

    };

} // namespace Berserk

#endif //BERSERK_TUNIQUEPTR_H