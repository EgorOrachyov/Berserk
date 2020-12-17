/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_CRC32_HPP
#define BERSERK_CRC32_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {

    /**
     * @brief Crc32 hashing utility
     */
    class Crc32 {
    public:
        /**
         * Hash buffer of len size in bytes
         *
         * @param buffer Pointer to memory with buffer
         * @param size Num of bytes to hash
         *
         * @return CRC hash value of the buffer
         */
        static uint32 Hash(const void *buffer, uint32 size);
    };

    /**
     * @brief Crc32 hash builder
     */
    class Crc32Builder {
    public:

        /**
         * Appends hashed buffer to already stored hash
         *
         * @param buffer Pointer to memory with buffer
         * @param size Num of bytes to hash
         *
         * @return This builder to chain the hash
         */
        Crc32Builder &Hash(const void *buffer, uint32 size) {
            uint32 hash = Crc32::Hash(buffer, size);
            mHash ^= hash;

            return *this;
        }

        /**
         * @return Crc32 hash value built within this builder
         */
        uint32 GetHash() const {
            return mHash;
        }

    private:
        uint32 mHash = 0;
    };

}

#endif //BERSERK_CRC32_HPP