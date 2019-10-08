//
// Created by Egor Orachyov on 2019-10-07.
//

#ifndef BERSERK_TSTATICARRAY_H
#define BERSERK_TSTATICARRAY_H

#include <TComparator.h>
#include <Containers/TList.h>
#include <Misc/Error.h>
#include <HAL/Memory.h>

namespace Berserk {

    template <typename T, uint32 CAPACITY = 16, typename C = TEquals<T>>
    class TStaticArray : public TList<T> {
    public:

        TStaticArray(const std::initializer_list<T> &list) {
            auto elements = (T*) mBuffer;
            for (auto &data : list) {
                if (mSize >= CAPACITY) {
                    return;
                }

                new (&elements[mSize]) T(data);
                mSize += 1;
            }
        }

        /** @copydoc TList::() */
        void add(const T &element) override {
            T* memory = addUninitialized();
            new (memory) T(element);

        }

        /** @copydoc TList::() */
        T *addUninitialized() override {
            if (mSize >= CAPACITY) {
                dev_error("Cannot add element, array is full");
            }

            auto index = mSize;
            mSize += 1;
            auto elements = (T*) mBuffer;
            return &elements[index];
        }

        /** @copydoc TList::() */
        void append(const TList<T> &container) override {

        }

        /** @copydoc TList::() */
        void append(const T *array, uint32 count) override {

        }

        /** @copydoc TList::() */
        void append(const std::initializer_list<T> &list) override {
            auto elements = (T*) mBuffer;
            for (auto &data : list) {
                if (mSize >= CAPACITY) {
                    return;
                }

                new (&elements[mSize]) T(data);
                mSize += 1;
            }
        }

        /** @copydoc TList::() */
        T &get(uint32 index) const override {
            if (index >= mSize) {
                dev_error("Index out of bounds [size: %u][index: %u]", mSize, index);
            }

            return getBuffer()[index];
        }

        /** @copydoc TList::() */
        T *find(TPredicate::Satisfy<T> predicate) const override {

        }

        /** @copydoc TList::() */
        void remove(uint32 index) override {

        }

        /** @copydoc TList::() */
        void clear() override {
            auto elements = (T*) mBuffer;
            for (int32 i = 0; i < mSize; i++) {
                elements[i].~T();
            }
            mSize = 0;
        }

        /** @copydoc TList::() */
        void clearNoDestructorCall() override {
            mSize = 0;
        }

        /** @copydoc TList::() */
        void sort(TPredicate::Compare<T> predicate) override {

        }

        /** @return Raw internal buffer */
        T* getBuffer() const {
            return (T*) mBuffer;
        }

        /** @copydoc TList::() */
        uint32 getSize() const override {
            return mSize;
        }

        /** @copydoc TList::() */
        uint32 getMemoryUsage() const override {
            return sizeof(TStaticArray);
        }

        T* begin() const {
            return & getBuffer()[0];
        }

        T* end() const {
            return & getBuffer()[mSize];
        }

    private:

        uint32 mSize = 0;
        uint8 mBuffer[CAPACITY * sizeof(T)];

    };

}

#endif //BERSERK_TSTATICARRAY_H