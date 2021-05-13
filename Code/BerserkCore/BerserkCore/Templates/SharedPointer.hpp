/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SHAREDPOINTER_HPP
#define BERSERK_SHAREDPOINTER_HPP

#include <BerserkCore/Templates/SharedPointerInternal.hpp>
#include <BerserkCore/Platform/System.hpp>

namespace Berserk {



    template <typename T>
    class SharedRef: public SimplePtr<T> {
    public:
        using SimplePtr<T>::mPtr;

        SharedRef() = default;

        explicit SharedRef(T* ptr) {
            assert(ptr != nullptr);

            mPtr = ptr;
            auto controller = Memory::Make<DefaultRefController<T>>(ptr, DefaultDeleter<T>());
            mController = std::move(SharedRefController(controller));
        }

        SharedRef(const SharedRef& other) noexcept {
            mPtr = other.mPtr;
            mController = other.mController;
        }

        SharedRef(SharedRef&& other) noexcept {
            mPtr = other.mPtr;
            mController = std::move(other.mController);

            other.mPtr = nullptr;
        }

        ~SharedRef() noexcept {
            mPtr = nullptr;
        };

        SharedRef& operator=(const SharedRef<T>& other) {
            if (this == &other) {
                return *this;
            }

            SharedRef<T> tmp(other);
            std::swap(*this, tmp);

            return *this;
        }

        SharedRef& operator=(SharedRef<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~SharedRef();
            mPtr = other.mPtr;
            mController = std::move(other.mController);

            other.mPtr = nullptr;

            return *this;
        }

        template<typename B>
        explicit operator SharedRef<B>() const {
            SharedRef<B> result;

            if (IsNotNull()) {
                result.mPtr = mPtr;
                result.mController = mController;
                result.mController.AddSharedRef();
            }

            return result;
        }

        T* operator->() const {
            assert(IsNotNull());
            return mPtr;
        }

        T& operator*() const {
            assert(IsNotNull());
            return *mPtr;
        }

        explicit operator bool() const { return IsNotNull(); }

        T& Get() const {
            assert(IsNotNull());
            return mPtr;
        }

        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }
        uint32 GetSharedRefs() const { return mController.IsEmpty()? 0: mController.GetSharedRefs(); }
        uint32 GetWeakRefs() const { return mController.IsEmpty()? 0: mController.GetWeakRefs(); }

        static SharedRef<T> MakeMove(T&& value) {
            SharedRef<T> result;

            auto controller = Memory::Make<ReferenceControllerWithObject<T>>(std::move(value));
            result.mController = std::move(SharedRefController(controller));
            result.mPtr = controller->Get();

            return result;
        }

        template<typename ... TArgs>
        static SharedRef<T> Make(TArgs&& ... args) {
            T tmp(std::forward<TArgs>(args)...);
            return MakeMove(std::move(tmp));
        }

    private:
        template<typename K>
        friend class SharedRef;
        template<typename K>
        friend class SharedPtr;
        template<typename K>
        friend class WeakPtr;

