//
// Created by Egor Orachyov on 13.05.2019.
//

#ifndef BERSERK_UNIQUEPOINTER_H
#define BERSERK_UNIQUEPOINTER_H

#include <Resource/IPointer.h>

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
     * @tparam T Class type to store pointer to that
     */
    template <class T>
    class UniquePointer : public IPointer<T>
    {
    public:

        /**
         * Null unique pointer. Nothing to delete when
         * its destructor method is called
         */
        UniquePointer() : IPointer<T>(nullptr, nullptr) {};

        /**
         * Creates unique pointer instance to the source, which will be
         * deleted, when this unique pointer destructor method is called
         * @param source
         * @param allocator
         */
        UniquePointer(T* source, IAllocator* allocator) : IPointer<T>(source, allocator) {};

        /**
         * Deletes object via default destructor/delete fuction,
         * whether the allocator presented or not
         */
        ~UniquePointer()
        {
            T* source;
            IAllocator* allocator;

            this->get(source, allocator);

            if (source)
            {
#ifdef PROFILE_POINTERS
                PUSH("UniquePointer: delete %p [allocator: %p]", source, allocator);
#endif

                if (allocator)
                {
                    source->~T();
                    allocator->free(source);
                }
                else
                {
                    delete (source);
                }

                this->set(nullptr, nullptr);
            }
        }

        /** @warning You cannot copy unique pointers */
        const UniquePointer<T> &operator = (const UniquePointer<T> &other) = delete;

        /**
         * Frees internal data, whether it is not null.
         * Assigns to THIS pointer the contents of the OTHER pointer.
         * Invalidates data of the other pointer - set it to null
         * @param other Source to assign
         */
        void assign(UniquePointer<T> &other)
        {
            this->~UniquePointer();
            this->set(other.mSource, other.mAllocator);
            other.set(nullptr, nullptr);
        }

    };

} // namespace Berserk

#endif //BERSERK_UNIQUEPOINTER_H