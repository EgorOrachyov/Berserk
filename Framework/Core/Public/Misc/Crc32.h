//
// Created by Egor Orachyov on 25.01.2019.
//

#ifndef BERSERK_CRC32_H
#define BERSERK_CRC32_H

#include "Public/Misc/Types.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
      * CRC hash generation for different types of input data
      */
    class CORE_API Crc32
    {

        /**
         * CRC-32 hash function for float32 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(float32 value);

        /**
         * CRC-32 hash function for int32 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(int32 value);

        /**
         * CRC-32 hash function for float64 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(float64 value);

        /**
         * CRC-32 hash function for int64 value
         *
         * @param value To be hashed
         * @return uint32 hash
         */
        static uint32 hash(int64 value);

        /**
         * CRC-32 hash function for chars array
         *
         * @param buffer Pointer to buffer
         * @param len Number of chars
         * @return uint32 hash
         */
        static uint32 hash(const char* buffer, uint32 len);

    };
}

#endif //BERSERK_CRC32_H