        SharedRefController mController;
    };

    template <typename T>
    class SharedPtr: public SimplePtr<T> {
    public:
        using SimplePtr<T>::mPtr;

        SharedPtr() = default;

        explicit SharedPtr(T* ptr) {
            if (ptr != nullptr) {
                mPtr = ptr;
                auto controller = Memory::Make<DefaultRefController<T>>(ptr, DefaultDeleter<T>());
                mController = std::move(SharedRefController(controller));
            }
        }

        explicit SharedPtr(const SharedRef<T>& other) noexcept {
            mPtr = other.mPtr;
            mController = other.mController;
        }

        SharedPtr(const SharedPtr& other) noexcept {
            mPtr = other.mPtr;
            mController = other.mController;
        }

        SharedPtr(SharedPtr&& other) noexcept {
            mPtr = other.mPtr;
            mController = std::move(other.mController);

            other.mPtr = nullptr;
        }

        ~SharedPtr() noexcept {
            mPtr = nullptr;
        };

        SharedPtr& operator=(const SharedPtr<T>& other) {
            if (this == &other) {
                return *this;
            }

            SharedPtr<T> tmp(other);
            std::swap(*this, tmp);

            return *this;
        }

        SharedPtr& operator=(SharedPtr<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~SharedPtr();
            mPtr = other.mPtr;
            mController = std::move(other.mController);

            other.mPtr = nullptr;

            return *this;
        }

        template<typename B>
        explicit operator SharedPtr<B>() const {
            SharedPtr<B> result;

            if (IsNotNull()) {
                result.mPtr = mPtr;
                result.mController = mController;
                result.mController.AddSharedRef();
            }

            return result;
        }

        T* operator->() const {
            assert(IsNotNull());
            return mPtr;
        }

        T& operator*() const {
            assert(IsNotNull());
            return *mPtr;
        }

        explicit operator bool() const { return IsNotNull(); }

        SharedRef<T> ToSharedRef() const {
            assert(IsNotNull());

            SharedRef<T> result;
            result.mPtr = mPtr;
            result.mController = mController;

            return result;
        }

        T* Get() const { return mPtr; }

        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }
        uint32 GetSharedRefs() const { return mController.IsEmpty()? 0: mController.GetSharedRefs(); }
        uint32 GetWeakRefs() const { return mController.IsEmpty()? 0: mController.GetWeakRefs(); }

        void Release() {
            mPtr = nullptr;
            mController.ReleaseSharedRef();
        }

        static SharedPtr<T> MakeMove(T&& value) {
            SharedPtr<T> result;

            auto controller = Memory::Make<ReferenceControllerWithObject<T>>(std::move(value));
            result.mController = std::move(SharedRefController(controller));
            result.mPtr = controller->Get();

            return result;
        }

        template<typename ... TArgs>
        static SharedPtr<T> Make(TArgs&& ... args) {
            T tmp(std::forward<TArgs>(args)...);
            return MakeMove(std::move(tmp));
        }

    private:
        template<typename K>
        friend class SharedRef;
        template<typename K>
        friend class SharedPtr;
        template<typename K>
        friend class WeakPtr;

        SharedRefController mController;
    };

    template <typename T>
    class WeakPtr: public SimplePtr<T> {
    public:
        using SimplePtr<T>::mPtr;

        WeakPtr() = default;

        explicit WeakPtr(const SharedRef<T> &other) {
            mPtr = other.mPtr;
            mController = other.mController;
        }

        explicit WeakPtr(const SharedPtr<T> &other) {
            mPtr = other.mPtr;
            mController = other.mController;
        }

        WeakPtr(const WeakPtr& other) noexcept {
            mPtr = other.mPtr;
            mController = other.mController;
        }

        WeakPtr(WeakPtr&& other) noexcept {
            mPtr = other.mPtr;
            mController = std::move(other.mController);

            other.mPtr = nullptr;
        }

        ~WeakPtr() noexcept {
            mPtr = nullptr;
        };

        WeakPtr& operator=(const WeakPtr<T>& other) {
            if (this == &other) {
                return *this;
            }

            WeakPtr<T> tmp(other);
            std::swap(*this, tmp);

            return *this;
        }

        WeakPtr& operator=(WeakPtr<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~WeakPtr();
            mPtr = other.mPtr;
            mController = std::move(other.mController);

            other.mPtr = nullptr;

            return *this;
        }

        template<typename B>
        explicit operator WeakPtr<B>() const {
            WeakPtr<B> result;

            if (IsNotNull()) {
                result.mPtr = mPtr;
                result.mController = mController;
                result.mController.AddSharedRef();
            }

            return result;
        }

        explicit operator bool() const { return IsNotNull(); }

        SharedPtr<T> ToSharedPtr() const {
            SharedPtr<T> result;

            if (mController.TryAddSharedRef()) {
                result.mPtr = mPtr;
                result.mController.RawSet(mController.mRefController);
            }

            return result;
        }

        void Release() {
            mPtr = nullptr;
            mController.ReleaseWeakRef();
        }

        T* Get() const { return mPtr; }
        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }
        uint32 GetSharedRefs() const { return mController.IsEmpty()? 0: mController.GetSharedRefs(); }
        uint32 GetWeakRefs() const { return mController.IsEmpty()? 0: mController.GetWeakRefs(); }

    private:
        template<typename K>
        friend class SharedRef;
        template<typename K>
        friend class SharedPtr;
        template<typename K>
        friend class WeakPtr;

        WeakRefController mController;
    };

}

#endif //BERSERK_SHAREDPOINTER_HPP