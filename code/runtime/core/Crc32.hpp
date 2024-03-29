/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_CRC32_HPP
#define BERSERK_CRC32_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Crc32Hash
 * @brief Crc32 hash value type
 */
using Crc32Hash = uint32;

/**
 * @class Crc32
 * @brief Crc32 hashing utility
 */
class Crc32 final {
public:
    /**
     * Hash buffer of len size in bytes
     *
     * @param buffer Pointer to memory with buffer
     * @param size Num of bytes to hash
     *
     * @return CRC hash value of the buffer
     */
    BRK_API static Crc32Hash Hash(const void *buffer, size_t size);
};

/**
 * @class Crc32Builder
 * @brief Crc32 hash builder
 */
class Crc32Builder final {
public:
    /**
     * Appends hashed buffer to already stored hash
     *
     * @param buffer Pointer to memory with buffer
     * @param size Num of bytes to hash
     *
     * @return This builder to chain the hash
     */
    Crc32Builder &Hash(const void *buffer, size_t size) {
        Crc32Hash hash = Crc32::Hash(buffer, size);
        mHash ^= hash;

        return *this;
    }

    /** @return Crc32 hash value built within this builder */
    Crc32Hash GetHash() const {
        return mHash;
    }

private:
    Crc32Hash mHash = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_CRC32_HPP
