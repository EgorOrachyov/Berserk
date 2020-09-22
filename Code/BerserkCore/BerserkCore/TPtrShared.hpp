/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TPTRSHARED_HPP
#define BERSERK_TPTRSHARED_HPP

#include <memory>

namespace Berserk {

    template <typename T>
    class TPtrShared {
    public:
        TPtrShared() = default;
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

#endif //BERSERK_TPTRSHARED_HPP