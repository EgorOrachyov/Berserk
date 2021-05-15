/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_SHAREDPOINTER_HPP
#define BERSERK_SHAREDPOINTER_HPP

#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Platform/Crc32.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Templates/SimplePtr.hpp>
#include <BerserkCore/Templates/SharedPointerInternal.hpp>

namespace Berserk {

    /**
     * SharedRef is reference counted shared object not-null pointer.
     * Controls object life-time. Destroys object if no shared references in the system to the object.
     *
     * @note Thread-safe
     *
     * @tparam T Type of the object
     */
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
            return *mPtr;
        }

        T* GetPtr() const {
            return mPtr;
        }

        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }
        bool IsUnique() const { return GetSharedRefs() == 1; }
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
            auto controller = Memory::Make<ReferenceControllerWithObject<T>>();

            try {
                new (controller->Get()) T(std::forward<TArgs>(args)...);

                SharedRef<T> result;
                result.mController = std::move(SharedRefController(controller));
                result.mPtr = controller->Get();

                return result;
            }
            catch (const std::exception& e) {
                Memory::Release(controller);
                throw;
            }
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

    /**
     * SharedPtr is a reference counted shared object pointer.
     * Controls object life-time. Destroys object if no shared references in the system to the object.
     *
     * @note Thread-safe
     *
     * @tparam T Type of the object
     */
    template <typename T>
    class SharedPtr: public SimplePtr<T> {
    public:
        using SimplePtr<T>::mPtr;

        SharedPtr() = default;
        SharedPtr(std::nullptr_t) : SharedPtr() {}

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
        bool IsUnique() const { return GetSharedRefs() == 1; }
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
            auto controller = Memory::Make<ReferenceControllerWithObject<T>>();

            try {
                new (controller->Get()) T(std::forward<TArgs>(args)...);

                SharedPtr<T> result;
                result.mController = std::move(SharedRefController(controller));
                result.mPtr = controller->Get();

                return result;
            }
            catch (const std::exception& e) {
                Memory::Release(controller);
                throw;
            }
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

    /**
     * WeakPtr is a reference counted weak object pointer.
     * Allows to get ptr to the object, it has shared references.
     *
     * @note Thread-safe
     *
     * @tparam T Type of the object
     */
    template <typename T>
    class WeakPtr: public SimplePtr<T> {
    public:
        using SimplePtr<T>::mPtr;

        WeakPtr() = default;
        WeakPtr(std::nullptr_t) : WeakPtr() {}

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

            if (mController.IsNotEmpty() && mController.TryAddSharedRef()) {
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

    /**
     * Derive your class from SharedFromThis to access shared ref within you class instance.
     * Use internal set methods to assign initial self-ptr.
     *
     * @tparam T Type of the self class
     */
    template<typename T>
    class SharedFromThis {
    public:

        /**
         * Provides access to a shared reference to this object.
         *
         * @note Self-must be assigned before the call to this function.
         * @note It is illegal to call this in the object's destructor.
         *
         * @return	Returns this object as a shared pointer
         */
        SharedRef<T> AsShared() {
            SharedPtr<T> tmp{mSelf.ToSharedPtr()};

            // In general, it must work.
            // If it fails, then:
            // - You request ptr if self was not assigned
            // - You request ptr in destruction chain
            assert(tmp.Get() == this);

            return tmp.ToSharedRef();
        }

        /**
         * Provides access to a shared reference to this object (const).
         *
         * @note Self-must be assigned before the call to this function.
         * @note It is illegal to call this in the object's destructor.
         *
         * @return	Returns this object as a shared pointer (const)
         */
        SharedRef<const T> AsShared() const {
            SharedPtr<const T> tmp{mSelf.ToSharedPtr()};

            // In general, it must work.
            // If it fails, then:
            // - You request ptr if self was not assigned
            // - You request ptr in destruction chain
            assert(tmp.Get() == this);

            return tmp.ToSharedRef();
        }

        /**
         * INTERNAL USAGE: Assign self from shared pointer.
         *
         * @note Call as soon as object is constructed.
         * @note Before this function call any AsShared() call is invalid.
         *
         * @param self Shared pointer to self object
         */
        void AssignSelf(const SharedRef<T> &self) {
            assert(mSelf.IsNull());
            assert(self.IsNotNull());
            mSelf = std::move(WeakPtr<T>{self});
        }

    protected:

        SharedFromThis() = default;
        ~SharedFromThis() = default;
        SharedFromThis& operator=(const SharedFromThis& other) {};

    private:
        // Weak pointer to ourselves.
        // If last shared reference is released, then this weak ptr is released too
        // And meta will be destructed.
        WeakPtr<T> mSelf;
    };

    template<typename T>
    class Equals<SharedRef<T>> {
    public:
        bool operator()(const SharedRef<T> &a, const SharedRef<T> &b) const {
            return a == b;
        }
    };

    template<typename T>
    class Equals<SharedPtr<T>> {
    public:
        bool operator()(const SharedPtr<T> &a, const SharedPtr<T> &b) const {
            return a == b;
        }
    };

    template<typename T>
    class Equals<WeakPtr<T>> {
    public:
        bool operator()(const WeakPtr<T> &a, const WeakPtr<T> &b) const {
            return a == b;
        }
    };

    template<typename T>
    class Hash<SharedRef<T>> {
    public:
        bool operator()(const SharedRef<T> &a) const {
            return Crc32::Hash((const void*) a.GetPtr(), sizeof(T*));
        }
    };

    template<typename T>
    class Hash<SharedPtr<T>> {
    public:
        bool operator()(const SharedPtr<T> &a) const {
            return Crc32::Hash((const void*) a.Get(), sizeof(T*));
        }
    };

    template<typename T>
    class Hash<WeakPtr<T>> {
    public:
        bool operator()(const WeakPtr<T> &a) const {
            return Crc32::Hash((const void*) a.Get(), sizeof(T*));
        }
    };

    template<typename T>
    class TextPrint<SharedRef<T>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const SharedRef<T>& a) const {
            stream.Add(String::From((const void*) a.GetPtr()));
        }
    };

    template<typename T>
    class TextPrint<SharedPtr<T>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const SharedPtr<T>& a) const {
            stream.Add(String::From((const void*) a.Get()));
        }
    };

    template<typename T>
    class TextPrint<WeakPtr<T>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const WeakPtr<T>& a) const {
            stream.Add(String::From((const void*) a.Get()));
        }
    };

}

#endif //BERSERK_SHAREDPOINTER_HPP