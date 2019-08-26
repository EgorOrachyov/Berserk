//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_TSHAREDPTR_H
#define BERSERK_TSHAREDPTR_H

#include <Memory/IAllocator.h>
#include <Resource/PtrManager.h>
#include <Misc/AssertDev.h>

namespace Berserk
{

    template <typename T> class TWeakPtr;

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
        typedef PtrManager::PtrInfo Info;

        /** Manager for all shared ptrs */
        static PtrManager& manager;

    protected:

        /** Private to create from weak ptr info [only internal usage] */
        TSharedPtr(Info* info, T* source)
        {
            mInfo = info;
            mSource = source;
        }

    public:

        GENERATE_NEW_DELETE(TSharedPtr);

        /** Initialize null pointer */
        TSharedPtr()
        {

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
            if (other.mInfo) other.mInfo->incRefShared();
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

                /** Decrement ref count and check whether it was last shared ptr */
                int32 prevValue = mInfo->decRefShared();

                if (prevValue == 1)
                {
                    /** Centralised delete */
                    manager.deleteNode_CallBySharedPtr(mSource, fun, mInfo);
                }

                mInfo = nullptr;
                mSource = nullptr;
            }
        }

        TSharedPtr& operator=(const TSharedPtr& other)
        {
            this->~TSharedPtr();

            if (other.mInfo) other.mInfo->incRefShared();
            mInfo = other.mInfo;
            mSource = other.mSource;

            return *this;
        }

        /**
         * Cast this shared pointer to super class T
         * @tparam TCast Cast type of object T
         * @return TSharedPtr of TSuper class
         */
        template <typename TCast>
        TSharedPtr<TCast> cast() const
        {
            TSharedPtr<TCast> p;
            TSharedPtr<T> *_p = (TSharedPtr<T>*) &p;

            if (mInfo) mInfo->incRefShared();

            _p->mInfo = mInfo;
            _p->mSource = mSource;

            return p;
        }

        /**
         * Cast this shared pointer to super class T
         * @tparam TCast Cast type of object T
         * @return TSharedPtr of TSuper class
         */
        template <typename TCast>
        explicit operator TSharedPtr<TCast>() const
        {
            return cast<TCast>();
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
        T* pointer() const
        {
            return mSource;
        }

        /** @return raw resource pointer */
        const T* operator->() const
        {
            return mSource;
        }

        T* operator->()
        {
            return mSource;
        }

        /** @return Number of shared references to handled resource */
        uint32 getSharedReferences() const
        {
            if (mInfo) return mInfo->getSharedCounter().load(std::memory_order_relaxed);
            return 0;
        }

        /** @return Number of weak references to handled resource */
        uint32 getWeakReferences() const
        {
            if (mInfo) return mInfo->getWeakCounter().load(std::memory_order_relaxed);
            return 0;
        }

        /** @return true if it is null pointer */
        bool isNull() const
        {
            return (mInfo == nullptr);
        }

        /** @return true if it is not null pointer */
        bool isPresent() const
        {
            return (mInfo != nullptr);
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

        friend class TWeakPtr<T>;

        /** Shared info */
        Info* mInfo = nullptr;

        /** Actual source */
        T* mSource = nullptr;

    };

    template <typename T>
    PtrManager& TSharedPtr<T>::manager = PtrManager::get();


} // namespace Berserk

#endif //BERSERK_TSHAREDPTR_H