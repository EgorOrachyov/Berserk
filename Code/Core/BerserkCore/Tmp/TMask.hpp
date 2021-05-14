/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TMASK_HPP
#define BERSERK_TMASK_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <initializer_list>

namespace Berserk {

    template <typename T, typename S = uint32>
    class TMask {
    public:
        TMask() = default;
        TMask(const TMask& other) = default;
        TMask(TMask&& other) noexcept = default;
        ~TMask() = default;

        TMask(const std::initializer_list<T>& bits) { for (auto bit: bits) { IntSetBit(bit); } }

        TMask& operator=(const TMask& other) = default;
        TMask& operator=(TMask&& other) noexcept = default;

        bool operator==(const TMask& other) const { return mStore == other.mStore; }
        bool operator!=(const TMask& other) const { return mStore != other.mStore; }

        TMask operator&(const TMask& other) const { TMask r; r.mStore = mStore & other.mStore; return r; }
        TMask operator|(const TMask& other) const { TMask r; r.mStore = mStore | other.mStore; return r; }
        TMask operator^(const TMask& other) const { TMask r; r.mStore = mStore ^ other.mStore; return r; }

        TMask AddBit(T bit) const { TMask r; r.AddBit(bit); return r; }
        TMask RemoveBit(T bit) const { TMask r; r.IntRemoveBit(bit); return r; }

        bool GetBit(T bit) const { return (mStore & (1u << (uint32)bit)) != 0; };
        S GetValue() const { return mStore; }

    private:
        void IntSetBit(T bit) { mStore |= 1u << (uint32)bit; }
        void IntRemoveBit(T bit) { mStore &= (~0u) ^ (1u << (uint32)bit); }

        S mStore = 0;
    };

}

#endif //BERSERK_TMASK_HPP