//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_TSHAREDPTR_H
#define BERSERK_TSHAREDPTR_H

#include <Memory/IAllocator.h>
#include <Resource/SharedPtrManager.h>

namespace Berserk
{

    /**
     * Generic shared pointer class. Allows to handle dynamically created
     * resource with reference counting. When the number of references equals 0,
     * then the resource will be destroyed and memory freed by specified allocator.
     *
     * @note Shared pointer could be created from any thread
     * @warning Concrete shared pointer should be process only in one thread.
     *          You cannot share the same pointer among other threads.
     *
     * @tparam T Class type to store pointer to that
     */
    template <typename T>
    class ENGINE_API TSharedPtr
    {
    private:

        /** Ptrs shared info */
        typedef SharedPtrManager::SharedPtrInfo Info;

        /** Manager for all shared ptrs */
        static SharedPtrManager& manager;

    public:

        /** Initialize null pointer */
        TSharedPtr()
        {
            mInfo = manager.emptyNode();
        }

        /** Initialize from source pointer and allocator */
        TSharedPtr(T* source, IAllocator* allocator)
        {
            mInfo = manager.createNode(allocator);
            mSource = source;
        }

        /**
         * Allows to initialize resource inside the shared ptr
         * @tparam TArgs Arguments type for T source constructor
         * @param allocator Allocator to allocate a memory
         * @param args Arguments for constructor
         */
        template <typename ... TArgs>
        explicit TSharedPtr(IAllocator& allocator, const TArgs& ... args)
        {
            mSource = new (allocator.allocate(sizeof(T))) T(args ...);
            mInfo = manager.createNode(&allocator);
        }

        TSharedPtr(const TSharedPtr& other)
        {
            manager.incReference(other.mInfo);
            mInfo = other.mInfo;
            mSource = other.mSource;
        }

        TSharedPtr(TSharedPtr&& other) noexcept
        {
            mInfo = other.mInfo;
            mSource = other.mSource;
            other.mInfo = nullptr;
            other.mSource = nullptr;
        }

        ~TSharedPtr()
        {
            if (mInfo != nullptr)
            {
                auto fun = [](void* source, IAllocator* allocator)
                {
                    if (source != nullptr)
                    {
                        if (allocator != nullptr)
                        {
                            T* s = (T*) source;
                            s->~T();
                            allocator->free(source);
                        }
                        else
                        {
                            T* s = (T*) source;
                            s->~T();
                        }
                    }
                };

                manager.deleteNode(mSource, fun, mInfo);
                mInfo = nullptr;
                mSource = nullptr;
            }
        }

        TSharedPtr& operator=(const TSharedPtr& other)
        {
            this->~TSharedPtr();
            manager.incReference(other.mInfo);
            mInfo = other.mInfo;
            mSource = other.mSource;
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
        const T* operator->() const
        {
            return mSource;
        }

        /** @return raw resource pointer */
        T* operator->()
        {
            return mSource;
        }

        /** @return true if it is null pointer */
        bool isNull() const
        {
            return (mSource == nullptr);
        }

        /** @return this == other */
        bool operator==(const TSharedPtr& other) const
        {
            return mSource == other.mSource;
        }

        /** @return this != other */
        bool operator!=(const TSharedPtr& other) const
        {
            return mSource != other.mSource;
        }

        /** @return this <= other */
        bool operator<=(const TSharedPtr& other) const
        {
            return mSource <= other.mSource;
        }

        /** @return this >= other */
        bool operator>=(const TSharedPtr& other) const
        {
            return mSource >= other.mSource;
        }

        /** @return this < other */
        bool operator<(const TSharedPtr& other) const
        {
            return mSource < other.mSource;
        }

        /** @return this > other */
        bool operator>(const TSharedPtr& other) const
        {
            return mSource > other.mSource;
        }

    private:

        /** Shared info */
        Info* mInfo = nullptr;

        /** Actual source */
        T* mSource = nullptr;

    };

    template <typename T>
    SharedPtrManager& TSharedPtr<T>::manager = SharedPtrManager::get();


} // namespace Berserk

#endif //BERSERK_TSHAREDPTR_H