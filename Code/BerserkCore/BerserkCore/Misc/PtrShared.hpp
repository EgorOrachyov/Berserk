/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PTRSHARED_HPP
#define BERSERK_PTRSHARED_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Memory.hpp>

namespace Berserk {

    template <typename T>
    class PtrShared {
    public:
        PtrShared() = default;

        explicit PtrShared(T* ptr) {
            if (ptr != nullptr) {
                auto& system = Platform::System::Instance();
                mMetaRef = new (system.AllocatePtrMeta(sizeof(TMeta))) TMeta(ptr);
            }
        }

        PtrShared(const PtrShared& other) noexcept {
            mMetaRef = other.mMetaRef;

            if (mMetaRef) {
                mMetaRef->AddRef();
            }
        }

        PtrShared(PtrShared&& other) noexcept {
            mMetaRef = other.mMetaRef;
            other.mMetaRef = nullptr;
        }

        ~PtrShared() noexcept {
            if (mMetaRef) {
                if (mMetaRef->Release()) {
                    mMetaRef->~TMeta();
                    auto& system = Platform::System::Instance();
                    system.DeallocatePtrMeta(mMetaRef);
                }

                mMetaRef = nullptr;
            }
        }

        PtrShared& operator=(const PtrShared<T>& other) {
            if (this == &other) {
                return *this;
            }

            this->~PtrShared();
            new (this) PtrShared<T>(other);
            return *this;
        }

        PtrShared& operator=(PtrShared<T>&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->~PtrShared();
            new (this) PtrShared<T>(std::move(other));
            return *this;
        }

        template<typename B>
        explicit operator PtrShared<B>() const {
            PtrShared<B> result;

            if (IsNotNull()) {
                result.mMetaRef = (typename PtrShared<B>::TMeta*) mMetaRef;
                result.mMetaRef->AddRef();
            }

            return result;
        }

        bool operator==(const PtrShared<T> &other) const { return mMetaRef == other.mMetaRef; }
        bool operator!=(const PtrShared<T> &other) const { return mMetaRef != other.mMetaRef; }
        bool operator<=(const PtrShared<T> &other) const { return GetPtrOrNull() <= other.GetPtrOrNull(); }
        bool operator>=(const PtrShared<T> &other) const { return GetPtrOrNull() >= other.GetPtrOrNull(); }
        bool operator> (const PtrShared<T> &other) const { return GetPtrOrNull() >  other.GetPtrOrNull(); }
        bool operator< (const PtrShared<T> &other) const { return GetPtrOrNull() <  other.GetPtrOrNull(); }

        bool operator==(const T* other) const { return mMetaRef == other; }
        bool operator!=(const T* other) const { return mMetaRef != other; }
        bool operator<=(const T* other) const { return GetPtrOrNull() <= other; }
        bool operator>=(const T* other) const { return GetPtrOrNull() >= other; }
        bool operator> (const T* other) const { return GetPtrOrNull() >  other; }
        bool operator< (const T* other) const { return GetPtrOrNull() <  other; }

        T* operator->() const { return mMetaRef->pointer;; }
        T& operator*() const { return *(mMetaRef->pointer); }

        explicit operator bool() const { return IsNotNull(); }

        bool IsNull() const { return mMetaRef == nullptr; }
        bool IsNotNull() const { return mMetaRef != nullptr; }

        T* GetPtrOrNull() const { return IsNotNull()? mMetaRef->pointer: nullptr; }
        uint32 GetReferencesCount() const { return IsNotNull()? mMetaRef->GetRefs(): 0; }

        static PtrShared MakeMove(T&& value) {
            auto& system = Platform::System::Instance();

            PtrShared result;
            T* created = new (Platform::Memory::Allocate(sizeof(T))) T(std::move(value));
            result.mMetaRef = new (system.AllocatePtrMeta(sizeof(TMeta))) TMeta(created);
            return result;
        }

        template<typename ... TArgs>
        static PtrShared MakeFromArgs(TArgs&& ... args) {
            auto& system = Platform::System::Instance();

            PtrShared result;
            T* created = new (Platform::Memory::Allocate(sizeof(T))) T(std::forward<TArgs>(args)...);
            result.mMetaRef = new (system.AllocatePtrMeta(sizeof(TMeta))) TMeta(created);
            return result;
        }

    private:

        class TMeta {
        public:
            Platform::AtomicInt32 references;
            T* pointer = nullptr;

            explicit TMeta(T* ptr) : references(1), pointer(ptr) { }
            virtual ~TMeta() = default;

            virtual bool Release() {
                bool isLastRef = false;

                if (pointer) {
                    int32 prev = references.fetch_sub(1);

                    // The last one
                    if (prev == 1) {
                        isLastRef = true;
                        pointer->~T();
                        Platform::Memory::Deallocate(pointer);
                        pointer = nullptr;
                    }
                }

                return isLastRef;
            };

            void AddRef() { references.fetch_add(1); }
            uint32 GetRefs() { return references.load(); }
        };

        template<typename DeleterType>
        class TMetaImp : public TMeta {
        public:
            using TMeta::references;
            using TMeta::pointer;

            DeleterType deleter;

            TMetaImp(T* ptr, DeleterType&& d) : TMeta(ptr), deleter(std::move(d)) { }
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
        friend class PtrShared;

        TMeta* mMetaRef = nullptr;
    };

}

#endif //BERSERK_PTRSHARED_HPP