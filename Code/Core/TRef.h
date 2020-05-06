/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TREF_H
#define BERSERK_TREF_H

#include <ErrorMacro.h>

namespace Berserk {

    /** @brief Error safe pointer wrapper */
    template <typename T>
    class TRef {
    public:

        TRef() = default;
        TRef(T& ref) : mPtr(&ref) { }
        TRef(T* ptr) : mPtr(ptr) { }
        TRef(const TRef& ref) = default;
        TRef(TRef&& ref) noexcept = default;
        ~TRef() = default;

        TRef& operator=(const TRef& ref) = default;
        TRef& operator=(TRef&& ref) noexcept = default;

        template <typename Callable>
        TRef& apply(Callable& callable) {
            if (isNotNull())
                callable(*mPtr);

            return *this;
        }

        template <typename Callable>
        const TRef& apply(const Callable& callable) const {
            if (isNotNull())
                callable(*mPtr);

            return *this;
        }

        T& operator*() { BERSERK_COND_ERROR_FAIL(mPtr != nullptr, "Null ptr"); return *mPtr; }
        const T& operator*() const { BERSERK_COND_ERROR_FAIL(mPtr != nullptr, "Null ptr"); return *mPtr; }

        T* operator->() { return mPtr; }
        const T* operator->() const { return mPtr; }

        T* getPtr() { return mPtr; }
        const T* getPtr() const { return mPtr; }

        bool isNull() const { return mPtr == nullptr; }
        bool isNotNull() const { return mPtr != nullptr; }

    private:
        T* mPtr = nullptr;
    };

}

#endif //BERSERK_TREF_H