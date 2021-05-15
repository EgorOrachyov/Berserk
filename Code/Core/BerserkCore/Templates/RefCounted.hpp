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

#ifndef BERSERK_REFCOUNTED_HPP
#define BERSERK_REFCOUNTED_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Platform/Crc32.hpp>
#include <BerserkCore/Templates/SimplePtr.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Strings/String.hpp>

namespace Berserk {

    /**
     * Derive from this class to enable thread safe ref-count logic in your class.
     * @note Suitable only for classes with virtual inheritance
     */
    class RefCountedThreadSafe {
    protected:
        RefCountedThreadSafe(): mRefsCount(0) {}
        virtual ~RefCountedThreadSafe() { assert(mRefsCount.load() == 0); }

        /** Protected for safety */
        RefCountedThreadSafe& operator=(const RefCountedThreadSafe&) { return *this; }
        RefCountedThreadSafe& operator=(RefCountedThreadSafe&&) noexcept { return *this; }

        /** Must be implemented by the inheritor to properly release resources */
        virtual void OnReleased() const = 0;

        /** Added when ref is added (ptr copied) */
        uint32 AddRefs() const {
            return mRefsCount.fetch_add(1);
        }

        /** Released when ref ptr is deleted */
        uint32 Release() const {
            auto refs = mRefsCount.fetch_sub(1);

            if (refs == 1) {
                // Last must release
                OnReleased();
            }

            return refs;
        }

        /** @return Number of refs to this object */
        uint32 GetRefs() const {
            return mRefsCount.load();
        }

    protected:
        template<typename T>
        friend class RefCounted;

        mutable AtomicUint32 mRefsCount;
    };

    enum class RefCountedBoxing {
        AddRefs,
        Keep
    };

    /**
     * RefCounted is a reference counted shared object pointer.
     * Object must itself provide reference counting mechanism.
     *
     * @tparam T Type of the object to reference
     */
    template <typename T>
    class RefCounted: public SimplePtr<T> {
    public:

        using SimplePtr<T>::mPtr;

        RefCounted() = default;
        RefCounted(std::nullptr_t) : RefCounted() {}

        explicit RefCounted(T* ptr, RefCountedBoxing boxing) {
            mPtr = ptr;

            if (mPtr && boxing == RefCountedBoxing::AddRefs) {
                mPtr->AddRefs();
            }
        }

        RefCounted(const RefCounted<T> &ref) {
            mPtr = ref.mPtr;

            if (mPtr) {
                mPtr->AddRefs();
            }
        }

        RefCounted(RefCounted<T> &&ref) noexcept {
            mPtr = ref.mPtr;
            ref.mPtr = nullptr;
        }

        ~RefCounted() noexcept {
            if (mPtr) {
                mPtr->Release();
                mPtr = nullptr;
            }
        }

        RefCounted& operator=(const RefCounted<T>& other) {
            if (this == &other) {
                return *this;
            }

            this->~RefCounted();
            new (this) RefCounted<T>(other);
            return *this;
        }

        RefCounted& operator=(RefCounted<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~RefCounted();
            new (this) RefCounted<T>(std::move(other));
            return *this;
        }

        template<typename B>
        explicit operator RefCounted<B>() const {
            RefCounted<B> result;

            if (IsNotNull()) {
                result.mPtr = mPtr;
                result.mPtr->AddRefs();
            }

            return result;
        }

        T* operator->() const { return mPtr; }
        T& operator*() const { return *(mPtr); }

        explicit operator bool() const { return IsNotNull(); }

        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }

        T* Get() const { return mPtr; }
        uint32 GetRefs() const { return IsNotNull() ? mPtr->GetRefs() : 0; }

    private:
        template<typename B>
        friend class RefCounted;
    };

    template<typename T>
    class Equals<RefCounted<T>> {
    public:
        bool operator()(const RefCounted<T>& a, const RefCounted<T>& b) const {
            return a == b;
        }
    };

    template<typename T>
    class Hash<RefCounted<T>> {
    public:
        bool operator()(const RefCounted<T> &a) const {
            return Crc32::Hash((const void *) a.Get(), sizeof(T *));
        }
    };

    template<typename T>
    class TextPrint<RefCounted<T>> {
    public:
        template<typename Stream>
        void operator()(Stream &stream, const RefCounted<T> &a) const {
            stream.Add(String::From((const void *) a.Get()));
        }
    };

}

#endif //BERSERK_REFCOUNTED_HPP