/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TARRAYVIEW_H
#define BERSERK_TARRAYVIEW_H

#include <Containers/TArray.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {

    template <typename T>
    class TArrayView final : public TIterable<T> {
    public:

        TArrayView() = default;
        TArrayView(const TArrayView& other) = default;
        TArrayView(TArrayView&& other) noexcept = default;
        ~TArrayView() override = default;

        TArrayView(uint32 size, T* buffer) {
            mSize = size;
            mBuffer = buffer;
        }

        TArrayView(TArray<T> &array) {
            mSize = array.size();
            mBuffer = array.data();
        }

        TArrayView(TArray<T> &array, uint32 offset, uint32 count) {
            BERSERK_COND_ERROR_RET(offset + count <= array.size(), "Out of the array bounds view area");

            mSize = count;
            mBuffer = array.data() + offset;
        }

        template <uint32 C>
        TArrayView(TArrayStatic<T,C>& array) {
            mSize = array.size();
            mBuffer = array.data();
        }

        template <uint32 C>
        TArrayView(TArrayStatic<T,C>& array, uint32 offset, uint32 count) {
            BERSERK_COND_ERROR_RET(offset + count <= array.size(), "Out of the array bounds view area");

            mSize = count;
            mBuffer = array.data() + offset;
        }

        T& operator[](uint32 i) {
            BERSERK_COND_ERROR_FAIL(i < mSize, "Index out of bounds");
            return mBuffer[i];
        }

        const T& operator[](uint32 i) const {
            BERSERK_COND_ERROR_FAIL(i < mSize, "Index out of bounds");
            return mBuffer[i];
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

        template <typename E = TEquals<T>>
        bool getIndexOf(const T& element, uint32& index) const {
            E eq;
            for (uint32 i = 0; i < size(); i++) {
                if (eq(mBuffer[i], element)) {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        void forEach(const Function<void(T&)> &function) override {
            for (uint32 i = 0; i < mSize; i++) {
                function(mBuffer[i]);
            }
        }
        void forEach(const Function<void(const T&)> &function) const override {
            for (uint32 i = 0; i < mSize; i++) {
                function(mBuffer[i]);
            }
        }

        bool isEmpty() const {
            return mSize == 0;
        }

        bool isNotEmpty() const {
            return mSize > 0;
        }

        uint32 size() const {
            return mSize;
        }

        T* data() {
            return mBuffer;
        }

        const T* data() const {
            return mBuffer;
        }

        const T* begin() const {
            return mBuffer;
        }

        const T* end() const {
            return mBuffer + mSize;
        }

        T* begin() {
            return mBuffer;
        }

        T* end() {
            return mBuffer + mSize;
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
        uint32 mSize = 0;
        T* mBuffer = nullptr;
    };

}

#endif //BERSERK_TARRAYVIEW_H