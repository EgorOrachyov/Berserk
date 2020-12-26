/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ARRAYFIXED_HPP
#define BERSERK_ARRAYFIXED_HPP

#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Misc/Contracts.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Assert.hpp>

namespace Berserk {

    template<typename T, uint32 Capacity = 8>
    class ArrayFixed final {
    public:

        static const uint32 CAPACITY = Capacity;

        ArrayFixed() noexcept = default;

        ArrayFixed(const std::initializer_list<T> &list) : ArrayFixed<T, Capacity>() {
            Add(list);
        }

        ArrayFixed(const ArrayFixed &other) {
            Add(other);
        }

        ArrayFixed(ArrayFixed &&other) noexcept : mSize(other.mSize) {
            Platform::Memory::Copy(GetData(), other.GetData(), sizeof(T) * mSize);
            other.mSize = 0;
        }

        ~ArrayFixed() {
            if (mSize > 0) {
                Clear();
                mSize = 0;
            }
        }

        void Resize(uint32 size, const T &e = T()) {
            BERSERK_ASSERT(size <= Capacity);

            if (size > CAPACITY) {
                BERSERK_CORE_LOG_ERROR("Size is out of array capacity");
                return;
            }

            if (size < mSize) {
                for (uint32 i = size; i < mSize; i++) {
                    Remove(i);
                }
            } else {
                for (uint32 i = mSize; i < size; i++) {
                    Emplace(e);
                }
            }
        }

        template<typename ... TArgs>
        T &Emplace(TArgs &&... args) {
            BERSERK_ASSERT(IsAbleToAddElement());

            if (!IsAbleToAddElement()) {
                BERSERK_CORE_LOG_FATAL("Attempt to Add elements more than array capacity");
            }

            T *obj = new(&(GetData()[mSize])) T(std::forward<TArgs>(args)...);
            mSize += 1;

            return *obj;
        }

        void Add(const T &element) {
            BERSERK_ASSERT(IsAbleToAddElement());

            if (!IsAbleToAddElement()) {
                BERSERK_CORE_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }

            T *obj = new(&(GetData()[mSize])) T(element);
            mSize += 1;
        }

        void Add(const std::initializer_list<T> &list) {
            BERSERK_ASSERT(IsAbleToAddElements(list.size()));

            if (!IsAbleToAddElements(list.size())) {
                BERSERK_CORE_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }
            for (const auto &e: list) {
                new(&(GetData()[mSize])) T(e);
                mSize += 1;
            }
        }

        template<uint32 K>
        void Add(const ArrayFixed<T, K> &other) {
            BERSERK_ASSERT(IsAbleToAddElements(other.GetSize()));

            if (!IsAbleToAddElements(other.GetSize())) {
                BERSERK_CORE_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }

            for (uint32 i = 0; i < other.GetSize(); i++) {
                new(&(GetData()[mSize])) T(other[i]);
                mSize += 1;
            }
        }

        void Add(const T *buffer, uint32 count) {
            BERSERK_ASSERT(IsAbleToAddElements(count));

            if (!IsAbleToAddElements(count)) {
                BERSERK_CORE_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }

            for (uint32 i = 0; i < count; i++) {
                new(&(GetData()[mSize])) T(buffer[i]);
                mSize += 1;
            }
        }

        void AddUnique(const std::initializer_list<T> &list) {
            for (const auto &e: list) {
                if (!Contains(e))
                    Add(e);
            }
        }

        void Remove(uint32 index) {
            BERSERK_ASSERT(index < mSize);

            if (index >= mSize) {
                BERSERK_CORE_LOG_ERROR("Index out of bounds");
                return;
            }

            GetData()[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Platform::Memory::Copy(&(GetData()[index]), &(GetData()[index + 1]), sizeof(T) * (mSize - index));
            }
        }

        void RemoveUnordered(uint32 index) {
            BERSERK_ASSERT(index < mSize);

            if (index >= mSize) {
                BERSERK_CORE_LOG_ERROR("Index out of bounds");
                return;
            }

            GetData()[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory(&(GetData()[index]), &(GetData()[mSize]), sizeof(T));
            }
        }

