/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_REF_HPP
#define BERSERK_REF_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Misc/Contracts.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    class RefCounted {
    public:
        RefCounted(): mRefsCount(1) {}
        virtual ~RefCounted() = default;

    protected:
        /** Must be implemented by the inheritor to properly release resources */
        virtual void OnReleased() const = 0;

        uint64 AddRefs() const {
            return mRefsCount.fetch_add(1);
        }

        uint64 Release() const {
            auto refs = mRefsCount.fetch_sub(1);

            if (refs == 1) {
                // Must release
                OnReleased();
            }

            return refs;
        }

        uint64 GetRefs() const {
            return mRefsCount.load();
        }

    protected:
        template<typename T>
        friend class Ref;

        mutable Platform::AtomicUint64 mRefsCount;
    };

    template <typename T>
    class Ref {
    public:
        Ref() = default;

        Ref(T* ptr, bool AddRefs = false) {
            mPtr = ptr;

            if (mPtr && AddRefs) {
                mPtr->AddRefs();
            }
        }

        Ref(const Ref<T> &ref) {
            mPtr = ref.mPtr;

            if (mPtr) {
                mPtr->AddRefs();
            }
        }

        Ref(Ref<T> &&ref) noexcept {
            mPtr = ref.mPtr;
            ref.mPtr = nullptr;
        }

        ~Ref() noexcept {
            if (mPtr) {
                mPtr->Release();
                mPtr = nullptr;
            }
        }

        Ref& operator=(const Ref<T>& other) {
            if (this == &other) {
                return *this;
            }

            this->~Ref();
            new (this) Ref<T>(other);
            return *this;
        }

        Ref& operator=(Ref<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~Ref();
            new (this) Ref<T>(std::move(other));
            return *this;
        }

        template<typename B>
        explicit operator Ref<B>() const {
            Ref<B> result;

            if (IsNotNull()) {
                result.mPtr = mPtr;
                result.mPtr->AddRefs();
            }

            return result;
        }

        bool operator==(const Ref<T> &other) const { return mPtr == other.mPtr; }
        bool operator!=(const Ref<T> &other) const { return mPtr != other.mPtr; }
        bool operator<=(const Ref<T> &other) const { return mPtr <= other.mPtr; }
        bool operator>=(const Ref<T> &other) const { return mPtr >= other.mPtr; }
        bool operator> (const Ref<T> &other) const { return mPtr >  other.mPtr; }
        bool operator< (const Ref<T> &other) const { return mPtr <  other.mPtr; }

        bool operator==(const T* other) const { return mPtr == other; }
        bool operator!=(const T* other) const { return mPtr != other; }
        bool operator<=(const T* other) const { return mPtr <= other; }
        bool operator>=(const T* other) const { return mPtr >= other; }
        bool operator> (const T* other) const { return mPtr >  other; }
        bool operator< (const T* other) const { return mPtr <  other; }

        T* operator->() const { return mPtr; }
        T& operator*() const { return *(mPtr); }

        explicit operator bool() const { return IsNotNull(); }

        bool IsNull() const { return mPtr == nullptr; }
        bool IsNotNull() const { return mPtr != nullptr; }

        T* GetPtrOrNull() const { return mPtr; }
        uint64 GetReferencesCount() const { return IsNotNull()? mPtr->GetRefs(): 0; }

    private:
        template<typename B>
        friend class Ref;

        T* mPtr = nullptr;
    };

    template<typename T>
    class Equals<Ref<T>> {
    public:
        bool operator()(const Ref<T>& a, const Ref<T>& b) const {
            return a == b;
        }
    };

}

#endif //BERSERK_REF_HPP