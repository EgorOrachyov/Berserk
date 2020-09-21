/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TARRAY_HPP
#define BERSERK_TARRAY_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <initializer_list>

namespace Berserk {

    template <typename T>
    class TArray {
    public:
        TArray();
        TArray(const std::initializer_list<T>& list);
        TArray(const TArray& other);
        TArray(TArray&& other);

        void add(const T& element);
        void add(const T* elements, uint32 count);
        void move(const T& element);

        template <typename ... TArgs>
        T& place(TArgs&& ... args);

        void remove(uint32 index);

        uint32 getSize() const { return mSize; }
        uint32 getCapacity() const { return mCapacity; }

        const T* getCBuffer() const { return mBuffer; }
        T* getCBuffer() { return mBuffer; }

        template <typename It>
        class Iterator {
        public:
        
        private:
            TArray& mArrayRef;
            uint32 mCurrentIndex;
        };

    private:
        T* mBuffer = nullptr;
        uint32 mSize;
        uint32 mCapacity;
    };

    template <typename T>
    void TArray<T>::add(const T &element) {

    }

    template <typename T>
    void TArray<T>::add(const T *elements, Berserk::uint32 count) {

    }

}

#endif //BERSERK_TARRAY_HPP