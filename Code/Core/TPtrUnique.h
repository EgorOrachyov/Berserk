/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
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
            mFuncFree = [](T* ptr){ Memory::free(ptr); };
        }
        TPtrUnique(T* ptr, const Function<void(T*)> &free) noexcept {
            mPtr = ptr;
            mFuncFree = free;
        }
        TPtrUnique() noexcept {
            mPtr = nullptr;
        }
        TPtrUnique(TPtrUnique& other) noexcept
            : mFuncFree(std::move(other.mFuncFree)) {
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        TPtrUnique(TPtrUnique&& other) noexcept
            : mFuncFree(std::move(other.mFuncFree)) {
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        ~TPtrUnique() {
            if (mPtr) {
                mPtr->~T();
                mFuncFree(mPtr);
                mPtr = nullptr;
            }
        }
        TPtrUnique<T>& operator=(TPtrUnique& other) noexcept {
            mPtr = other.mPtr;
            mFuncFree = other.mFuncFree;
            other.mPtr = nullptr;
            return *this;
        }
        void free() {
            ~TPtrUnique<T>();
        }
        void freeNoDestruction() {
            mPtr = nullptr;
        }
    private:
        using TPtr<T>::mPtr;
        /** Called to deallocate ptr object after it is destructed */
        Function<void(T*)> mFuncFree;
    };

}

#endif //BERSERK_TPTRUNIQUE_H