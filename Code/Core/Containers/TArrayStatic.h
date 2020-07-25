/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TARRAYSTATIC_H
#define BERSERK_TARRAYSTATIC_H

#include <Containers/TIterable.h>
#include <IO/Archive.h>
#include <ErrorMacro.h>
#include <TPredicates.h>

namespace Berserk {

    template <typename T, uint32 C = 8>
    class TArrayStatic final : public TIterable<T> {
    public:

        static const uint32 CAPACITY = C;

        TArrayStatic() noexcept = default;
        TArrayStatic(const std::initializer_list<T> &list) : TArrayStatic<T,C>() {
            add(list);
        }

        template <uint32 K>
        TArrayStatic(const TArrayStatic<T,K>& other) : TArrayStatic<T,C>() {
            add(other);
        }

        template <uint32 K>
        TArrayStatic(TArrayStatic<T,K> &&other) noexcept
        : mSize(other.mSize) {
            BERSERK_COND_ERROR_FAIL(other.size(), "Size of other array more than this array capacity");
            Memory::copy(data(), other.data(), sizeof(T) * mSize);
            other.mSize = 0;
        }
        ~TArrayStatic() override {
            if (mSize > 0) {
                clear();
                mSize = 0;
            }
        }
        void resize(uint32 size, const T& e = T()) {
            BERSERK_COND_ERROR_FAIL(size <= CAPACITY, "Size is out of array capacity");

            if (size < mSize) {
                for (uint32 i = size; i < mSize; i++) {
                    remove(i);
                }
            }
            else {
                for (uint32 i = mSize; i < size; i++) {
                    emplace(e);
                }
            }
        }
        template <typename ... TArgs>
        T& emplace(TArgs && ... args) {

            T* obj = new(&(data()[mSize])) T(std::forward<TArgs>(args)...);
            mSize += 1;

            return *obj;
        }

        T& add(const T& element) {

            T* obj = new(&(data()[mSize])) T(element);
            mSize += 1;

            return *obj;
        }
        void add(const std::initializer_list<T> &list) {
            BERSERK_COND_ERROR_FAIL(ableToAddElements(list.size()), "Attempt to add elements more than array capacity");

            for (const auto& e: list) {
                new(&(data()[mSize])) T(e);
                mSize += 1;
            }
        }
        template <uint32 K>
        void add(const TArrayStatic<T,K> &other) {
            BERSERK_COND_ERROR_FAIL((void*)this != (void*)&other, "Containers must differ");
            BERSERK_COND_ERROR_FAIL(ableToAddElements(other.size()), "Attempt to add elements more than array capacity");

            for (uint32 i = 0; i < other.mSize; i++) {
                new(&(data()[mSize])) T(other[i]);
                mSize += 1;
            }
        }
        void add(const T* buffer, uint32 count) {
            BERSERK_COND_ERROR_FAIL(ableToAddElements(count), "Attempt to add elements more than array capacity");

            for (uint32 i = 0; i < count; i++) {
                new(&(data()[mSize])) T(buffer[i]);
                mSize += 1;
            }
        }
        void addUnique(const std::initializer_list<T> &list) {
            for (const auto& e: list) {
                if (!contains(e))
                    add(e);
            }
        }
        void remove(uint32 index) {
            if (index >= mSize) {
                BERSERK_ERROR_RET("Index out of range")
            }

            data()[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&(data()[index]), &(data()[index + 1]), sizeof(T) * (mSize - index));
            }
        }
        void removeUnordered(uint32 index) {
            if (index >= mSize) {
                BERSERK_ERROR_RET("Index out of range")
            }

            data()[index].~T();
            mSize -= 1;

            if (mSize != index) {
                Memory::copy(&(data()[index]), &(data()[mSize]), sizeof(T));
            }
        }
        uint32 removeMatchAll(const Function<bool(const T& a)> &predicate) {
            uint32 removed = 0;
            uint32 i = 0;
            while (i < mSize) {
                if (predicate(data()[i])) {
                    remove(i);
                    removed += 1;
                }
                else {
                    i += 1;
                }
            }

            return removed;
        }
        uint32 removeUnorderedMatchAll(const Function<bool(const T& a)> &predicate) {
            uint32 removed = 0;
            uint32 i = 0;
            while (i < mSize) {
                if (predicate(data()[i])) {
                    removeUnordered(i);
                    removed += 1;
                }
                else {
                    i += 1;
                }
            }

            return removed;
        }
        template <typename E = TEquals<T>>
        void removeElement(const T& toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(data()[i], toRemove)) {
                    remove(i);
                    break;
                }

