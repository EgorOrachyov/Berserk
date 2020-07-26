/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TPTR_H
#define BERSERK_TPTR_H

#include <Typedefs.h>

namespace Berserk {

    template <typename T>
    class TPtr {
    public:
        using type = T;

        bool operator==(const TPtr<T> &other) const { return mPtr == other.mPtr; }
        bool operator!=(const TPtr<T> &other) const { return mPtr != other.mPtr; }
        bool operator<=(const TPtr<T> &other) const { return mPtr <= other.mPtr; }
        bool operator>=(const TPtr<T> &other) const { return mPtr >= other.mPtr; }
        bool operator> (const TPtr<T> &other) const { return mPtr >  other.mPtr; }
        bool operator< (const TPtr<T> &other) const { return mPtr <  other.mPtr; }

        bool operator==(const T* other) const { return mPtr == other; }
        bool operator!=(const T* other) const { return mPtr != other; }
        bool operator<=(const T* other) const { return mPtr <= other; }
        bool operator>=(const T* other) const { return mPtr >= other; }
        bool operator> (const T* other) const { return mPtr >  other; }
        bool operator< (const T* other) const { return mPtr <  other; }

        T* operator->() { return mPtr; }
        T* operator->() const { return mPtr; }

        T& operator*() { return *mPtr; }
        T& operator*() const { return *mPtr; }

        T* getPtr() { return mPtr; }
        T* getPtr() const { return mPtr; }

        bool isNull() const { return mPtr == nullptr; }
        bool isNotNull() const { return mPtr != nullptr; }

    protected:
        T* mPtr;
    };

}

#endif //BERSERK_TPTR_H