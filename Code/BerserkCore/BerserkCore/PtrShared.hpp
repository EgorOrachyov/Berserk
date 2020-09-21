/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PTRSHARED_HPP
#define BERSERK_PTRSHARED_HPP

#include <memory>

namespace Berserk {

    template <typename T>
    class PtrShared {
    public:
        PtrShared() = default;
        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr.get(); }
        operator bool() const { return isNotNull(); }
        bool isNull() const { return mPtr == nullptr; }
        bool isNotNull() const { return mPtr != nullptr; }
        T* get() const { return mPtr.get(); }
    private:
        std::shared_ptr<T> mPtr;
    };

}

#endif //BERSERK_PTRSHARED_HPP