                i += 1;
            }
        }
        template <typename E = TEquals<T>>
        void removeElementUnordered(const T& toRemove) {
            E equals;
            uint32 i = 0;
            while (i < mSize) {
                if (equals(data()[i], toRemove)) {
                    removeUnordered(i);
                    break;
                }

                i += 1;
            }
        }
        void removeElementPtr(const T* toRemove) {
            uint32 i = 0;
            while (i < mSize) {
                if (&(data()[i]) == toRemove) {
                    remove(i);
                    break;
                }

                i += 1;
            }
        }

        template <typename E = TEquals<T>>
        bool contains(const T& element) const {
            E eq;
            for (const auto& e: *this) {
                if (eq(e,element))
                    return true;
            }
            return false;
        }

        TArrayStatic& operator*=(uint32 N) {
            BERSERK_COND_ERROR_FAIL(N * mSize <= CAPACITY, "Attempt to add elements more than array capacity");

            if (N == 0) {
                clear();
            }
            else {
                auto oldSize = mSize;
                for (uint32 i = 1; i < N; i++) {
                    for (uint32 j = 0; j < oldSize; j++) {
                        new(&data()[mSize]) T(data()[j]);
                        mSize += 1;
                    }
                }
            }

            return *this;
        }
        TArrayStatic& operator+=(const T& element) {
            add(element);
            return *this;
        }
        TArrayStatic& operator+=(const std::initializer_list<T> &list) {
            add(list);
            return *this;
        }
        template <uint32 K>
        TArrayStatic& operator+=(const TArrayStatic<T,K>& other) {
            add(other);
            return *this;
        }
        TArrayStatic& operator=(const std::initializer_list<T> &list) {
            clear();
            add(list);
            return *this;
        }
        template <uint32 K>
        TArrayStatic& operator=(const TArrayStatic<T,K>& other) {
            BERSERK_COND_ERROR_FAIL(this != &other, "Containers must differ");
            clear();
            add(other);
            return *this;
        }
        template <uint32 K>
        TArrayStatic& operator=(TArrayStatic<T,K>&& other) noexcept {
            BERSERK_COND_ERROR_FAIL(this != &other, "Containers must differ");
            BERSERK_COND_ERROR_FAIL(other.size(), "Size of other array more than this array capacity");
            clear();
            mSize = other.mSize;
            Memory::copy(data(), other.data(), sizeof(T) * mSize);
            other.mSize = 0;
            return *this;
        }
        template <uint32 K>
        TArrayStatic<T,C + K> operator+(const TArrayStatic<T,K>& other) const {
            TArrayStatic<T,C + K> result;
            result += *this;
            result += other;
            return result;
        }
        T& operator[](uint32 index) {
            if (index >= mSize) {
                BERSERK_ERROR_FAIL("Index out of range")
            }

            return data()[index];
        }
        const T& operator[](uint32 index) const {
            if (index >= mSize) {
                BERSERK_ERROR_FAIL("Index out of range")
            }

            return data()[index];
        }

        void resizeExplicitly(uint32 size) {
            if (size <= CAPACITY) {
                mSize = size;
            }
        }
        void clear() {
            for (uint32 i = 0; i < mSize; i++) {
                data()[i].~T();
            }
            mSize = 0;
        }
        void clearNoDestructorCall() {
            mSize = 0;
        }

        void forEach(const Function<void(T&)> &function) override {
            for (uint32 i = 0; i < mSize; i++) {
                function(data()[i]);
            }
        }
        void forEach(const Function<void(const T&)> &function) const override {
            for (uint32 i = 0; i < mSize; i++) {
                function(data()[i]);
            }
        }

        template <typename E = TEquals<T>>
        bool getIndexOf(const T& element, uint32& index) const {
            E eq;
            for (uint32 i = 0; i < size(); i++) {
                if (eq((const T) mBuffer[i], element)) {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        bool ableToAddElement() const { return mSize + 1 <= CAPACITY; }
        bool ableToAddElements(uint32 toAdd) const { return mSize + toAdd <= CAPACITY; }
        T* data() { return (T*)mBuffer; }
        const T* data() const { return (const T*)mBuffer; }
        uint32 size() const { return mSize; }
        uint32 capacity() const { return CAPACITY; }

        const T* begin() const {
            return data();
        }
        const T* end() const {
            return data() + mSize;
        }

        T* begin() {
            return data();
        }
        T* end() {
            return data() + mSize;
        }

        template <typename H = THash<T>>
        uint32 hash() const {
            H hashGenerator;
            uint64 h = 0;
            for (const auto& a: *this) {
                h += hashGenerator(a);
            }
            return Crc32::hash(&h, sizeof(h));
        }

    private:

        /**
         * Serialize an array of objects in an archive.
         * @note T objects must support serialization operation '<<'
         * @param archive Archive to serialize data
         * @param array Array of the elements of type T to be serialized
         * @return archive
         */
        friend Archive& operator<<(Archive& archive, const TArrayStatic<T,C> &array) {
            auto elements = array.size();
            archive << elements;

            for (const auto& e: array) {
                archive << e;
            }

            return archive;
        }

        /**
         * Deserialize an array of objects from an archive.
         * @note T objects must support serialization operation '>>'
         * @param archive Archive to deserialize data
         * @param array Array of the elements of type T to be deserialized
         * @return archive
         */
        friend Archive& operator>>(Archive& archive, TArrayStatic<T,C> &array) {
            uint32 elements = 0;
            archive >> elements;
            BERSERK_COND_ERROR_FAIL(elements <= CAPACITY, "Unable to read all the elements from archive");

            for (uint32 i = 0; i < elements; i++) {
                T object;
                archive >> object;
                array.emplace(std::move(object));
            }

            return archive;
        }

    private:

        template <typename F, uint32 K>
        friend class TArrayStatic;

        uint32 mSize = 0;
        uint8 mBuffer[sizeof(T) * CAPACITY] = {};
    };

}

#endif //BERSERK_TARRAYSTATIC_H