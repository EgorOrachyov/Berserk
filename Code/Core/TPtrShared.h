/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TPTRSHARED_H
#define BERSERK_TPTRSHARED_H

#include <TPtr.h>
#include <AllocPool.h>
#include <Platform/Memory.h>
#include <Platform/Atomic.h>
#include <Platform/Mutex.h>

namespace Berserk {

    class PtrAllocator {
    private:
        struct Node {
            Node();
            AtomicInt32 refcount;
            const Function<void(void*)> *deallocate;
        };
        PtrAllocator();
        ~PtrAllocator();
        Node* allocate();
        void free(Node* node);
        static PtrAllocator& getSingleton();
    private:
        AllocPool mAlloc;
        Mutex mAccessMutex;
        template <typename T>
        friend class TPtrShared;
    };

    /**
     * @brief Reference counted shared pointer.
     *
     * Automatically increments reference for each copied pointer instance.
     * if references equals 0, then the object is automatically destroyed
     * and its memory freed by dealloc function.
     * @tparam T Type of referenced object
     */
    template <typename T>
    class TPtrShared : public TPtr<T> {
    public:
        TPtrShared() noexcept {
            mPtr = nullptr;
        }
        TPtrShared(T* object) noexcept : TPtrShared<T>() {
            if (object != nullptr) {
                mPtr = object;
                mNode = PtrAllocator::getSingleton().allocate();
                mNode->deallocate = nullptr;
                mNode->refcount.fetch_add(1);
            }
        }
        TPtrShared(T* object, const Function<void(void*)> *dealloc) : TPtrShared<T>() {
            if (object != nullptr) {
                mPtr = object;
                mNode = PtrAllocator::getSingleton().allocate();
                mNode->deallocate = dealloc;
                mNode->refcount.fetch_add(1);
            }
        }
        TPtrShared(const TPtrShared& other) noexcept {
            mPtr = other.mPtr;
            mNode = other.mNode;

            if (mNode)
                mNode->refcount.fetch_add(1);
        }
        TPtrShared(TPtrShared&& other) noexcept {
            mPtr = other.mPtr;
            mNode = other.mNode;
            other.mPtr = nullptr;
            other.mNode = nullptr;
        }
        ~TPtrShared() {
            if (mPtr) {
                auto refs = mNode->refcount.fetch_sub(1);

                // It was the last reference
                if (refs == 1) {
                    mPtr->~T();
                    if (mNode->deallocate) (*mNode->deallocate)(mPtr);
                    PtrAllocator::getSingleton().free((PtrAllocator::Node*) mNode);
                }

                mPtr = nullptr;
                mNode = nullptr;
            }
        }
        TPtrShared& operator=(const TPtrShared& other) noexcept {
            this->~TPtrShared();
            new (this) TPtrShared<T>(other);
            return *this;
        }
        TPtrShared& operator=(TPtrShared&& other) noexcept {
            this->~TPtrShared();
            new (this) TPtrShared<T>(std::move(other));
            return *this;
        }
        void free() {
            this->~TPtrShared();
        }
        template <typename M>
        explicit operator TPtrShared<M>() const {
            TPtrShared<M> ptr;
            ptr.mPtr = (M*) mPtr;
            ptr.mNode = mNode;

            if (mNode != nullptr)
                mNode->refcount.fetch_add(1);

            return ptr;
        }
        template <typename ... TArgs>
        static TPtrShared<T> make(TArgs &&... args) {
            void* mem = Memory::allocate(sizeof(T));
            T* obj = new (mem) T(std::forward<TArgs>(args)...);
            return TPtrShared<T>(obj,&Memory::DEFAULT_DEALLOC);
        }
    private:
        template <typename M>
        friend class TPtrShared;
        using TPtr<T>::mPtr;
        PtrAllocator::Node* mNode = nullptr;
    };

}

#endif //BERSERK_TPTRSHARED_H