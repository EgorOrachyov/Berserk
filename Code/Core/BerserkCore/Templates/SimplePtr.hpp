/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SIMPLEPTR_HPP
#define BERSERK_SIMPLEPTR_HPP

namespace Berserk {

    template<typename T>
    class SimplePtr {
    public:

        template<typename U>
        bool operator==(const SimplePtr<U> &other) const { return mPtr == other.mPtr; }
        template<typename U>
        bool operator!=(const SimplePtr<U> &other) const { return mPtr != other.mPtr; }
        template<typename U>
        bool operator<=(const SimplePtr<U> &other) const { return mPtr <= other.mPtr; }
        template<typename U>
        bool operator>=(const SimplePtr<U> &other) const { return mPtr >= other.mPtr; }
        template<typename U>
        bool operator> (const SimplePtr<U> &other) const { return mPtr >  other.mPtr; }
        template<typename U>
        bool operator< (const SimplePtr<U> &other) const { return mPtr <  other.mPtr; }

        template<typename U>
        bool operator==(const U* other) const { return mPtr == other; }
        template<typename U>
        bool operator!=(const U* other) const { return mPtr != other; }
        template<typename U>
        bool operator<=(const U* other) const { return mPtr <= other; }
        template<typename U>
        bool operator>=(const U* other) const { return mPtr >= other; }
        template<typename U>
        bool operator> (const U* other) const { return mPtr >  other; }
        template<typename U>
        bool operator< (const U* other) const { return mPtr <  other; }

        SimplePtr(const SimplePtr&) = delete;
        SimplePtr(SimplePtr&&) = delete;

    protected:
        SimplePtr() = default;

        T* mPtr = nullptr;
    };

}

#endif //BERSERK_SIMPLEPTR_HPP
