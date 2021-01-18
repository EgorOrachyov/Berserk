/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MASK_HPP
#define BERSERK_MASK_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {

    template<typename Enum, typename StoreType = uint32>
    class Mask {
    public:
        Mask() = default;

        Mask(const std::initializer_list<Enum> &bits) {
            for (auto bit: bits) {
                auto offset = (StoreType) bit;
                mMask |= ((StoreType) 1) << offset;
            }
        }

        ~Mask() = default;

        Mask& Set(Enum bit) {
            auto offset = (StoreType) bit;
            mMask |= ((StoreType) 1) << offset;
            return *this;
        }

        Mask& Remove(Enum bit) {
            auto offset = (StoreType) bit;
            auto mask = (StoreType)(((StoreType) 1) << offset);
            mMask &= (~mask);
            return *this;
        }

        bool Get(Enum bit) const {
            auto offset = (StoreType) bit;
            return (mMask & (((StoreType) 1) << offset)) != 0;
        }

        StoreType GetMask() const { return mMask; }

    private:
        StoreType mMask = 0;
    };

}

#endif //BERSERK_MASK_HPP