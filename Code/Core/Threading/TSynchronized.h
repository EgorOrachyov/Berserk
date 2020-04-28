/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TSYNCHRONIZED_H
#define BERSERK_TSYNCHRONIZED_H

#include <Platform/Mutex.h>

namespace Berserk {

    /**
     * Wraps an object of type T to synchronized container
     * @tparam T Type of the object to wrap
     */
    template <typename T>
    class TSynchronized {
    public:

        template <typename ... TArgs>
        explicit TSynchronized(TArgs&& ... args) : mContainer(std::forward<TArgs>(args)...) { }

        ~TSynchronized() = default;

    private:

        template <typename P>
        friend class TGuard;

        template <typename P>
        friend class TUnsafeGuard;

        T mContainer;
        Mutex mMutex;
    };

    /**
     * Provides thread safe access to the synchronized object T
     * @tparam T Type of the object to provide access
     */
    template <typename T>
    class TGuard {
    public:

        explicit TGuard(TSynchronized<T> &container) : mContainerRef(container) {
            container.mMutex.lock();
        }

        ~TGuard() {
            mContainerRef.mMutex.unlock();
        }

        T& get() {
            return mContainerRef.mContainer;
        }

        const T& get() const {
            return mContainerRef.mContainer;
        }

        T* operator->() {
            return &mContainerRef.mContainer;
        }

        const T* operator->() const {
            return &mContainerRef.mContainer;
        }

    private:

        TSynchronized<T> &mContainerRef;

    };

    /**
     * Provides thread-unsafe access to the synchronized object T
     * @tparam T Type of the object to provide access
     */
    template <typename T>
    class TUnsafeGuard {
    public:

        explicit TUnsafeGuard(TSynchronized<T> &container) : mContainerRef(container) { }

        ~TUnsafeGuard() = default;

        T& get() {
            return mContainerRef.mContainer;
        }

        const T& get() const {
            return mContainerRef.mContainer;
        }

        T* operator->() {
            return &mContainerRef.mContainer;
        }

        const T* operator->() const {
            return &mContainerRef.mContainer;
        }

    private:

        TSynchronized<T> &mContainerRef;

    };

}

#endif //BERSERK_TSYNCHRONIZED_H