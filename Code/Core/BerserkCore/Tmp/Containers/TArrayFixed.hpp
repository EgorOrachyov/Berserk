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

#ifndef BERSERK_TARRAYFIXED_HPP
#define BERSERK_TARRAYFIXED_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/TPredicates.hpp>
#include <BerserkCore/Logging.hpp>

namespace Berserk {

    template<typename T, uint32 C = 8>
    class TArrayFixed final {
    public:

        static const uint32 CAPACITY = C;

        TArrayFixed() noexcept = default;

        TArrayFixed(const std::initializer_list<T> &list) : TArrayFixed<T, C>() {
            Add(list);
        }

        TArrayFixed(const TArrayFixed &other) {
            Add(other);
        }

        TArrayFixed(TArrayFixed &&other) noexcept : mSize(other.mSize) {
            Memory::Copy(GetData(), other.GetData(), sizeof(T) * mSize);
            other.mSize = 0;
        }

        ~TArrayFixed() {
            if (mSize > 0) {
                Clear();
                mSize = 0;
            }
        }

        void Resize(uint32 size, const T &e = T()) {
            if (size > CAPACITY) {
                BERSERK_LOG_ERROR("Size is out of array capacity");
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
            if (!IsAbleToAddElement()) {
                BERSERK_FATAL_ERROR("Attempt to Add elements more than array capacity");
            }

            T *obj = new(&(GetData()[mSize])) T(std::forward<TArgs>(args)...);
            mSize += 1;

            return *obj;
        }

        void Add(const T &element) {
            if (!IsAbleToAddElement()) {
                BERSERK_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }

            T *obj = new(&(GetData()[mSize])) T(element);
            mSize += 1;
        }

        void Add(const std::initializer_list<T> &list) {
            if (!IsAbleToAddElements(list.size())) {
                BERSERK_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }
            for (const auto &e: list) {
                new(&(GetData()[mSize])) T(e);
                mSize += 1;
            }
        }

        template<uint32 K>
        void Add(const TArrayFixed<T, K> &other) {
            if (!IsAbleToAddElements(other.GetSize())) {
                BERSERK_LOG_ERROR("Attempt to Add elements more than array capacity");
                return;
            }

            for (uint32 i = 0; i < other.GetSize(); i++) {
                new(&(GetData()[mSize])) T(other[i]);
                mSize += 1;
            }
        }

        void Add(const T *buffer, uint32 count) {
            if (!IsAbleToAddElements(count)) {
                BERSERK_LOG_ERROR("Attempt to Add elements more than array capacity");
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
            if (index >= mSize) {
                BERSERK_LOG_ERROR("Index out of boumds");
                return;
            }

            GetData()[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::Copy(&(GetData()[index]), &(GetData()[index + 1]), sizeof(T) * (mSize - index));
            }
        }

        void RemoveUnordered(uint32 index) {
            if (index >= mSize) {
                BERSERK_LOG_ERROR("Index out of boumds");
                return;
            }

            GetData()[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory(&(GetData()[index]), &(GetData()[mSize]), sizeof(T));
            }
        }

        template<typename E = TEquals <T>>
        void RemoveElement(const T &toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(GetData()[i], toRemove)) {
                    Remove(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = TEquals <T>>
        void RemoveElementUnordered(const T &toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(GetData()[i], toRemove)) {
                    RemoveUnordered(i);
                    break;
                }

                i += 1;
            }
        }

        template<typename E = TEquals <T>>
        bool Contains(const T &element) const {
            E eq;
            for (const auto &e: *this) {
                if (eq(e, element))
                    return true;
            }
            return false;
        }

        TArrayFixed &operator=(const std::initializer_list<T> &list) {
            Clear();
            Add(list);
            return *this;
        }

        TArrayFixed &operator=(const TArrayFixed &other) {
            if (this == &other) {
                return *this;
            }

            Clear();
            Add(other);
            return *this;
        }

        TArrayFixed &operator=(TArrayFixed &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            Clear();
            mSize = other.mSize;
            Memory::Copy(GetData(), other.GetData(), sizeof(T) * mSize);
            other.mSize = 0;
            return *this;
        }

        T &operator[](uint32 index) {
            if (index >= mSize) {
                BERSERK_FATAL_ERROR("Index out of range");
            }

            return GetData()[index];
        }

        const T &operator[](uint32 index) const {
            if (index >= mSize) {
                BERSERK_FATAL_ERROR("Index out of range");
            }

            return GetData()[index];
        }

        void Clear() {
            for (uint32 i = 0; i < mSize; i++) {
                GetData()[i].~T();
            }
            mSize = 0;
        }

        template<typename E = TEquals <T>>
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

        template<typename H = THash<T>>
        uint32 Hash() const {
            H hashGenerator;
            uint64 h = 0;
            for (const auto &a: *this) {
                h += hashGenerator(a);
            }
            return Crc32::Hash(&h, sizeof(h));
        }

        template <typename Tit, typename Cit>
        class TIterator {
        public:

            TIterator(Cit* array, uint32 index) {
                mArray =  array;
                mIndex = index;
            }

            bool operator==(const TIterator& other) const {
                return (mArray == other.mArray) && ((mArray == nullptr) || (mIndex == other.mIndex));
            }

            bool operator!=(const TIterator& other) const {
                return !(*this == other);
            }

            void operator++() {
                if (mIndex + 1 < mArray->GetSize()) {
                    mIndex += 1;
                }
                else {
                    // If cannot increment - nullify the iterator
                    mArray = nullptr;
                    mIndex = 0;
                }
            }

            Tit& operator*() {
                return (*mArray)[mIndex];
            }

        protected:
            Cit* mArray;
            uint32 mIndex = 0;
        };

        class Iterator: public TIterator<T,TArrayFixed<T,C>> {
        public:
            using TIterator<T,TArrayFixed<T,C>>::mArray;
            using TIterator<T,TArrayFixed<T,C>>::mIndex;

            Iterator(TArrayFixed<T,C>* array, uint32 index) : TIterator<T,TArrayFixed<T,C>>(array, index) {

            }

            bool Remove() {
                if (mArray != nullptr && mIndex < mArray->GetSize()) {
                    mArray->Remove(mIndex);

                    // If after index no more elements - nullify the iterator
                    if (mIndex >= mArray->GetSize()) {
                        mArray = nullptr;
                        mIndex = 0;
                    }

                    return true;
                }

                return false;
            }
        };

        class ConstIterator: public TIterator<const T,const TArrayFixed<T,C>> {
        public:
            using TIterator<const T,const TArrayFixed<T,C>>::mArray;
            using TIterator<const T,const TArrayFixed<T,C>>::mIndex;

            ConstIterator(const TArrayFixed<T,C>* array, uint32 index) : TIterator<const T,const TArrayFixed<T,C>>(array, index) {

            }
        };

        Iterator begin() { return Iterator(IsNotEmpty()? this: nullptr, 0); }
        Iterator end() { return Iterator(nullptr, 0); }

        ConstIterator begin() const { return ConstIterator(IsNotEmpty()? this: nullptr, 0); }
        ConstIterator end() const { return ConstIterator(nullptr, 0); }

    private:
        uint32 mSize = 0;
        uint8 mBuffer[sizeof(T) * CAPACITY] = {};
    };

}

#endif //BERSERK_TARRAYFIXED_HPP
