/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TPTRSHARED_HPP
#define BERSERK_TPTRSHARED_HPP

#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    namespace Details {
        void* PtrGenericAllocate(uint64 size);
        void PtrGenericFree(void* memory);
        void* PtrMetaAllocate(uint64 size);
        void PtrMetaFree(void* memory);
    }

    template <typename T>
    class TPtrShared {
    public:
        TPtrShared() = default;

        TPtrShared(T* ptr) {
            if (ptr != nullptr) {
                mMetaRef = new (Details::PtrMetaAllocate(sizeof(TMeta))) TMeta(ptr);
            }
        }

        TPtrShared(const TPtrShared& other) noexcept {
            mMetaRef = other.mMetaRef;

            if (mMetaRef) {
                mMetaRef->AddRef();
            }
        }

        TPtrShared(TPtrShared&& other) noexcept {
            mMetaRef = other.mMetaRef;
            other.mMetaRef = nullptr;
        }

        ~TPtrShared() noexcept {
            if (mMetaRef) {
                if (mMetaRef->Release()) {
                    mMetaRef->~TMeta();
                    Details::PtrMetaFree(mMetaRef);
                }

                mMetaRef = nullptr;
            }
        }

        TPtrShared& operator=(const TPtrShared<T>& other) {
            if (this == &other) {
                return *this;
            }

            this->~TPtrShared();
            new (this) TPtrShared<T>(other);
            return *this;
        }

        TPtrShared& operator=(TPtrShared<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~TPtrShared();
            new (this) TPtrShared<T>(std::move(other));
            return *this;
        }

        template<typename B>
        explicit operator TPtrShared<B>() const {
            TPtrShared<B> result;

            if (IsNotNull()) {
                result.mMetaRef = (typename TPtrShared<B>::TMeta*) mMetaRef;
                result.mMetaRef->AddRef();
            }

            return result;
        }

        bool operator==(const TPtrShared<T> &other) const { return mMetaRef == other.mMetaRef; }
        bool operator!=(const TPtrShared<T> &other) const { return mMetaRef != other.mMetaRef; }
        bool operator<=(const TPtrShared<T> &other) const { return GetPtrOrNull() <= other.GetPtrOrNull(); }
        bool operator>=(const TPtrShared<T> &other) const { return GetPtrOrNull() >= other.GetPtrOrNull(); }
        bool operator> (const TPtrShared<T> &other) const { return GetPtrOrNull() >  other.GetPtrOrNull(); }
        bool operator< (const TPtrShared<T> &other) const { return GetPtrOrNull() <  other.GetPtrOrNull(); }

        bool operator==(const T* other) const { return mMetaRef == other; }
        bool operator!=(const T* other) const { return mMetaRef != other; }
        bool operator<=(const T* other) const { return GetPtrOrNull() <= other; }
        bool operator>=(const T* other) const { return GetPtrOrNull() >= other; }
        bool operator> (const T* other) const { return GetPtrOrNull() >  other; }
        bool operator< (const T* other) const { return GetPtrOrNull() <  other; }

        T* operator->() const { return mMetaRef->pointer;; }
        T& operator*() const { return *(mMetaRef->pointer); }

        operator bool() const { return IsNotNull(); }

        bool IsNull() const { return mMetaRef == nullptr; }
        bool IsNotNull() const { return mMetaRef != nullptr; }

        T* GetPtrOrNull() const { return IsNotNull()? mMetaRef->pointer: nullptr; }
        uint32 GetReferencesCount() const { return IsNotNull()? mMetaRef->GetRefs(): 0; }

        static TPtrShared MakeMove(T& value) {
            TPtrShared result;
            T* created = new (Details::PtrGenericAllocate(sizeof(T))) T(std::move(value));
            result.mMetaRef = new (Details::PtrMetaAllocate(sizeof(TMeta))) TMeta(created);
            return result;
        }

        template<typename ... TArgs>
        static TPtrShared MakeFromArgs(TArgs&& ... args) {
            TPtrShared result;
            T* created = new (Details::PtrGenericAllocate(sizeof(T))) T(std::forward<TArgs>(args)...);
            result.mMetaRef = new (Details::PtrMetaAllocate(sizeof(TMeta))) TMeta(created);
            return result;
        }

    private:

        class TMeta {
        public:
            AtomicInt32 references;
            T* pointer = nullptr;

            TMeta(T* ptr) : references(1), pointer(ptr) { }
            virtual ~TMeta() = default;

            virtual bool Release() {
                bool isLastRef = false;

                if (pointer) {
                    int32 prev = references.fetch_sub(1);

                    // The last one
                    if (prev == 1) {
                        isLastRef = true;
                        pointer->~T();
                        Details::PtrGenericFree(pointer);
                        pointer = nullptr;
                    }
                }

                return isLastRef;
            };

            void AddRef() { references.fetch_add(1); }
            uint32 GetRefs() { return references.load(); }
        };

        template<typename D>
        class TMetaImp : public TMeta {
        public:
            using TMeta::references;
            using TMeta::pointer;

            D deleter;

            TMetaImp(T* ptr, D d) : TMeta(ptr), deleter(std::move(d)) { }
            ~TMetaImp() override = default;

            bool Release() override {
                bool isLastRef = false;

                if (pointer) {
                    int32 prev = references.fetch_sub(1);

                    // The last one
                    if (prev == 1) {
                        isLastRef = true;
                        pointer->~T();
                        deleter(pointer);
                        pointer = nullptr;
                    }
                }

                return isLastRef;
            }
        };

        template<typename K>
        friend class TPtrShared;

        TMeta* mMetaRef = nullptr;
    };

}

#endif //BERSERK_TPTRSHARED_HPP