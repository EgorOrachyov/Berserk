/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TPTRUNIQUE_H
#define BERSERK_TPTRUNIQUE_H

#include <TPtr.h>
#include <Platform/Memory.h>

namespace Berserk {

    template <typename T>
    class TPtrUnique : public TPtr<T> {
    public:
        TPtrUnique(T* ptr) noexcept {
            mPtr = ptr;
            mFuncFree = nullptr;
        }
        TPtrUnique(T* ptr, const Function<void(void*)> *free) noexcept {
            mPtr = ptr;
            mFuncFree = free;
        }
        TPtrUnique() noexcept {
            mPtr = nullptr;
        }
        TPtrUnique(TPtrUnique&& other) noexcept {
            mPtr = other.mPtr;
            mFuncFree = other.mFuncFree;
            other.mPtr = nullptr;
            other.mFuncFree = nullptr;
        }
        ~TPtrUnique() {
            if (mPtr) {
                mPtr->~T();
                if (mFuncFree) (*mFuncFree)(mPtr);
                mPtr = nullptr;
            }
        }
        TPtrUnique<T>& operator=(TPtrUnique&& other) noexcept {
            this->~TPtrUnique<T>();
            mPtr = other.mPtr;
            mFuncFree = other.mFuncFree;
            other.mPtr = nullptr;
            other.mFuncFree = nullptr;
            return *this;
        }
        void free() {
            this->~TPtrUnique<T>();
        }
        template <typename ... TArgs>
        static TPtrUnique<T> make(TArgs &&... args) {
            void* mem = Memory::allocate(sizeof(T));
            T* obj = new (mem) T(std::forward<TArgs>(args)...);
            return TPtrUnique<T>(obj,&Memory::DEFAULT_DEALLOC);
        }
    private:
        using TPtr<T>::mPtr;
        /** Called to deallocate ptr object after it is destructed */
        const Function<void(void*)> *mFuncFree = nullptr;
    };

}

#endif //BERSERK_TPTRUNIQUE_H