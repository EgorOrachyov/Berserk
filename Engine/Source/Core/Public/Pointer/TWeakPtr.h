//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_TWEAKPTR_H
#define BERSERK_TWEAKPTR_H

#include <Pointer/TSharedPtr.h>
#include <Pointer/PtrManager.h>

namespace Berserk
{

    /**
     * Generic weak pointer class. Allows to handle dynamically created
     * resource with weak reference counting. Could be created only by shared pointer.
     *
     * When number of shared pointers which possess the same resource equals 0, then
     * the resource becomes destroyed and weak pointers lose their resources.
     *
     * When you want to access weak pointer resource you must make shared pointer from weak,
     * and then release shared pointer, if it is no more needed.
     *
     * Usage scheme:
     *
     * TWeakPointer ptr = someSharedPtr;
     * ...
     * {
     *   // want to access data
     *   TSharedPtr p = ptr.lock();
     *   if (!p.isNull)
     *   {
     *      // do some important operations
     *      ...
     *   }
     * }
     *
     * @note Weak pointer could be created from any thread
     * @warning Concrete weak pointer should be process only in one thread.
     *          You cannot share the same pointer among other threads.
     *
     * @tparam T Class type to store pointer to that
     */
    template <typename T>
    class ENGINE_API TWeakPtr
    {
    private:

        /** Ptrs shared info */
        typedef PtrManager::PtrInfo Info;

        /** Manager for all shared ptrs */
        static PtrManager& manager;

    public:

        GENERATE_NEW_DELETE(TWeakPtr);

        /** Initialize null pointer */
        TWeakPtr()
        {

        }

        /** From shared ptr */
        TWeakPtr(const TSharedPtr<T>& other)
        {

            if (other.mInfo) other.mInfo->incRefWeak();
            mInfo = other.mInfo;
            mSource = other.mSource;
        }

        TWeakPtr(const TWeakPtr& other)
        {
            if (other.mInfo) other.mInfo->incRefWeak();
            mInfo = other.mInfo;
            mSource = other.mSource;
        }

        TWeakPtr(TWeakPtr&& other) noexcept
        {
            mInfo = other.mInfo;
            mSource = other.mSource;
            other.mInfo = nullptr;
            other.mSource = nullptr;
        }

        ~TWeakPtr()
        {
            if (mInfo != nullptr)
            {
                /** Decrement ref count and check whether it was last weak ptr */
                int32 prevValue = mInfo->decRefWeak();

                /** Delete only if there is no shared ref */
                if (prevValue == 1 && !mInfo->hasSharedRefs())
                {
                    /** Centralised delete */
                    manager.deleteNode_CallByWeakPtr(mInfo);
                }

                mInfo = nullptr;
                mSource = nullptr;
            }
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
        bool operator==(const TWeakPtr& other) const
        {
            return mSource == other.mSource;
        }

        /** @return this != other */
        bool operator!=(const TWeakPtr& other) const
        {
            return mSource != other.mSource;
        }

        /** @return this <= other */
        bool operator<=(const TWeakPtr& other) const
        {
            return mSource <= other.mSource;
        }

        /** @return this >= other */
        bool operator>=(const TWeakPtr& other) const
        {
            return mSource >= other.mSource;
        }

        /** @return this < other */
        bool operator<(const TWeakPtr& other) const
        {
            return mSource < other.mSource;
        }

        /** @return this > other */
        bool operator>(const TWeakPtr& other) const
        {
            return mSource > other.mSource;
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

        /**
         * Attempts to access weak resource
         * @return Shared pointer, if resource available, otherwise null pointer
         */
        TSharedPtr<T> lock()
        {
            if (mInfo)
            {
                AtomicInt &sc = mInfo->getSharedCounter();
                AtomicInt &wc = mInfo->getWeakCounter();

                int32 value;

                do
                {
                    value = sc.load();
                    if (value == 0) return TSharedPtr<T>();
                }
                while (!sc.compare_exchange_weak(value, value + 1));

                return TSharedPtr<T>(mInfo, mSource);
            }

            return TSharedPtr<T>();
        }

        TWeakPtr& operator=(const TWeakPtr& other)
        {
            this->~TWeakPtr();

            if (other.mInfo) other.mInfo->incRefWeak();
            mInfo = other.mInfo;
            mSource = other.mSource;

            return *this;
        }

        /**
        * Cast this weak pointer to super class T
        * @tparam TCast Cast type of object T
        * @return TWeakPtr of TSuper class
        */
        template <typename TCast>
        TWeakPtr<TCast> cast() const
        {
            TWeakPtr<TCast> p;
            TWeakPtr<T> *_p = (TWeakPtr<T>*) &p;

            if (mInfo) mInfo->incRefWeak();

            _p->mInfo = mInfo;
            _p->mSource = mSource;

            return p;
        }

        /**
         * Cast this weak pointer to super class T
         * @tparam TCast Cast type of object T
         * @return TWeakPtr of TSuper class
         */
        template <typename TCast>
        explicit operator TWeakPtr<TCast>() const
        {
            return cast<TCast>();
        }

    private:

        /** Weak ptr info */
        Info* mInfo = nullptr;

        /** Actual source [may be invalid: must no be dereferenced] */
        T* mSource = nullptr;

    };

    template <typename T>
    PtrManager& TWeakPtr<T>::manager = PtrManager::get();

} // namespace Berserk

#endif //BERSERK_TWEAKPTR_H