/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CRC32_H
#define BERSERK_CRC32_H

#include <Typedefs.h>

namespace Berserk {

    /** Crc32 hashing utility */
    class Crc32 {
    public:
        /**
         * Hash buffer of len size in bytes
         * @param buffer Pointer to memory with buffer
         * @param size Num of bytes to hash
         * @return CRC hash value of the buffer
         */
        static uint32 hash(const void* buffer, uint32 size);

        /** Additive hashing utility */
        class Builder {
        public:
            void hash(const void* buffer, uint32 size);
            uint32 getHash() const;
        private:
            uint32 mHashValue = 0xFFFFFFFF;
        };

    };

}

#endif //BERSERK_CRC32_H