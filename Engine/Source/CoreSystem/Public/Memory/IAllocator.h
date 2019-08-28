//
// Created by Egor Orachyov on 19.03.2019.
//

#ifndef BERSERK_IALLOCATOR_H
#define BERSERK_IALLOCATOR_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Misc/Alignment.h>

#include <memory>

namespace Berserk
{

    class MEMORY_API IAllocator
    {
    public:

        virtual ~IAllocator() = default;

        /**
         * Allocates memory by this allocator for object T
         * @tparam T Type of the object to allocate
         * @return Pointer to allocated memory for object of type T
         */
        template <typename T>
        T* engine_alloc()
        {
            return (T*) allocate(sizeof(T));
        }

        /**
         * Create new instance of type T and allocate by this allocator
         *
         * @note T must support engine new/delete allocation policy
         *
         * @tparam T Type of the object to create
         * @tparam TArgs Type of arguments for T constructor
         * @param args Actual arguments
         * @return Pointer to allocated and created instance of type T
         */
        template <typename T, typename ... TArgs>
        T* engine_new(TArgs &&... args)
        {
            return new (allocate(sizeof(T))) T(std::forward<TArgs>(args) ...);
        };

        /**
         * Create new instance of type T and allocate by this allocator
         * @note T must support engine new/delete allocation policy
         *
         * @tparam T Type of the object to create
         * @return Pointer to allocated and created instance of type T
         */
        template <typename T>
        T* engine_new_no_args()
        {
            return new (allocate(sizeof(T))) T();
        }

        /**
         * Destroy object of type T, created by engnie_new method
         * @tparam T Type of the object to destroy
         * @param object
         */
        template <typename T>
        void engine_destroy(T* object)
        {
            object->~T();
            free(object);
        }

        /** Allocates chosen size of continuous memory block */
        virtual void* allocate(uint32 size) { allocate(size, MEMORY_ALIGNMENT); };

        /** Allocates chosen size of continuous memory block with desired alignment */
        virtual void* allocate(uint32 size, uint32 alignment) = 0;

        /** Free memory block */
        virtual void free(void *pointer) = 0;

        /** @return Total number of memoryFree calls in the engine [in bytes] */
        virtual uint32 getFreeCalls() const { return 0; }

        /** @return Total number of memoryAllocate and memoryCAllocate in the engine [in bytes] */
        virtual uint32 getAllocateCalls() const { return 0; }

        /** @return Total memory usage for the whole time of engine working [in bytes] */
        virtual uint64 getTotalMemoryUsage() const { return 0; }

    };

} // namespace Berserk

#endif //BERSERK_IALLOCATOR_H