        template<typename E = Equals<T>>
        void RemoveElement(const T &toRemove, E&& equals = E()) {
            uint32 i = 0;
            while (i < mSize) {
                if (equals(GetData()[i], toRemove)) {
                    Remove(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = Equals <T>>
        void RemoveElementUnordered(const T &toRemove, E&& equals = E()) {
            uint32 i = 0;
            while (i < mSize) {
                if (equals(GetData()[i], toRemove)) {
                    RemoveUnordered(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = Equals<T>>
        bool Contains(const T &element, E&& equals = E()) const {
            E eq;
            for (const auto &e: *this) {
                if (eq(e, element))
                    return true;
            }
            return false;
        }

        ArrayFixed &operator=(const std::initializer_list<T> &list) {
            Clear();
            Add(list);
            return *this;
        }

        ArrayFixed &operator=(const ArrayFixed &other) {
            if (this == &other) {
                return *this;
            }

            Clear();
            Add(other);
            return *this;
        }

        ArrayFixed &operator=(ArrayFixed &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            Clear();
            mSize = other.mSize;
            Platform::Memory::Copy(GetData(), other.GetData(), sizeof(T) * mSize);
            other.mSize = 0;
            return *this;
        }

        T &operator[](uint32 index) {
            if (index >= mSize) {
                BERSERK_EXCEPT(InvalidOpException, "Index out of range");
            }

            return GetData()[index];
        }

        const T &operator[](uint32 index) const {
            if (index >= mSize) {
                BERSERK_EXCEPT(InvalidOpException, "Index out of range");
            }

            return GetData()[index];
        }

        void Clear() {
            for (uint32 i = 0; i < mSize; i++) {
                GetData()[i].~T();
            }
            mSize = 0;
        }

        template<typename E = Equals <T>>
        bool GetIndexOf(const T &element, uint32 &index) const {
            E eq;
            for (uint32 i = 0; i < GetSize(); i++) {
                if (eq((const T) mBuffer[i], element)) {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        bool IsAbleToAddElement() const { return mSize + 1 <= CAPACITY; }
        bool IsAbleToAddElements(uint32 toAdd) const { return mSize + toAdd <= CAPACITY; }

        bool IsEmpty() const { return mSize == 0; }
        bool IsNotEmpty() const { return mSize > 0; }

        T *GetData() { return (T *) mBuffer; }
        const T *GetData() const { return (const T *) mBuffer; }

        uint32 GetSize() const { return mSize; }
        uint32 GetCapacity() const { return CAPACITY; }
        uint32 GetSizeBytes() const { return mSize * sizeof(T); }

        template<typename H = Hash<T>>
        uint32 Hash(H&& hashGenerator = H()) const {
            uint64 h = 0;
            for (const auto &a: *this) {
                h += hashGenerator(a);
            }
            return Crc32::Hash(&h, sizeof(h));
        }

        template<typename TElement>
        class IteratorBase {
        protected:
            friend class ArrayFixed<T, Capacity>;

            IteratorBase(TElement *buffer, size_t index, size_t size)
                    : mBuffer(buffer), mIndex(index), mSize(size) {

            }

        public:

            bool operator==(const IteratorBase &other) const {
                return mBuffer == other.mBuffer && mIndex == other.mIndex;
            }

            bool operator!=(const IteratorBase &other) const {
                return !(*this == other);
            }

            void operator++() {
                if (mIndex + 1 < mSize) {
                    mIndex += 1;
                    return;
                }

                if (mBuffer != nullptr) {
                    mIndex = 0;
                    mSize = 0;
                    mBuffer = nullptr;
                }
            }

            TElement &operator*() {
                return mBuffer[mIndex];
            }

        protected:
            TElement *mBuffer;
            size_t mIndex = 0;
            size_t mSize = 0;
        };

        IteratorBase<T> begin() {
            return {IsNotEmpty() ? GetData() : nullptr, 0, GetSize()};
        }

        IteratorBase<T> end() {
            return {nullptr, 0, 0};
        }

        IteratorBase<const T> begin() const {
            return {IsNotEmpty() ? GetData() : nullptr, 0, GetSize()};
        }

        IteratorBase<const T> end() const {
            return {nullptr, 0, 0};
        }

    private:
        uint32 mSize = 0;
        uint8 mBuffer[sizeof(T) * CAPACITY] = {};
    };

}

#endif //BERSERK_ARRAYFIXED_HPP
