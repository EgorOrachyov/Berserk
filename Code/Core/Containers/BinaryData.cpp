/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Containers/BinaryData.h>

namespace Berserk {

    void BinaryData::add(const uint8 *buffer, uint32 bufferSize) {
        auto oldSize = mData.size();
        mData.resize(oldSize + bufferSize);
        Memory::copy(mData.data() + oldSize, buffer, bufferSize);
    }

    void BinaryData::write(Berserk::uint32 offset, const Berserk::uint8 *buffer, Berserk::uint32 bufferSize) {
        BERSERK_COND_ERROR_RET(offset + bufferSize <= size(), "Out of range section");
        Memory::copy(mData.data() + offset, buffer, bufferSize);
    }

}