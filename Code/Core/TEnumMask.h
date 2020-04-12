/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TENUMMASK_H
#define BERSERK_TENUMMASK_H

#include <Typedefs.h>
#include <ErrorMacro.h>

namespace Berserk {

    template <typename E>
    class TEnumMask {
    public:

        static const uint32 BITS_PER_BYTE = 8;
        static const uint32 MAX_BIT = sizeof(uint64) * BITS_PER_BYTE;

        TEnumMask() = default;
        TEnumMask(uint64 mask) : mMask(mask) {}
        TEnumMask(const TEnumMask& mask) = default;

        TEnumMask &operator=(const TEnumMask& mask) = default;

        void setFlag(E flag, bool value) {
            auto offset = (uint64) flag;
            BERSERK_COND_ERROR_RET(offset < MAX_BIT, "Flag bit out of mask max range");

            uint64 bit = (value? 1 : 0);
            uint64 index = ((uint64)1) << offset;
            uint64 mask = 0xffffffffffffffff ^ index;
            mMask = mMask & mask;
            mMask |= (bit) << offset;
        }

        bool getFlag(E flag) const {
            auto offset = (uint64) flag;
            BERSERK_COND_ERROR_RET_VALUE(false, offset < MAX_BIT, "Flag bit out of mask max range");

            return ((mMask >> offset) & 0x1) == 0x1;
        }

        uint64 getMask() const {
            return mMask;
        }

    private:
        uint64 mMask;
    };

}

#endif //BERSERK_TENUMMASK_H